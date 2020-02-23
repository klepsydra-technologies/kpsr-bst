/*****************************************************************************
*
*                           Klepsydra Core Modules
*              Copyright (C) 2019-2020  Klepsydra Technologies GmbH
*                            All Rights Reserved.
*
*  This file is subject to the terms and conditions defined in
*  file 'LICENSE.md', which is part of this source code package.
*
*  NOTICE:  All information contained herein is, and remains the property of Klepsydra
*  Technologies GmbH and its suppliers, if any. The intellectual and technical concepts
*  contained herein are proprietary to Klepsydra Technologies GmbH and its suppliers and
*  may be covered by Swiss and Foreign Patents, patents in process, and are protected by
*  trade secret or copyright law. Dissemination of this information or reproduction of
*  this material is strictly forbidden unless prior written permission is obtained from
*  Klepsydra Technologies GmbH.
*
*****************************************************************************/
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <math.h>

#include <sstream>
#include <fstream>

#include "gtest/gtest.h"

#include <klepsydra/core/cache_listener.h>

#include <klepsydra/mem_core/mem_env.h>

#include <klepsydra/bst_client_server/client_state_machine.h>

#include "bst_client_test_middleware_provider.h"

TEST(ClientStateMachineTest, BaseTest) {
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::mem::CacheListener<std::string> stateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()->registerListener("test", stateMachineListener.cacheListenerFunction);

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::bst::ClientStateMachine clientStateMachine(&environment, &bstClientMiddlewareProvider,
                                                     stateMachineMiddlewareProvider.getPublisher(),
                                                     stateMachineExtEventMiddlewareProvider.getSubscriber(),
                                                     & telemetrySystemEventListener);

    ASSERT_EQ(stateMachineListener.counter, 0);

    clientStateMachine.start();
    ASSERT_EQ(stateMachineListener.counter, 1);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:idle");

    bst::comms::TelemetrySystem_t telemetrySystem;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 2);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 3);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlReadyReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND)
                .withType(CMD_PAYLOAD_CONTROL)
                .withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 4);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_PREFLIGHT;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 5);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlReady");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("takeoffReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 6);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:preFlightReq");

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 7);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:launchModeReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND)
                .withType(CMD_FLIGHT_MODE)
                .withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(builder.build());
    }
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 8);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:launchModeReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_LAUNCH;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 9);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:enableEngineReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND)
                .withType(CMD_ENGINE_KILL)
                .withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 10);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:launchReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND)
                .withType(CMD_LAUNCH)
                .withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 11);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:launchReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_FLYING;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 12);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:flying");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("landReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 13);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:landReq");

    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND)
                .withType(CMD_LAND)
                .withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 14);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:landReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_LANDING;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 15);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:landed");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlOffReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 16);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlOffReq");
    {
        kpsr::bst::BstReplyMessageBuilder builder;
        builder.withId(CONTROL_COMMAND)
                .withType(CMD_PAYLOAD_CONTROL)
                .withAck(true);

        bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(builder.build());
    }

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 17);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    clientStateMachine.stop();
}

TEST(ClientStateMachineTest, JumpPreflighTest) {
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::mem::CacheListener<std::string> stateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()->registerListener("test", stateMachineListener.cacheListenerFunction);

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::bst::ClientStateMachine clientStateMachine(&environment, &bstClientMiddlewareProvider,
                                                     stateMachineMiddlewareProvider.getPublisher(),
                                                     stateMachineExtEventMiddlewareProvider.getSubscriber(),
                                                     & telemetrySystemEventListener);

    ASSERT_EQ(stateMachineListener.counter, 0);

    clientStateMachine.start();
    ASSERT_EQ(stateMachineListener.counter, 1);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:idle");

    bst::comms::TelemetrySystem_t telemetrySystem;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 2);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 3);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlReadyReq");

    kpsr::bst::BstReplyMessageBuilder builder;
    builder.withId(CONTROL_COMMAND)
            .withType(CMD_PAYLOAD_CONTROL)
            .withAck(true);

    bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(builder.build());

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 4);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_LAUNCH;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 5);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlReady");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("takeoffReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 6);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:preFlightReq");

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 7);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:launchModeReq");

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 8);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:enableEngineReq");

    clientStateMachine.stop();
}

TEST(ClientStateMachineTest, ErrorInFlightModeTest) {
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::mem::CacheListener<std::string> stateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()->registerListener("test", stateMachineListener.cacheListenerFunction);

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::bst::ClientStateMachine clientStateMachine(&environment, &bstClientMiddlewareProvider,
                                                     stateMachineMiddlewareProvider.getPublisher(),
                                                     stateMachineExtEventMiddlewareProvider.getSubscriber(),
                                                     & telemetrySystemEventListener);

    ASSERT_EQ(stateMachineListener.counter, 0);

    clientStateMachine.start();
    ASSERT_EQ(stateMachineListener.counter, 1);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:idle");

    bst::comms::TelemetrySystem_t telemetrySystem;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 2);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 3);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlReadyReq");

    kpsr::bst::BstReplyMessageBuilder builder;
    builder.withId(CONTROL_COMMAND)
            .withType(CMD_PAYLOAD_CONTROL)
            .withAck(true);

    bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(builder.build());

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 4);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlReqAccepted");

    telemetrySystem.flight_mode = FLIGHT_MODE_TERMINATE;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 5);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:error");

    clientStateMachine.stop();
}

TEST(ClientStateMachineTest, NackOnPayLoadControlTest) {
    kpsr::mem::MemEnv environment;
    environment.setPropertyInt("bst_client_flight_mode_attempts", 1);

    kpsr::bst::test::BstClientTestMiddlewareProvider bstClientMiddlewareProvider;

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::mem::CacheListener<std::string> stateMachineListener;
    kpsr::mem::CacheListener<TelemetrySystem_t> telemetrySystemEventListener;
    stateMachineMiddlewareProvider.getSubscriber()->registerListener("test", stateMachineListener.cacheListenerFunction);

    kpsr::EventEmitterMiddlewareProvider<std::string> stateMachineExtEventMiddlewareProvider(nullptr, "test", 0, nullptr, nullptr);
    kpsr::bst::ClientStateMachine clientStateMachine(&environment, &bstClientMiddlewareProvider,
                                                     stateMachineMiddlewareProvider.getPublisher(),
                                                     stateMachineExtEventMiddlewareProvider.getSubscriber(),
                                                     & telemetrySystemEventListener);

    ASSERT_EQ(stateMachineListener.counter, 0);

    clientStateMachine.start();
    ASSERT_EQ(stateMachineListener.counter, 1);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:idle");

    bst::comms::TelemetrySystem_t telemetrySystem;
    bstClientMiddlewareProvider.testTelemetrySystemProvider.getPublisher()->publish(telemetrySystem);

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 2);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:ready");

    stateMachineExtEventMiddlewareProvider.getPublisher()->publish("payloadControlReqRx");
    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 3);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:payloadControlReadyReq");

    kpsr::bst::BstReplyMessageBuilder builder;
    builder.withId(CONTROL_COMMAND)
            .withType(CMD_PAYLOAD_CONTROL)
            .withAck(false);

    bstClientMiddlewareProvider.testBst2KpsrReplyMessageProvider.getPublisher()->publish(builder.build());

    clientStateMachine.execute();

    ASSERT_EQ(stateMachineListener.counter, 4);
    ASSERT_EQ(* stateMachineListener.getLastReceivedEvent().get(), "bstClientStateMachine:error");

    clientStateMachine.stop();
}
