/**
 * @file
 * alljoyn_sessionlistener is an collection of callback functions implemented by
 * users of the AllJoyn API in order to receive sessions related event information.
 */

/******************************************************************************
 * Copyright 2009-2013, Qualcomm Innovation Center, Inc.
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
#ifndef _ALLJOYN_C_SESSIONLISTENER_H
#define _ALLJOYN_C_SESSIONLISTENER_H

#include <alljoyn_c/AjAPI.h>
#include <alljoyn_c/Session.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * alljoyn_sessionlistener contains callback functions implemented by AllJoyn users
 * and called by AllJoyn to inform users of session related events.
 */
typedef struct _alljoyn_sessionlistener_handle*             alljoyn_sessionlistener;

/**
 * Type for the SesionLost callback.
 *
 * Called by the bus when an existing session becomes disconnected.
 *
 * @param context       context pointer that was passed into the alljoyn_sessionlistener_create function
 * @param alljoyn_sessionid     Id of session that was lost.
 */
typedef void (*alljoyn_sessionlistener_sessionlost_ptr)(const void* context, alljoyn_sessionid sessionId);

/**
 * Type for the SessionMemberAdded callback.
 *
 * Called by the bus when a member of a multipoint session is added.
 *
 * @param context       context pointer that was passed into the alljoyn_sessionlistener_create function
 * @param alljoyn_sessionid     Id of session whose member(s) changed.
 * @param uniqueName    Unique name of member who was added.
 */
typedef void (*alljoyn_sessionlistener_sessionmemberadded_ptr)(const void* context, alljoyn_sessionid sessionId,
                                                               const char* uniqueName);

/**
 * Type for the SessionMemberRemoved callback.
 *
 * Called by the bus when a member of a multipoint session is removed.
 *
 * @param context       context pointer that was passed into the alljoyn_sessionlistener_create function
 * @param alljoyn_sessionid     Id of session whose member(s) changed.
 * @param uniqueName    Unique name of member who was removed.
 */
typedef void (*alljoyn_sessionlistener_sessionmemberremoved_ptr)(const void* context, alljoyn_sessionid sessionId,
                                                                 const char* uniqueName);

/**
 * Structure used during alljoyn_sessionlistener_create to provide callbacks into C.
 */
typedef struct {
    /**
     * Called by the bus when an existing session becomes disconnected.
     */
    alljoyn_sessionlistener_sessionlost_ptr session_lost;
    /**
     * Called by the bus when a member of a multipoint session is added.
     */
    alljoyn_sessionlistener_sessionmemberadded_ptr session_member_added;
    /**
     * Called by the bus when a member of a multipoint session is removed.
     */
    alljoyn_sessionlistener_sessionmemberremoved_ptr session_member_removed;
} alljoyn_sessionlistener_callbacks;

/**
 * Create an alljoyn_sessionlistener which will trigger the provided callbacks,
 * passing along the provided context.
 *
 * @param callbacks Callbacks to trigger for associated events.
 * @param context   Context to pass to callback functions
 *
 * @return Handle to newly allocated alljoyn_sessionlistener.
 */
extern AJ_API alljoyn_sessionlistener alljoyn_sessionlistener_create(const alljoyn_sessionlistener_callbacks* callbacks,
                                                                     const void* context);

/**
 * Destroy an alljoyn_sessionlistener.
 *
 * @param listener alljoyn_sessionlistener to destroy.
 */
extern AJ_API void alljoyn_sessionlistener_destroy(alljoyn_sessionlistener listener);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
