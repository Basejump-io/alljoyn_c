/******************************************************************************
 * Copyright 2012-2013, Qualcomm Innovation Center, Inc.
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
#include <alljoyn_c/BusAttachment.h>
#include <alljoyn_c/InterfaceDescription.h>
#include <qcc/Thread.h>
#include <qcc/Util.h>
#include "ajTestCommon.h"

TEST(BusAttachmentTest, createinterface) {
    QStatus status = ER_OK;
    alljoyn_busattachment bus = NULL;
    bus = alljoyn_busattachment_create("BusAttachmentTest", QCC_FALSE);
    ASSERT_TRUE(bus != NULL);
    alljoyn_interfacedescription testIntf = NULL;
    status = alljoyn_busattachment_createinterface(bus, "org.alljoyn.test.BusAttachment", &testIntf, QCC_FALSE);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    alljoyn_busattachment_destroy(bus);
}

TEST(BusAttachmentTest, deleteinterface) {
    QStatus status = ER_OK;
    alljoyn_busattachment bus = NULL;
    bus = alljoyn_busattachment_create("BusAttachmentTest", QCC_FALSE);
    ASSERT_TRUE(bus != NULL);
    alljoyn_interfacedescription testIntf = NULL;
    status = alljoyn_busattachment_createinterface(bus, "org.alljoyn.test.BusAttachment", &testIntf, QCC_FALSE);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = alljoyn_busattachment_deleteinterface(bus, testIntf);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    alljoyn_busattachment_destroy(bus);
}

TEST(BusAttachmentTest, start_stop_join) {
    QStatus status = ER_FAIL;
    alljoyn_busattachment bus = NULL;
    bus = alljoyn_busattachment_create("BusAttachmentTest", QCC_FALSE);
    status = alljoyn_busattachment_start(bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = alljoyn_busattachment_stop(bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = alljoyn_busattachment_join(bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    alljoyn_busattachment_destroy(bus);
}

TEST(BusAttachmentTest, isstarted_isstopping) {
    QStatus status = ER_FAIL;
    alljoyn_busattachment bus = NULL;
    bus = alljoyn_busattachment_create("BusAttachmentTest", QCC_FALSE);
    EXPECT_EQ(QCC_FALSE, alljoyn_busattachment_isstarted(bus));
    status = alljoyn_busattachment_start(bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    EXPECT_EQ(QCC_TRUE, alljoyn_busattachment_isstarted(bus));
    status = alljoyn_busattachment_stop(bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    /*
     * Assumption made that the isstopping function will be called before all of
     * the BusAttachement threads have completed so it will return QCC_TRUE it is
     * possible, but unlikely, that this could return QCC_FALSE.
     */

    EXPECT_EQ(QCC_TRUE, alljoyn_busattachment_isstopping(bus));
    status = alljoyn_busattachment_join(bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    EXPECT_EQ(QCC_FALSE, alljoyn_busattachment_isstarted(bus));
    alljoyn_busattachment_destroy(bus);
}

TEST(BusAttachmentTest, getconcurrency) {
    alljoyn_busattachment bus = NULL;
    unsigned int concurrency = -1;
    bus = alljoyn_busattachment_create("BusAttachmentTest", QCC_TRUE);

    concurrency = alljoyn_busattachment_getconcurrency(bus);
    //The default value for getconcurrency is 4
    EXPECT_EQ(4u, concurrency) << "  Expected a concurrency of 4 got " << concurrency;

    alljoyn_busattachment_destroy(bus);

    bus = NULL;
    concurrency = -1;

    bus = alljoyn_busattachment_create_concurrency("BusAttachmentTest", QCC_TRUE, 8);

    concurrency = alljoyn_busattachment_getconcurrency(bus);
    //The default value for getconcurrency is 4
    EXPECT_EQ(8u, concurrency) << "  Expected a concurrency of 8 got " << concurrency;

    alljoyn_busattachment_destroy(bus);
}

TEST(BusAttachmentTest, isconnected)
{
    QStatus status;
    alljoyn_busattachment bus;
    size_t i;

    QCC_BOOL allow_remote[2] = { QCC_FALSE, QCC_TRUE };

    for (i = 0; i < ArraySize(allow_remote); i++) {
        status = ER_FAIL;
        bus = NULL;

        bus = alljoyn_busattachment_create("BusAttachmentTest", allow_remote[i]);

        status = alljoyn_busattachment_start(bus);
        EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
        EXPECT_FALSE(alljoyn_busattachment_isconnected(bus));

        status = alljoyn_busattachment_connect(bus, ajn::getConnectArg().c_str());
        EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
        if (ER_OK == status) {
            EXPECT_TRUE(alljoyn_busattachment_isconnected(bus));
        }

        status = alljoyn_busattachment_disconnect(bus, ajn::getConnectArg().c_str());
        EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
        if (ER_OK == status) {
            EXPECT_FALSE(alljoyn_busattachment_isconnected(bus));
        }

        status = alljoyn_busattachment_stop(bus);
        EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
        status = alljoyn_busattachment_join(bus);
        EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

        alljoyn_busattachment_destroy(bus);
    }
}

TEST(BusAttachmentTest, getconnectspec)
{
    QStatus status = ER_OK;

    alljoyn_busattachment bus = NULL;
    bus = alljoyn_busattachment_create("BusAttachmentTest", QCC_TRUE);

    status = alljoyn_busattachment_start(bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = alljoyn_busattachment_connect(bus, ajn::getConnectArg().c_str());
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    const char* connectspec = alljoyn_busattachment_getconnectspec(bus);

    /*
     * The BusAttachment has joined either a separate daemon or it is using
     * the in process name service.  If the internal name service is used
     * the connect spec will be 'null:' otherwise it will match the ConnectArg.
     */
    EXPECT_TRUE(strcmp(ajn::getConnectArg().c_str(), connectspec) == 0 ||
                strcmp("null:", connectspec) == 0);

    status = alljoyn_busattachment_stop(bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);
    status = alljoyn_busattachment_join(bus);
    EXPECT_EQ(ER_OK, status) << "  Actual Status: " << QCC_StatusText(status);

    alljoyn_busattachment_destroy(bus);
}
