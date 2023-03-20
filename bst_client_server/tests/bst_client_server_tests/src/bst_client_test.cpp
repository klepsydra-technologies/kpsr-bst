// Copyright 2023 Klepsydra Technologies AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <math.h>
#include <stdio.h>
#include <thread>
#include <unistd.h>

#include <fstream>
#include <sstream>

#include "gtest/gtest.h"

#include <klepsydra/core/cache_listener.h>

#include <klepsydra/mem_core/mem_env.h>

#include <klepsydra/bst_client_server/bst_client.h>

#include "bst_client_test_middleware_provider.h"

TEST(BstClientTest, BaseTest)
{
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_heartbeat_period_microsecs", 0);
    environment.setPropertyInt("bst_client_run_period_microsecs", 0);
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr,
                                                                                     "test",
                                                                                     0,
                                                                                     nullptr,
                                                                                     nullptr);
    kpsr::EventEmitterMiddlewareProvider<std::string>
        stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);

    kpsr::mem::CacheListener<std::string> testStateMachineListener;
    kpsr::mem::CacheListener<std::string> bstClientStateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()
        ->registerListener("test", testStateMachineListener.cacheListenerFunction);

    kpsr::bst::BstClient bstClient(nullptr,
                                   &environment,
                                   stateMachineExtEventMiddlewareProvider.getSubscriber(),
                                   stateMachineExtEventMiddlewareProvider.getPublisher(),
                                   stateMachineMiddlewareProvider.getSubscriber(),
                                   stateMachineMiddlewareProvider.getPublisher(),
                                   bstClientStateMachineListener,
                                   &bstClientMiddlewareProvider,
                                   &telemetrySystemEventListener);

    ASSERT_EQ(testStateMachineListener.counter, 0);

    bstClient.start();
    ASSERT_EQ(testStateMachineListener.counter, 1);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:idle");

    bst::comms::TelemetrySystem_t telemetrySystem;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 2);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:ready");

    {
        bool validAction = bstClient.acquirePayloadControl();
        ASSERT_TRUE(validAction);
        bstClient.execute();
    }

    ASSERT_EQ(testStateMachineListener.counter, 3);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:payloadControlReadyReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_PAYLOAD_CONTROL).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 4);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:payloadControlReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_PREFLIGHT;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 5);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:payloadControlReady");

    {
        bool validAction = bstClient.takeoff();
        ASSERT_TRUE(validAction);
        bstClient.execute();
    }

    ASSERT_EQ(testStateMachineListener.counter, 6);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:preFlightReq");

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 7);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:launchModeReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_FLIGHT_MODE).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }
    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 8);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:launchModeReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_LAUNCH;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 9);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:enableEngineReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_ENGINE_KILL).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 10);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:launchReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_LAUNCH).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 11);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:launchReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_FLYING;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 12);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:flying");

    {
        bool validAction = bstClient.land();
        ASSERT_TRUE(validAction);
        bstClient.execute();
    }

    ASSERT_EQ(testStateMachineListener.counter, 13);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:landReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_LAND).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 14);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:landReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_LANDING;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 15);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:landed");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlOffReqRx");
    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 16);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:payloadControlOffReq");
    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_PAYLOAD_CONTROL).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 17);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:ready");
}

TEST(BstClientTest, InvalidActions)
{
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_heartbeat_period_microsecs", 0);
    environment.setPropertyInt("bst_client_run_period_microsecs", 0);
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr,
                                                                                     "test",
                                                                                     0,
                                                                                     nullptr,
                                                                                     nullptr);
    kpsr::EventEmitterMiddlewareProvider<std::string>
        stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);

    kpsr::mem::CacheListener<std::string> testStateMachineListener;
    kpsr::mem::CacheListener<std::string> bstClientStateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()
        ->registerListener("test", testStateMachineListener.cacheListenerFunction);

    kpsr::bst::BstClient bstClient(nullptr,
                                   &environment,
                                   stateMachineExtEventMiddlewareProvider.getSubscriber(),
                                   stateMachineExtEventMiddlewareProvider.getPublisher(),
                                   stateMachineMiddlewareProvider.getSubscriber(),
                                   stateMachineMiddlewareProvider.getPublisher(),
                                   bstClientStateMachineListener,
                                   &bstClientMiddlewareProvider,
                                   &telemetrySystemEventListener);

    ASSERT_EQ(testStateMachineListener.counter, 0);

    bstClient.start();
    ASSERT_EQ(testStateMachineListener.counter, 1);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:idle");

    {
        bool validAction = bstClient.acquirePayloadControl();
        ASSERT_FALSE(validAction);
    }

    {
        bool validAction = bstClient.takeoff();
        ASSERT_FALSE(validAction);
    }

    {
        bool validAction = bstClient.land();
        ASSERT_FALSE(validAction);
    }
    bstClient.execute();

    ASSERT_EQ(testStateMachineListener.counter, 1);
    ASSERT_EQ(bstClient.getCurrentState(), "bstClientStateMachine:idle");
}
