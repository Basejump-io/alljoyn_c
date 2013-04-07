/**
 * @file PasswordManager.h is the top-level object that provides the interface to
 * set credentials used for the authentication of thin clients.
 */

/******************************************************************************
 * Copyright 2013, Qualcomm Innovation Center, Inc.
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
#ifndef _ALLJOYN_C_PASSWORDMANAGER_H
#define _ALLJOYN_C_PASSWORDMANAGER_H

#include <qcc/platform.h>

#include <alljoyn_c/AjAPI.h>
#include <Status.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Set credentials used for the authentication of thin clients.
 *
 * @warning Before invoking Connect() to BusAttachment, the application should call SetCredentials
 * if it expects to be able to communicate to/from thin clients.
 * The bundled daemon will start advertising the name as soon as it is started and MUST have
 * the credentials set to be able to authenticate any thin clients that may try to use the
 * bundled daemon to communicate with the app.
 * @param authMechanism  The name of the authentication mechanism issuing the request.
 * @param password       The password.
 *
 * @return
 *      - #ER_OK  if the credentials was successfully set.
 */
extern AJ_API QStatus alljoyn_passwordmanager_setcredentials(const char* authMechanism, const char* password);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

