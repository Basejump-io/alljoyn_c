/**
 * @file
 * Class for encapsulating Session option information.
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

#include <alljoyn/MsgArg.h>
#include <alljoyn/Session.h>
#include <alljoyn_c/MsgArg.h>
#include <alljoyn_c/Session.h>

#define QCC_MODULE "ALLJOYN"

struct _alljoyn_sessionopts_handle {
    /* Empty by design, this is just to allow the type restrictions to save coders from themselves */
};

alljoyn_sessionopts alljoyn_sessionopts_create(uint8_t traffic, QCC_BOOL isMultipoint,
                                               uint8_t proximity, alljoyn_transportmask transports)
{
    return (alljoyn_sessionopts) new ajn::SessionOpts((ajn::SessionOpts::TrafficType)traffic, isMultipoint == QCC_TRUE ? true : false,
                                                      (ajn::SessionOpts::Proximity)proximity, (ajn::TransportMask)transports);
}

void alljoyn_sessionopts_destroy(alljoyn_sessionopts opts)
{
    delete (ajn::SessionOpts*)opts;
}

uint8_t alljoyn_sessionopts_get_traffic(const alljoyn_sessionopts opts)
{
    return ((const ajn::SessionOpts*)opts)->traffic;
}

void alljoyn_sessionopts_set_traffic(alljoyn_sessionopts opts, uint8_t traffic)
{
    ((ajn::SessionOpts*)opts)->traffic = (ajn::SessionOpts::TrafficType)traffic;
}

QCC_BOOL alljoyn_sessionopts_get_multipoint(const alljoyn_sessionopts opts)
{
    return (((const ajn::SessionOpts*)opts)->isMultipoint ? QCC_TRUE : QCC_FALSE);
}

void alljoyn_sessionopts_set_multipoint(alljoyn_sessionopts opts, QCC_BOOL isMultipoint)
{
    (isMultipoint == QCC_TRUE) ? ((ajn::SessionOpts*)opts)->isMultipoint = true :
                                                                           ((ajn::SessionOpts*)opts)->isMultipoint = false;
}

uint8_t alljoyn_sessionopts_get_proximity(const alljoyn_sessionopts opts)
{
    return ((const ajn::SessionOpts*)opts)->proximity;
}

void alljoyn_sessionopts_set_proximity(alljoyn_sessionopts opts, uint8_t proximity)
{
    ((ajn::SessionOpts*)opts)->proximity = (ajn::SessionOpts::Proximity)proximity;
}

alljoyn_transportmask alljoyn_sessionopts_get_transports(const alljoyn_sessionopts opts)
{
    return ((const ajn::SessionOpts*)opts)->transports;
}

void alljoyn_sessionopts_set_transports(alljoyn_sessionopts opts, alljoyn_transportmask transports)
{
    ((ajn::SessionOpts*)opts)->transports = (ajn::TransportMask)transports;
}

QCC_BOOL alljoyn_sessionopts_iscompatible(const alljoyn_sessionopts one, const alljoyn_sessionopts other)
{
    return (((const ajn::SessionOpts*)one)->IsCompatible(*((const ajn::SessionOpts*)other)) == true ? QCC_TRUE : QCC_FALSE);
}

int32_t alljoyn_sessionopts_cmp(const alljoyn_sessionopts one, const alljoyn_sessionopts other)
{
    const ajn::SessionOpts& _one = *((const ajn::SessionOpts*)one);
    const ajn::SessionOpts& _other = *((const ajn::SessionOpts*)other);

    return (_one == _other ? 0 : (_other < _one ? 1 : -1));
}
