/**
 * @file
 *
 * This file implements a AuthListener subclass for use by the C API
 */

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

#include <alljoyn/AuthListener.h>
#include <alljoyn_c/AuthListener.h>
#include <assert.h>

using namespace qcc;
using namespace std;

namespace ajn {
/**
 * Abstract base class implemented by AllJoyn users and called by AllJoyn to inform
 * users of bus related events.
 */
class AuthListenerCallbackC : public AuthListener {
  public:
    AuthListenerCallbackC(const alljoyn_authlistener_callbacks* callbacks_in, const void* context_in)
    {
        memcpy(&callbacks, callbacks_in, sizeof(alljoyn_authlistener_callbacks));
        context = context_in;
    }

    virtual bool RequestCredentials(const char* authMechanism, const char* peerName, uint16_t authCount,
                                    const char* userName, uint16_t credMask, Credentials& credentials)
    {
        assert(callbacks.request_credentials != NULL && "request_credentials callback must be specified");
        bool ret = (callbacks.request_credentials(context, authMechanism, peerName, authCount, userName,
                                                  credMask, (alljoyn_credentials)(&credentials)) == QCC_TRUE ? true : false);
        return ret;
    }

    virtual bool VerifyCredentials(const char* authMechanism, const char* peerName, const Credentials& credentials)
    {
        bool ret = AuthListener::VerifyCredentials(authMechanism, peerName, credentials);
        if (callbacks.verify_credentials != NULL) {
            ret = (callbacks.verify_credentials(context, authMechanism, peerName, (alljoyn_credentials)(&credentials)) == QCC_TRUE ? true : false);
        }
        return ret;
    }

    virtual void SecurityViolation(QStatus status, const Message& msg)
    {
        if (callbacks.security_violation != NULL) {
            callbacks.security_violation(context, status, (alljoyn_message)(&msg));
        }
    }

    virtual void AuthenticationComplete(const char* authMechanism, const char* peerName, bool success)
    {
        assert(callbacks.authentication_complete != NULL && "authentication_complete callback must be specified");
        callbacks.authentication_complete(context, authMechanism, peerName, (success == true ? QCC_TRUE : QCC_FALSE));
    }
  private:
    alljoyn_authlistener_callbacks callbacks;
    const void* context;
};

class AuthListenerAsyncCallbackC : public AuthListener {
  public:
    AuthListenerAsyncCallbackC(const alljoyn_authlistenerasync_callbacks* callbacks_in, const void* context_in)
    {
        memcpy(&callbacks, callbacks_in, sizeof(alljoyn_authlistener_callbacks));
        context = context_in;
    }

    virtual QStatus RequestCredentialsAsync(const char* authMechanism, const char* authPeer,
                                            uint16_t authCount, const char* userName, uint16_t credMask,
                                            void* authContext)
    {
        assert(callbacks.request_credentials != NULL && "request_credentials callback must be specified");
        QStatus ret = callbacks.request_credentials(context, (alljoyn_authlistener) this, authMechanism, authPeer, authCount, userName, credMask, authContext);
        return ret;
    }

    virtual QStatus VerifyCredentialsAsync(const char* authMechanism, const char* authPeer,
                                           const Credentials& credentials, void* authContext) {
        QStatus ret = ER_NOT_IMPLEMENTED;
        if (callbacks.verify_credentials != NULL) {
            ret = callbacks.verify_credentials(context, (alljoyn_authlistener) this, authMechanism, authPeer, (alljoyn_credentials)(&credentials), authContext);
        }
        return ret;
    }

    virtual void SecurityViolation(QStatus status, const Message& msg)
    {
        if (callbacks.security_violation != NULL) {
            callbacks.security_violation(context, status, (alljoyn_message)(&msg));
        }
    }

    virtual void AuthenticationComplete(const char* authMechanism, const char* peerName, bool success)
    {
        assert(callbacks.authentication_complete != NULL && "authentication_complete callback must be specified");
        callbacks.authentication_complete(context, authMechanism, peerName, (success == true ? QCC_TRUE : QCC_FALSE));
    }
  private:
    alljoyn_authlistenerasync_callbacks callbacks;
    const void* context;
};

}

struct _alljoyn_authlistener_handle {
    /* Empty by design, this is just to allow the type restrictions to save coders from themselves */
};

alljoyn_authlistener alljoyn_authlistener_create(const alljoyn_authlistener_callbacks* callbacks, const void* context)
{
    return (alljoyn_authlistener) new ajn::AuthListenerCallbackC(callbacks, context);
}

void alljoyn_authlistener_destroy(alljoyn_authlistener listener)
{
    assert(listener != NULL && "listener parameter must not be NULL");
    delete (ajn::AuthListenerCallbackC*)listener;
}

alljoyn_authlistener alljoyn_authlistenerasync_create(const alljoyn_authlistenerasync_callbacks* callbacks, const void* context)
{
    return (alljoyn_authlistener) new ajn::AuthListenerAsyncCallbackC(callbacks, context);
}

void alljoyn_authlistenerasync_destroy(alljoyn_authlistener listener)
{
    assert(listener != NULL && "listener parameter must not be NULL");
    delete (ajn::AuthListenerAsyncCallbackC*)listener;
}

extern AJ_API QStatus alljoyn_authlistener_requestcredentialsresponse(alljoyn_authlistener listener, void* authContext, QCC_BOOL accept, alljoyn_credentials credentials)
{
    return ((ajn::AuthListenerAsyncCallbackC*)listener)->RequestCredentialsResponse(authContext, (accept == QCC_TRUE) ? true : false, *((ajn::AuthListener::Credentials*)credentials));
}

QStatus alljoyn_authlistener_verifycredentialsresponse(alljoyn_authlistener listener, void* authContext, QCC_BOOL accept)
{
    return ((ajn::AuthListenerAsyncCallbackC*)listener)->VerifyCredentialsResponse(authContext, (accept == QCC_TRUE) ? true : false);
}
struct _alljoyn_credentials_handle {
    /* Empty by design, this is just to allow the type restrictions to save coders from themselves */
};

alljoyn_credentials alljoyn_credentials_create()
{
    return (alljoyn_credentials) new ajn::AuthListener::Credentials();
}

void alljoyn_credentials_destroy(alljoyn_credentials cred)
{
    assert(cred != NULL && "cred parameter must not be NULL");
    delete (ajn::AuthListener::Credentials*)cred;
}

QCC_BOOL alljoyn_credentials_isset(const alljoyn_credentials cred, uint16_t creds)
{
    return ((ajn::AuthListener::Credentials*)cred)->IsSet(creds) == true ? QCC_TRUE : QCC_FALSE;
}

void alljoyn_credentials_setpassword(alljoyn_credentials cred, const char* pwd)
{
    ((ajn::AuthListener::Credentials*)cred)->SetPassword(pwd);
}

void alljoyn_credentials_setusername(alljoyn_credentials cred, const char* userName)
{
    ((ajn::AuthListener::Credentials*)cred)->SetUserName(userName);
}

void alljoyn_credentials_setcertchain(alljoyn_credentials cred, const char* certChain)
{
    ((ajn::AuthListener::Credentials*)cred)->SetCertChain(certChain);
}

void alljoyn_credentials_setprivatekey(alljoyn_credentials cred, const char* pk)
{
    ((ajn::AuthListener::Credentials*)cred)->SetPrivateKey(pk);
}

void alljoyn_credentials_setlogonentry(alljoyn_credentials cred, const char* logonEntry)
{
    ((ajn::AuthListener::Credentials*)cred)->SetLogonEntry(logonEntry);
}

void alljoyn_credentials_setexpiration(alljoyn_credentials cred, uint32_t expiration)
{
    ((ajn::AuthListener::Credentials*)cred)->SetExpiration(expiration);
}

const char* alljoyn_credentials_getpassword(const alljoyn_credentials cred)
{
    return ((ajn::AuthListener::Credentials*)cred)->GetPassword().c_str();
}

const char* alljoyn_credentials_getusername(const alljoyn_credentials cred)
{
    return ((ajn::AuthListener::Credentials*)cred)->GetUserName().c_str();
}

const char* alljoyn_credentials_getcertchain(const alljoyn_credentials cred)
{
    return ((ajn::AuthListener::Credentials*)cred)->GetCertChain().c_str();
}

const char* alljoyn_credentials_getprivateKey(const alljoyn_credentials cred)
{
    return ((ajn::AuthListener::Credentials*)cred)->GetPrivateKey().c_str();
}

const char* alljoyn_credentials_getlogonentry(const alljoyn_credentials cred)
{
    return ((ajn::AuthListener::Credentials*)cred)->GetLogonEntry().c_str();
}

uint32_t alljoyn_credentials_getexpiration(const alljoyn_credentials cred)
{
    return ((ajn::AuthListener::Credentials*)cred)->GetExpiration();
}

void alljoyn_credentials_clear(alljoyn_credentials cred)
{
    ((ajn::AuthListener::Credentials*)cred)->Clear();
}
