/******************************************************************************
 * Copyright 2010-2013, Qualcomm Innovation Center, Inc.
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
#ifndef _ALLJOYN_C_PROXYBUSOBJECTLISTENERC_H
#define _ALLJOYN_C_PROXYBUSOBJECTLISTENERC_H
#include <alljoyn/ProxyBusObject.h>
namespace ajn {
/*
 * When setting up a asynchronous Introspection call a callback handler for C
 * alljoyn_proxybusobject_listener_introspectcb_ptr function pointer will be
 * passed in as the callback handler.  AllJoyn expects a
 * ProxyBusObject::Listener::IntrospectCB method handler.  The function handler
 * will be passed as the part of the void* context that is passed to the internal
 * callback handler and will then be used to map the internal callback handler
 * to the user defined alljoyn_proxybusobject_listener_introspectcb_ptr callback
 * function pointer.
 */
class IntrospectCallbackContext {
  public:
    IntrospectCallbackContext(alljoyn_proxybusobject_listener_introspectcb_ptr replyhandler_ptr, void* context) :
        replyhandler_ptr(replyhandler_ptr), context(context) { }

    alljoyn_proxybusobject_listener_introspectcb_ptr replyhandler_ptr;
    void* context;
};

class GetPropertyCallbackContext {
  public:
    GetPropertyCallbackContext(alljoyn_proxybusobject_listener_getpropertycb_ptr replyhandler_ptr, void* context) :
        replyhandler_ptr(replyhandler_ptr), context(context) { }

    alljoyn_proxybusobject_listener_getpropertycb_ptr replyhandler_ptr;
    void* context;
};

class GetAllPropertiesCallbackContext {
  public:
    GetAllPropertiesCallbackContext(alljoyn_proxybusobject_listener_getallpropertiescb_ptr replyhandler_ptr, void* context) :
        replyhandler_ptr(replyhandler_ptr), context(context) { }

    alljoyn_proxybusobject_listener_getallpropertiescb_ptr replyhandler_ptr;
    void* context;
};

class SetPropertyCallbackContext {
  public:
    SetPropertyCallbackContext(alljoyn_proxybusobject_listener_setpropertycb_ptr replyhandler_ptr, void* context) :
        replyhandler_ptr(replyhandler_ptr), context(context) { }

    alljoyn_proxybusobject_listener_setpropertycb_ptr replyhandler_ptr;
    void* context;
};

class ProxyBusObjectListenerC : public ajn::ProxyBusObject::Listener {
  public:
    void IntrospectCB(QStatus status, ajn::ProxyBusObject* obj, void* context)
    {
        /*
         * The IntrospectCallbackContext that found in the context pointer is
         * allocated in when the user calls alljoyn_proxybusobject_introspectremoteobjectasync
         * as soon as the IntrospectCB is received this context pointer will not be
         * used again and must be freed.  we don't call delete here the instance
         * of the IntrospectCallbackContext will cause a memory leak.
         */
        IntrospectCallbackContext* in = (IntrospectCallbackContext*)context;
        in->replyhandler_ptr(status, (alljoyn_proxybusobject)obj, in->context);
        in->replyhandler_ptr = NULL;
        delete in;
    }

    void GetPropertyCB(QStatus status, ProxyBusObject* obj, const MsgArg& value, void* context)
    {
        GetPropertyCallbackContext* in = (GetPropertyCallbackContext*)context;
        in->replyhandler_ptr(status, (alljoyn_proxybusobject)obj, (alljoyn_msgarg)(&value), in->context);
        in->replyhandler_ptr = NULL;
        delete in;
    }

    void GetAllPropertiesCB(QStatus status, ProxyBusObject* obj, const MsgArg& value, void* context)
    {
        GetAllPropertiesCallbackContext* in = (GetAllPropertiesCallbackContext*)context;
        in->replyhandler_ptr(status, (alljoyn_proxybusobject)obj, (alljoyn_msgarg)(&value), in->context);
        in->replyhandler_ptr = NULL;
        delete in;
    }

    void SetPropertyCB(QStatus status, ProxyBusObject* obj, void* context)
    {
        SetPropertyCallbackContext* in = (SetPropertyCallbackContext*)context;
        in->replyhandler_ptr(status, (alljoyn_proxybusobject)obj, in->context);
        in->replyhandler_ptr = NULL;
        delete in;
    }


};
}
#endif
