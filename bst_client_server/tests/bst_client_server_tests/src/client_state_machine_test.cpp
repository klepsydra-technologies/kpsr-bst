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

#include <klepsydra/bst_client_server/client_state_machine.h>

#include "bst_client_test_middleware_provider.h"

TEST(ClientStateMachineTest, BaseTest)
{
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr,
                                                                                     "test",
                                                                                     0,
                                                                                     nullptr,
                                                                                     nullptr);
    kpsr::mem::CacheListener<std::string> stateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()
        ->registerListener("test", stateMachineListener.cacheListenerFunction);

    kpsr::EventEmitterMiddlewareProvider<std::string>
        stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::bst::ClientStateMachine clientStateMachine(&environment,
                                                     &bstClientMiddlewareProvider,
                                                     stateMachineMiddlewareProvider.getPublisher(),
                                                     stateMachineExtEventMiddlewareProvider
                                                         .getSubscriber(),
                                                     &telemetrySystemEventListener);

    ASSERT_EQ(stateMachineListener.counter, 0);

    clientStateMachine.start();
    ASSERT_EQ(stateMachineListener.counter, 1);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:idle");

    bst::comms::TelemetrySystem_t telemetrySystem;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 2);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 3);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlReadyReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_PAYLOAD_CONTROL).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 4);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_PREFLIGHT;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 5);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlReady");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("takeoffReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 6);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:preFlightReq");

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 7);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:launchModeReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_FLIGHT_MODE).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 8);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:launchModeReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_LAUNCH;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 9);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:enableEngineReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_ENGINE_KILL).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 10);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:launchReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_LAUNCH).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 11);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:launchReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_FLYING;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 12);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:flying");

    {
        kpsr::bst::WaypointCommandMessage message;
        message.id = 100;

        clientStateMachine.sendWaypoints(message);
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 13);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:flightPlanReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(100).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 14);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:flying");

    {
        kpsr::bst::BstRequestMessageBuilder messageBuilder;
        messageBuilder.withId(CONTROL_COMMAND).withType(CMD_WAYPOINT);

        clientStateMachine.sendCommand(*messageBuilder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 15);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:controlCommandReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_WAYPOINT).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 16);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:flying");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("landReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 17);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:landReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_LAND).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 18);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:landReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_LANDING;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 19);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:landed");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlOffReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 20);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlOffReq");
    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND).withType(CMD_PAYLOAD_CONTROL).withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
            builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 21);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    clientStateMachine.stop();
}

TEST(ClientStateMachineTest, JumpPreflighTest)
{
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr,
                                                                                     "test",
                                                                                     0,
                                                                                     nullptr,
                                                                                     nullptr);
    kpsr::mem::CacheListener<std::string> stateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()
        ->registerListener("test", stateMachineListener.cacheListenerFunction);

    kpsr::EventEmitterMiddlewareProvider<std::string>
        stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::bst::ClientStateMachine clientStateMachine(&environment,
                                                     &bstClientMiddlewareProvider,
                                                     stateMachineMiddlewareProvider.getPublisher(),
                                                     stateMachineExtEventMiddlewareProvider
                                                         .getSubscriber(),
                                                     &telemetrySystemEventListener);

    ASSERT_EQ(stateMachineListener.counter, 0);

    clientStateMachine.start();
    ASSERT_EQ(stateMachineListener.counter, 1);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:idle");

    bst::comms::TelemetrySystem_t telemetrySystem;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 2);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 3);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlReadyReq");

    kpsr::bst::BstReplyMessageBuilder builder;
    builder.withId(CONTROL_COMMAND).withType(CMD_PAYLOAD_CONTROL).withAck(true);

    bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
        builder.build());

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 4);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_LAUNCH;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 5);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlReady");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("takeoffReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 6);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:preFlightReq");

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 7);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:launchModeReq");

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 8);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:enableEngineReq");

    clientStateMachine.stop();
}

TEST(ClientStateMachineTest, ErrorInFlightModeTest)
{
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr,
                                                                                     "test",
                                                                                     0,
                                                                                     nullptr,
                                                                                     nullptr);
    kpsr::mem::CacheListener<std::string> stateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()
        ->registerListener("test", stateMachineListener.cacheListenerFunction);

    kpsr::EventEmitterMiddlewareProvider<std::string>
        stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::bst::ClientStateMachine clientStateMachine(&environment,
                                                     &bstClientMiddlewareProvider,
                                                     stateMachineMiddlewareProvider.getPublisher(),
                                                     stateMachineExtEventMiddlewareProvider
                                                         .getSubscriber(),
                                                     &telemetrySystemEventListener);

    ASSERT_EQ(stateMachineListener.counter, 0);

    clientStateMachine.start();
    ASSERT_EQ(stateMachineListener.counter, 1);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:idle");

    bst::comms::TelemetrySystem_t telemetrySystem;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 2);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 3);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlReadyReq");

    kpsr::bst::BstReplyMessageBuilder builder;
    builder.withId(CONTROL_COMMAND).withType(CMD_PAYLOAD_CONTROL).withAck(true);

    bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
        builder.build());

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 4);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_TERMINATE;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 5);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:error");

    clientStateMachine.stop();
}

TEST(ClientStateMachineTest, NackOnPayLoadControlTest)
{
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr,
                                                                                     "test",
                                                                                     0,
                                                                                     nullptr,
                                                                                     nullptr);
    kpsr::mem::CacheListener<std::string> stateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()
        ->registerListener("test", stateMachineListener.cacheListenerFunction);

    kpsr::EventEmitterMiddlewareProvider<std::string>
        stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::bst::ClientStateMachine clientStateMachine(&environment,
                                                     &bstClientMiddlewareProvider,
                                                     stateMachineMiddlewareProvider.getPublisher(),
                                                     stateMachineExtEventMiddlewareProvider
                                                         .getSubscriber(),
                                                     &telemetrySystemEventListener);

    ASSERT_EQ(stateMachineListener.counter, 0);

    clientStateMachine.start();
    ASSERT_EQ(stateMachineListener.counter, 1);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:idle");

    bst::comms::TelemetrySystem_t telemetrySystem;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 2);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 3);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(),
              "bstClientStateMachine:payloadControlReadyReq");

    kpsr::bst::BstReplyMessageBuilder builder;
    builder.withId(CONTROL_COMMAND).withType(CMD_PAYLOAD_CONTROL).withAck(false);

    bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(
        builder.build());

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 4);
    ASSERT_EQ(*stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:error");

    clientStateMachine.stop();
}
