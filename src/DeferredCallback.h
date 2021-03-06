/******************************************************************************
 * Copyright 2009-2011, Qualcomm Innovation Center, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 ******************************************************************************/
#ifndef _ALLJOYN_C_DEFERREDCALLBACK_H
#define _ALLJOYN_C_DEFERREDCALLBACK_H

#include <alljoyn_c/AjAPI.h>
#include <list>
#include <signal.h>
#include <qcc/Mutex.h>
#include <qcc/Thread.h>

/*
 * The DeferredCallback class is used by the AllJoyn Unity Extension to force
 * callbacks to be returned from the main thread.  The reason for this is when
 * running Unity on Android devices a single-apartment thread model is used.
 * By default AllJoyn's callbacks come back on there own threads.
 *
 * If the static variable sMainThreadCallbacksOnly is set to true when the
 * callback is received it will be added to a pending callbacks list. The
 * programmer is responsible for regularly calling
 * DeferredCallback::TriggerCallbacks to process the callbacks that have been
 * added to the pending callbacks list.
 *
 * In the code deferred callbacks are expected to be used as follows.
 * @code
 * if (!DeferredCallback::sMainThreadCallbacksOnly) {
 *     callbacks.bus_disconnected(context);
 * } else {
 *    DeferredCallback_1<void, const void*>* dcb =
 *        new DeferredCallback_1<void, const void*>(callbacks.bus_disconnected, context);
 *    DEFERRED_CALLBACK_EXECUTE(dcb);
 * }
 * @endcode
 *
 * note the dcb pointer is not deleted.  It will be deleted in the
 * TriggerCallbacks method after the callback has been processed.
 *
 * Since the dcb pointer is only freed if the TriggerCallbacks is called the
 * DeferredCallback class should only be used if the variable
 * sMainThreadCallbackOnly is true.
 *
 * By default sMainThreadCallbackOnly is false.  This can be change from the C
 * bindings by calling
 * @code
 * alljoyn_unity_set_deferred_callback_mainthread_only(QCC_TRUE)
 * @endcode
 *
 * The TriggerCallbacks can be called from the C bindings by calling
 * @code
 * alljoyn_unity_deferred_callbacks_process()
 * @endcode
 *
 * The DeferredCallback class is explicitly designed for usage in a specific
 * situation, the AllJoyn Unity Extension on Android.  The default settings
 * (which do not used the DeferredCallback) should not be changed to used the
 * DeferredCallback except in that specific situation or for testing.
 */

//#define DEBUG_DEFERRED_CALLBACKS 1

#if DEBUG_DEFERRED_CALLBACKS
#   include <stdio.h>
#   define DEFERRED_CALLBACK_EXECUTE(cb) cb->Execute(); printf("%s (%d) -- Executing on %s thread\n", __FILE__, __LINE__, DeferredCallback::IsMainThread() ? "main" : "alternate")
#else
#   define DEFERRED_CALLBACK_EXECUTE(cb) cb->Execute()
#endif

namespace ajn {

class DeferredCallback {
  public:
    DeferredCallback() : finished(false), executeNow(false) {
        if (!initilized) {
            sMainThread = qcc::Thread::GetThread();
            initilized = true;
        }
    }

    virtual ~DeferredCallback() { }

    static int TriggerCallbacks()
    {
        int ret = 0;
        while (!sPendingCallbacks.empty()) {
            sCallbackListLock.Lock(MUTEX_CONTEXT);
            if (sPendingCallbacks.empty()) {
                sCallbackListLock.Unlock(MUTEX_CONTEXT);
                break;
            }
            DeferredCallback* cb = sPendingCallbacks.front();
            sPendingCallbacks.pop_front();
            sCallbackListLock.Unlock(MUTEX_CONTEXT);
            cb->runCallbackNow();
            while (!cb->finished)
                qcc::Sleep(1);
            delete cb;
            ret++;
        }
        return ret;
    }

    static bool IsMainThread()
    {
        return (sMainThreadCallbacksOnly ? (sMainThread == qcc::Thread::GetThread()) : true);
    }

  protected:
    virtual void runCallbackNow() { };

    void Wait()
    {
        while (!executeNow)
            qcc::Sleep(1);
    }

    class ScopeFinishedMarker {
      public:
        ScopeFinishedMarker(volatile sig_atomic_t* finished) : finishedSig(finished) { }
        ~ScopeFinishedMarker() { *finishedSig = true; }
      private:
        volatile sig_atomic_t* finishedSig;
    };
  public:
    static bool sMainThreadCallbacksOnly;

  protected:
    static bool initilized;
    volatile sig_atomic_t finished;
    static std::list<DeferredCallback*> sPendingCallbacks;
    static qcc::Thread* sMainThread;
    static qcc::Mutex sCallbackListLock;

    volatile sig_atomic_t executeNow;
  private:
};

template <typename R, typename T>
class DeferredCallback_1 : public DeferredCallback {
  public:
    typedef R (*DeferredCallback_1_Callback)(T arg1);

    DeferredCallback_1(DeferredCallback_1_Callback callback, T param1) : _callback(callback), _param1(param1)
    {
    }

    virtual void runCallbackNow() {
        retVal = _callback(_param1);
        executeNow = true;
    }

    virtual R Execute()
    {
        ScopeFinishedMarker finisher(&finished);
        if (!sMainThreadCallbacksOnly) {
            runCallbackNow();
        } else {
            sCallbackListLock.Lock(MUTEX_CONTEXT);
            sPendingCallbacks.push_back(this);
            sCallbackListLock.Unlock(MUTEX_CONTEXT);
            if (!IsMainThread()) {
                Wait();
            }
        }
        R ret = retVal;
        return ret;
    }

  protected:
    DeferredCallback_1_Callback _callback;
    T _param1;
    R retVal;
};

template <typename T>
class DeferredCallback_1<void, T> : public DeferredCallback {
  public:
    typedef void (*DeferredCallback_1_Callback)(T arg1);

    DeferredCallback_1(DeferredCallback_1_Callback callback, T param1) : _callback(callback), _param1(param1)
    {
    }

    virtual void runCallbackNow() {
        _callback(_param1);
        executeNow = true;
    }

    virtual void Execute()
    {
        ScopeFinishedMarker finisher(&finished);
        if (!sMainThreadCallbacksOnly) {
            runCallbackNow();
        } else {
            sCallbackListLock.Lock(MUTEX_CONTEXT);
            sPendingCallbacks.push_back(this);
            sCallbackListLock.Unlock(MUTEX_CONTEXT);
            if (!IsMainThread()) {
                Wait();
            }
        }
    }

  protected:
    DeferredCallback_1_Callback _callback;
    T _param1;
};

template <typename R, typename T, typename U>
class DeferredCallback_2 : public DeferredCallback {
  public:
    typedef R (*DeferredCallback_2_Callback)(T arg1, U arg2);

    DeferredCallback_2(DeferredCallback_2_Callback callback, T param1, U param2) : _callback(callback), _param1(param1), _param2(param2)
    {
    }

    virtual void runCallbackNow() {
        retVal = _callback(_param1, _param2);
        executeNow = true;
    }

    virtual R Execute()
    {
        ScopeFinishedMarker finisher(&finished);
        if (!sMainThreadCallbacksOnly) {
            runCallbackNow();
        } else {
            sCallbackListLock.Lock(MUTEX_CONTEXT);
            sPendingCallbacks.push_back(this);
            sCallbackListLock.Unlock(MUTEX_CONTEXT);
            if (!IsMainThread()) {
                Wait();
            }
        }
        R ret = retVal;
        return ret;
    }

  protected:
    DeferredCallback_2_Callback _callback;
    T _param1;
    U _param2;
    R retVal;
};

template <typename T, typename U>
class DeferredCallback_2<void, T, U> : public DeferredCallback {
  public:
    typedef void (*DeferredCallback_2_Callback)(T arg1, U arg2);

    DeferredCallback_2(DeferredCallback_2_Callback callback, T param1, U param2) : _callback(callback), _param1(param1), _param2(param2)
    {
    }

    virtual void runCallbackNow() {
        _callback(_param1, _param2);
        executeNow = true;
    }

    virtual void Execute()
    {
        ScopeFinishedMarker finisher(&finished);
        if (!sMainThreadCallbacksOnly) {
            runCallbackNow();
        } else {
            sCallbackListLock.Lock(MUTEX_CONTEXT);
            sPendingCallbacks.push_back(this);
            sCallbackListLock.Unlock(MUTEX_CONTEXT);
            if (!IsMainThread()) {
                Wait();
            }
        }
    }

  protected:
    DeferredCallback_2_Callback _callback;
    T _param1;
    U _param2;
};

template <typename R, typename T, typename U, typename V>
class DeferredCallback_3 : public DeferredCallback {
  public:
    typedef R (*DeferredCallback_3_Callback)(T arg1, U arg2, V arg3);

    DeferredCallback_3(DeferredCallback_3_Callback callback, T param1, U param2, V param3) :
        _callback(callback), _param1(param1), _param2(param2), _param3(param3)
    {
    }

    virtual void runCallbackNow() {
        retVal = _callback(_param1, _param2, _param3);
        executeNow = true;
    }

    virtual R Execute()
    {
        ScopeFinishedMarker finisher(&finished);
        if (!sMainThreadCallbacksOnly) {
            runCallbackNow();
        } else {
            sCallbackListLock.Lock(MUTEX_CONTEXT);
            sPendingCallbacks.push_back(this);
            sCallbackListLock.Unlock(MUTEX_CONTEXT);
            if (!IsMainThread()) {
                Wait();
            }
        }
        R ret = retVal;
        return ret;
    }

  protected:
    DeferredCallback_3_Callback _callback;
    T _param1;
    U _param2;
    V _param3;
    R retVal;
};

template <typename T, typename U, typename V>
class DeferredCallback_3<void, T, U, V> : public DeferredCallback {
  public:
    typedef void (*DeferredCallback_3_Callback)(T arg1, U arg2, V arg3);

    DeferredCallback_3(DeferredCallback_3_Callback callback, T param1, U param2, V param3) :
        _callback(callback), _param1(param1), _param2(param2), _param3(param3)
    {
    }

    virtual void runCallbackNow() {
        _callback(_param1, _param2, _param3);
        executeNow = true;
    }

    virtual void Execute()
    {
        ScopeFinishedMarker finisher(&finished);
        if (!sMainThreadCallbacksOnly) {
            runCallbackNow();
        } else {
            sCallbackListLock.Lock(MUTEX_CONTEXT);
            sPendingCallbacks.push_back(this);
            sCallbackListLock.Unlock(MUTEX_CONTEXT);
            if (!IsMainThread()) {
                Wait();
            }
        }
    }

  protected:
    DeferredCallback_3_Callback _callback;
    T _param1;
    U _param2;
    V _param3;
};

template <typename R, typename T, typename U, typename V, typename W>
class DeferredCallback_4 : public DeferredCallback {
  public:
    typedef R (*DeferredCallback_4_Callback)(T arg1, U arg2, V arg3, W arg4);

    DeferredCallback_4(DeferredCallback_4_Callback callback, T param1, U param2, V param3, W param4) :
        _callback(callback), _param1(param1), _param2(param2), _param3(param3), _param4(param4)
    {
    }

    virtual void runCallbackNow() {
        retVal = _callback(_param1, _param2, _param3, _param4);
        executeNow = true;
    }

    virtual R Execute()
    {
        ScopeFinishedMarker finisher(&finished);
        if (!sMainThreadCallbacksOnly) {
            runCallbackNow();
        } else {
            sCallbackListLock.Lock(MUTEX_CONTEXT);
            sPendingCallbacks.push_back(this);
            sCallbackListLock.Unlock(MUTEX_CONTEXT);
            if (!IsMainThread()) {
                Wait();
            }
        }
        R ret = retVal;
        return ret;
    }

  protected:
    DeferredCallback_4_Callback _callback;
    T _param1;
    U _param2;
    V _param3;
    W _param4;
    R retVal;
};

template <typename T, typename U, typename V, typename W>
class DeferredCallback_4<void, T, U, V, W> : public DeferredCallback {
  public:
    typedef void (*DeferredCallback_4_Callback)(T arg1, U arg2, V arg3, W arg4);

    DeferredCallback_4(DeferredCallback_4_Callback callback, T param1, U param2, V param3, W param4) :
        _callback(callback), _param1(param1), _param2(param2), _param3(param3), _param4(param4)
    {
    }

    virtual void runCallbackNow() {
        _callback(_param1, _param2, _param3, _param4);
        executeNow = true;
    }


    virtual void Execute()
    {
        ScopeFinishedMarker finisher(&finished);
        if (!sMainThreadCallbacksOnly) {
            runCallbackNow();
        } else {
            sCallbackListLock.Lock(MUTEX_CONTEXT);
            sPendingCallbacks.push_back(this);
            sCallbackListLock.Unlock(MUTEX_CONTEXT);
            if (!IsMainThread()) {
                Wait();
            }
        }
    }

  protected:
    DeferredCallback_4_Callback _callback;
    T _param1;
    U _param2;
    V _param3;
    W _param4;
};
}

#endif
