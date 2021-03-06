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

#include <gtest/gtest.h>
#include "ajTestCommon.h"
#include <alljoyn_c/DBusStdDefines.h>
#include <alljoyn_c/Message.h>
#include <alljoyn_c/BusAttachment.h>
#include <alljoyn_c/ProxyBusObject.h>
#include <qcc/Thread.h>


static alljoyn_busattachment s_bus;

static const char* OBJECT_NAME = "org.alljoyn.test.BusAttachmentTest";

static QCC_BOOL name_owner_changed_flag = QCC_FALSE;
static QCC_BOOL listener_registered_flag = QCC_FALSE;

static void listener_registered(const void* context, alljoyn_busattachment s_bus) {
    listener_registered_flag = QCC_TRUE;
}

static void name_owner_changed_blocking_call(const void* context, const char* busName, const char* previousOwner, const char* newOwner) {
    QStatus status = ER_FAIL;
    alljoyn_proxybusobject proxyObj = alljoyn_proxybusobject_create(s_bus, "org.alljoyn.Bus", "/org/alljoyn/Bus", 0);
    EXPECT_TRUE(proxyObj);
    status = alljoyn_proxybusobject_introspectremoteobject(proxyObj);
    EXPECT_EQ(ER_BUS_BLOCKING_CALL_NOT_ALLOWED, status) << "  Actual Status: " << QCC_StatusText(status);
    EXPECT_NO_FATAL_FAILURE(alljoyn_proxybusobject_destroy(proxyObj));
    name_owner_changed_flag = QCC_TRUE;
}

TEST(ConcurrentCallbackTest, enableconcurrentcallbacks_not_used)
{
    QStatus status = ER_FAIL;
    name_owner_changed_flag = QCC_FALSE;
    listener_registered_flag = QCC_FALSE;

    /* register bus listener */
    alljoyn_buslistener_callbacks buslistenerCbs = {
        &listener_registered,
        NULL,     //listener_unregistered
        NULL,     //found_advertised_name
        NULL,     //lost_advertised_name
        &name_owner_changed_blocking_call, //name_owner_changed
        NULL,     //bus_stopping
        NULL,     //bus_disconnected
        NULL              //prop_changed
    };
    alljoyn_buslistener buslistener = alljoyn_buslistener_create(&buslistenerCbs, NULL);
    s_bus = alljoyn_busattachment_create("BusListenerTest", QCC_FALSE);

    status = alljoyn_busattachment_start(s_bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = alljoyn_busattachment_connect(s_bus, ajn::getConnectArg().c_str());

    alljoyn_busattachment_registerbuslistener(s_bus, buslistener);
    for (size_t i = 0; i < 200; ++i) {
        if (listener_registered_flag) {
            break;
        }
        qcc::Sleep(5);
    }
    EXPECT_TRUE(listener_registered_flag);

    alljoyn_busattachment_requestname(s_bus, OBJECT_NAME, 0);
    for (size_t i = 0; i < 200; ++i) {
        if (name_owner_changed_flag) {
            break;
        }
        qcc::Sleep(5);
    }
    EXPECT_TRUE(name_owner_changed_flag);

    status = alljoyn_busattachment_stop(s_bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = alljoyn_busattachment_join(s_bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    alljoyn_busattachment_destroy(s_bus);
    alljoyn_buslistener_destroy(buslistener);
}

static void name_owner_changed_enableconcurrentcallbacks(const void* context, const char* busName, const char* previousOwner, const char* newOwner) {
    QStatus status = ER_FAIL;
    alljoyn_proxybusobject proxyObj = alljoyn_proxybusobject_create(s_bus, "org.alljoyn.Bus", "/org/alljoyn/Bus", 0);
    EXPECT_TRUE(proxyObj);
    alljoyn_busattachment_enableconcurrentcallbacks(s_bus);
    status = alljoyn_proxybusobject_introspectremoteobject(proxyObj);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    EXPECT_NO_FATAL_FAILURE(alljoyn_proxybusobject_destroy(proxyObj));
    name_owner_changed_flag = QCC_TRUE;
}

TEST(ConcurrentCallbackTest, enableconcurrentcallbacks_used)
{
    QStatus status = ER_FAIL;
    name_owner_changed_flag = QCC_FALSE;
    listener_registered_flag = QCC_FALSE;

    /* register bus listener */
    alljoyn_buslistener_callbacks buslistenerCbs = {
        &listener_registered,
        NULL,     //listener_unregistered
        NULL,     //found_advertised_name
        NULL,     //lost_advertised_name
        &name_owner_changed_enableconcurrentcallbacks, //name_owner_changed
        NULL,     //bus_stopping
        NULL,     //bus_disconnected
        NULL              //prop_changed
    };
    alljoyn_buslistener buslistener = alljoyn_buslistener_create(&buslistenerCbs, NULL);
    s_bus = alljoyn_busattachment_create("BusListenerTest", QCC_FALSE);

    status = alljoyn_busattachment_start(s_bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = alljoyn_busattachment_connect(s_bus, ajn::getConnectArg().c_str());

    alljoyn_busattachment_registerbuslistener(s_bus, buslistener);
    for (size_t i = 0; i < 200; ++i) {
        if (listener_registered_flag) {
            break;
        }
        qcc::Sleep(5);
    }
    EXPECT_TRUE(listener_registered_flag);

    alljoyn_busattachment_requestname(s_bus, OBJECT_NAME, 0);
    for (size_t i = 0; i < 200; ++i) {
        if (name_owner_changed_flag) {
            break;
        }
        qcc::Sleep(5);
    }
    EXPECT_TRUE(name_owner_changed_flag);

    status = alljoyn_busattachment_stop(s_bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = alljoyn_busattachment_join(s_bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    alljoyn_busattachment_destroy(s_bus);
    alljoyn_buslistener_destroy(buslistener);
}
