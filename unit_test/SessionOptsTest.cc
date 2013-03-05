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

#include <alljoyn_c/Session.h>
#include <alljoyn_c/TransportMask.h>

TEST(SessionOptsTest, accessor_functions) {
    alljoyn_sessionopts opts = alljoyn_sessionopts_create(ALLJOYN_TRAFFIC_TYPE_MESSAGES,
                                                          QCC_FALSE,
                                                          ALLJOYN_PROXIMITY_ANY,
                                                          ALLJOYN_TRANSPORT_ANY);
    EXPECT_EQ(ALLJOYN_TRAFFIC_TYPE_MESSAGES, alljoyn_sessionopts_get_traffic(opts));
    EXPECT_EQ(QCC_FALSE, alljoyn_sessionopts_get_multipoint(opts));
    EXPECT_EQ(ALLJOYN_PROXIMITY_ANY, alljoyn_sessionopts_get_proximity(opts));
    EXPECT_EQ(ALLJOYN_TRANSPORT_ANY, alljoyn_sessionopts_get_transports(opts));

    alljoyn_sessionopts_set_traffic(opts, ALLJOYN_TRAFFIC_TYPE_RAW_RELIABLE);
    EXPECT_EQ(ALLJOYN_TRAFFIC_TYPE_RAW_RELIABLE, alljoyn_sessionopts_get_traffic(opts));

    alljoyn_sessionopts_set_multipoint(opts, QCC_TRUE);
    EXPECT_EQ(QCC_TRUE, alljoyn_sessionopts_get_multipoint(opts));

    alljoyn_sessionopts_set_proximity(opts, ALLJOYN_PROXIMITY_PHYSICAL);
    EXPECT_EQ(ALLJOYN_PROXIMITY_PHYSICAL, alljoyn_sessionopts_get_proximity(opts));

    alljoyn_sessionopts_set_transports(opts, ALLJOYN_TRANSPORT_LAN);
    EXPECT_EQ(ALLJOYN_TRANSPORT_LAN, alljoyn_sessionopts_get_transports(opts));

    alljoyn_sessionopts_destroy(opts);
}
