/**
 * @file
 * Transport type definitions
 */

/******************************************************************************
 * Copyright 2009-2010, Qualcomm Innovation Center, Inc.
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
#ifndef _ALLJOYN_C_TRANPORTMASK_H
#define _ALLJOYN_C_TRANPORTMASK_H

#include <qcc/platform.h>
#include <alljoyn_c/AjAPI.h>

/** Bitmask of all transport types */
typedef uint16_t alljoyn_transportmask;

const alljoyn_transportmask ALLJOYN_TRANSPORT_NONE      = 0x0000;   /**< no transports */
const alljoyn_transportmask ALLJOYN_TRANSPORT_ANY       = 0xFFFF;   /**< ANY transport */
const alljoyn_transportmask ALLJOYN_TRANSPORT_LOCAL     = 0x0001;   /**< Local (same device) transport */
const alljoyn_transportmask ALLJOYN_TRANSPORT_BLUETOOTH = 0x0002;   /**< Bluetooth transport */
const alljoyn_transportmask ALLJOYN_TRANSPORT_TCP       = 0x0004;   /**< Transport using TCP (same as TRANSPORT_WLAN) */
const alljoyn_transportmask ALLJOYN_TRANSPORT_WLAN      = 0x0004;   /**< Wireless local-area network transport */
const alljoyn_transportmask ALLJOYN_TRANSPORT_WWAN      = 0x0008;   /**< Wireless wide-area network transport */
const alljoyn_transportmask ALLJOYN_TRANSPORT_LAN       = 0x0010;   /**< Wired local-area network transport */
const alljoyn_transportmask ALLJOYN_TRANSPORT_ICE       = 0x0020;   /**< Transport using ICE protocol */
const alljoyn_transportmask ALLJOYN_TRANSPORT_PROXIMITY = 0x0040;   /**< Transport using WinRT Proximity Framework */
const alljoyn_transportmask ALLJOYN_TRANSPORT_WFD       = 0x0080;   /**< Transport using Wi-Fi Direct transport */

#endif
