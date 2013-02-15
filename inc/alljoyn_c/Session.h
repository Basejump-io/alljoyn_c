/**
 * @file
 * AllJoyn session related data types.
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
#ifndef _ALLJOYN_C_SESSION_H
#define _ALLJOYN_C_SESSION_H

#include <qcc/platform.h>
#include <alljoyn_c/AjAPI.h>
#include <alljoyn_c/TransportMask.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * alljoyn_sessionopts contains a set of parameters that define a Session's characteristics.
 */
typedef struct _alljoyn_sessionopts_handle*                 alljoyn_sessionopts;

/**
 * alljoyn_sessionport identifies a per-alljoyn_busattachment receiver for incoming
 * JoinSession requests.
 *
 * alljoyn_sessionport values are bound to an alljoyn_busattachment when the attachment calls
 * alljoyn_busattachment_bindsessionport.
 *
 * NOTE: Valid alljoyn_sessionport values range from 1 to 0xFFFF.
 */
typedef uint16_t alljoyn_sessionport;

/** Invalid SessionPort value used to indicate that BindSessionPort should choose any available port */
const alljoyn_sessionport ALLJOYN_SESSION_PORT_ANY = 0;

/** SessionId uniquely identifies an AllJoyn session instance */
typedef uint32_t alljoyn_sessionid;

/** @name TrafficType */
// {@
#define ALLJOYN_TRAFFIC_TYPE_MESSAGES        0x01   /**< Session carries message traffic */
#define ALLJOYN_TRAFFIC_TYPE_RAW_UNRELIABLE  0x02   /**< Session carries an unreliable (lossy) byte stream */
#define ALLJOYN_TRAFFIC_TYPE_RAW_RELIABLE    0x04   /**< Session carries a reliable byte stream */
// @}

/** @name Proximity */
// {@
#define ALLJOYN_PROXIMITY_ANY       0xFF /**< Accept any proximity options */
#define ALLJOYN_PROXIMITY_PHYSICAL  0x01 /**< Limit the session to the same physical device */
#define ALLJOYN_PROXIMITY_NETWORK   0x02 /**< Limit the session to network proximity */
// @}

/**
 * Construct an alljoyn_sessionopts with specific parameters.
 *
 * @param traffic       Type of traffic.
 * @param isMultipoint  true iff session supports multipoint (greater than two endpoints).
 * @param proximity     Proximity constraint bitmask.
 * @param transports    Allowed transport types bitmask.
 *
 * @return the allocated alljoyn_sessionopts
 */
extern AJ_API alljoyn_sessionopts alljoyn_sessionopts_create(uint8_t traffic, QCC_BOOL isMultipoint,
                                                             uint8_t proximity, alljoyn_transportmask transports);

/**
 * Destroy an alljoyn_sessionopts created with alljoyn_sessionopts_create.
 *
 * @param opts alljoyn_sessionopts to destroy
 */
extern AJ_API void alljoyn_sessionopts_destroy(alljoyn_sessionopts opts);

/**
 * Accessor for the traffic member of alljoyn_sessionopts.
 *
 * @param opts alljoyn_sessionopts
 *
 * @return Traffic type specified by the specified alljoyn_sessionopts.
 */
extern AJ_API uint8_t alljoyn_sessionopts_traffic(const alljoyn_sessionopts opts);

/**
 * Accessor for the isMultipoint member of alljoyn_sessionopts.
 *
 * @param opts alljoyn_sessionopts
 *
 * @return Multipoint value specified by the specified alljoyn_sessionopts.
 */
extern AJ_API QCC_BOOL alljoyn_sessionopts_multipoint(const alljoyn_sessionopts opts);

/**
 * Accessor for the proximity member of alljoyn_sessionopts.
 *
 * @param opts alljoyn_sessionopts
 *
 * @return Proximity specified by the specified alljoyn_sessionopts.
 */
extern AJ_API uint8_t alljoyn_sessionopts_proximity(const alljoyn_sessionopts opts);

/**
 * Accessor for the transports member of alljoyn_sessionopts.
 *
 * @param opts alljoyn_sessionopts
 *
 * @return Transports allowed by the specified alljoyn_sessionopts.
 */
extern AJ_API alljoyn_transportmask alljoyn_sessionopts_transports(const alljoyn_sessionopts opts);

/**
 * Determine whether one alljoyn_sessionopts is compatible with the alljoyn_sessionopts
 * offered by other
 *
 * @param one    Options to be compared against other.
 * @param other  Options to be compared against one.
 * @return QCC_TRUE iff this alljoyn_sessionopts can use the option set offered by other.
 */
extern AJ_API QCC_BOOL alljoyn_sessionopts_iscompatible(const alljoyn_sessionopts one, const alljoyn_sessionopts other);

/**
 * Compare two alljoyn_sessionopts.
 *
 * @param one    Options to be compared against other.
 * @param other  Options to be compared against one.
 * @return 0 if the alljoyn_sessionopts are equal, 1 if one > other, -1 if one < other.
 */
extern AJ_API int32_t alljoyn_sessionopts_cmp(const alljoyn_sessionopts one, const alljoyn_sessionopts other);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
