/**
 * @file
 * The alljoyn_keystorelistener handles requests to load or store the key store.
 */
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
#ifndef _ALLJOYN_C_KEYSTORE_LISTENER_H
#define _ALLJOYN_C_KEYSTORE_LISTENER_H

#include <alljoyn_c/AjAPI.h>
#include <Status.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The alljoyn_keystore manages storing and loading of key blobs from
 * external storage.
 */
typedef struct _alljoyn_keystore_handle*                    alljoyn_keystore;

/**
 * An application can provide a key store listener to override the default key store
 * load request and store request behavior.  If an alljoyn_keystorelistener is provided it will
 * override the default key store behavior.
 */
typedef struct _alljoyn_keystorelistener_handle*            alljoyn_keystorelistener;

/**
 * Type for the LoadRequest callback.
 *
 * This function is called when a key store needs to be loaded.
 * @remark The application must call <tt>#alljoyn_keystorelistener_putkeys</tt> to put the new key store data into the
 * internal key store.
 *
 * @param context    the context pointer passed into the alljoyn_keystorelistener_create function
 * @param keyStore   Reference to the alljoyn_keystore to be loaded.
 *
 * @return
 *      - #ER_OK if the load request was satisfied
 *      - An error status otherwise
 *
 */
typedef QStatus (*alljoyn_keystorelistener_loadrequest_ptr)(const void* context, alljoyn_keystore keyStore);

/**
 * Type for the StoreRequest callback.
 *
 * This function is called when a key store needs to be stored.
 * @remark The application must call <tt>#alljoyn_keystorelistener_getkeys</tt> to obtain the key data to be stored.
 *
 * @param context    the context pointer passed into the alljoyn_keystorelistener_create function
 * @param keyStore   Reference to the alljoyn_keystore to be stored.
 *
 * @return
 *      - #ER_OK if the store request was satisfied
 *      - An error status otherwise
 */
typedef QStatus (*alljoyn_keystorelistener_storerequest_ptr)(const void* context, alljoyn_keystore keyStore);

/**
 * Structure used during alljoyn_keystorelistener_create to provide callbacks into C.
 */
typedef struct {
    /**
     * This function is called when a key store needs to be loaded.
     */
    alljoyn_keystorelistener_loadrequest_ptr load_request;
    /**
     * This function is called when a key store needs to be stored.
     */
    alljoyn_keystorelistener_storerequest_ptr store_request;
} alljoyn_keystorelistener_callbacks;

/**
 * Create an alljoyn_keystorelistener
 *
 * @param callbacks  Callbacks to trigger for associated events.
 * @param context    Context to pass along to callback functions.
 *
 * @return an allocated alljoyn_keystorelistener
 */
extern AJ_API alljoyn_keystorelistener alljoyn_keystorelistener_create(const alljoyn_keystorelistener_callbacks* callbacks,
                                                                       const void* context);

/**
 * Destroy an alljoyn_keystorelistener
 *
 * @param listener The alljoyn_keystorelistener to destroy.
 */
extern AJ_API void alljoyn_keystorelistener_destroy(alljoyn_keystorelistener listener);

/**
 * Put keys into the key store from an encrypted byte string.
 *
 * @param listener  The alljoyn_keystorelistener into which to put the keys.
 * @param keyStore  The keyStore to put to. This is the keystore indicated in the LoadRequest call.
 * @param source    The byte string containing the encrypted key store contents.
 * @param password  The password required to decrypt the key data
 *
 * @return
 *      - #ER_OK if successful
 *      - An error status otherwise
 *
 */
extern AJ_API QStatus alljoyn_keystorelistener_putkeys(alljoyn_keystorelistener listener, alljoyn_keystore keyStore,
                                                       const char* source, const char* password);

/**
 * Get the current keys from the key store as an encrypted byte string.
 *
 * @param listener  The alljoyn_keystorelistener from which to get the keys.
 * @param keyStore  The keyStore to get from. This is the keystore indicated in the StoreRequest call.
 * @param sink      The byte string to write the keys to.
 * @param sink_sz   The size of the byte string provided.
 * @return
 *      - #ER_OK if successful
 *      - An error status otherwise
 */
extern AJ_API QStatus alljoyn_keystorelistener_getkeys(alljoyn_keystorelistener listener, alljoyn_keystore keyStore,
                                                       char* sink, size_t sink_sz);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
