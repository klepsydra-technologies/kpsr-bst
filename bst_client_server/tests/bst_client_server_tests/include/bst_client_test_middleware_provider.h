/*
 * Copyright 2023 Klepsydra Technologies AG
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BST_CLIENT_TEST_MIDDLEWARE_PROVIDER_H
#define BST_CLIENT_TEST_MIDDLEWARE_PROVIDER_H

#include <klepsydra/core/event_emitter_middleware_provider.h>

#include <klepsydra/bst_comms/bst_client_middleware_provider.h>

namespace kpsr {
namespace bst {
namespace test {
class BstClientTestMiddlewareProvider : public BstClientMiddlewareProvider
{
public:
    BstClientTestMiddlewareProvider()
        : testBstRequestMessageProvider(nullptr, "test", 0, nullptr, nullptr)
        , testBst2KpsrReplyMessageProvider(nullptr, "test", 0, nullptr, nullptr)
        , testBstWaypointCommandMessageProvider(nullptr, "test", 0, nullptr, nullptr)
        , testPoseEventDataProvider(nullptr, "test", 0, nullptr, nullptr)
        , testSystemInitializeProvider(nullptr, "test", 0, nullptr, nullptr)
        , testTelemetryPositionProvider(nullptr, "test", 0, nullptr, nullptr)
        , testTelemetryOrientationProvider(nullptr, "test", 0, nullptr, nullptr)
        , testSensorProvider(nullptr, "test", 0, nullptr, nullptr)
        , testCalibrateProvider(nullptr, "test", 0, nullptr, nullptr)
        , testControlCommandProvider(nullptr, "test", 0, nullptr, nullptr)
        , testControlPidProvider(nullptr, "test", 0, nullptr, nullptr)
        , testSystemProvider(nullptr, "test", 0, nullptr, nullptr)
        , testTelemetrySystemProvider(nullptr, "test", 0, nullptr, nullptr)
        , testTelemetryPressureProvider(nullptr, "test", 0, nullptr, nullptr)
        , testTelemetryControlProvider(nullptr, "test", 0, nullptr, nullptr)
        , testTelemetryGCSProvider(nullptr, "test", 0, nullptr, nullptr)
        , testPayloadControlProvider(nullptr, "test", 0, nullptr, nullptr)
    {}

    virtual void start() override {}
    virtual void stop() override {}

    virtual Publisher<BstRequestMessage> *getBstRequestMessagePublisher() override
    {
        return testBstRequestMessageProvider.getPublisher();
    }

    virtual Subscriber<BstReplyMessage> *getBst2KpsrReplyMessageSubscriber() override
    {
        return testBst2KpsrReplyMessageProvider.getSubscriber();
    }

    virtual Publisher<WaypointCommandMessage> *getBstWaypointCommandMessagePublisher() override
    {
        return testBstWaypointCommandMessageProvider.getPublisher();
    }

    virtual Subscriber<geometry::PoseEventData> *getPoseEventDataSubscriber() override
    {
        return testPoseEventDataProvider.getSubscriber();
    }

    virtual Subscriber<SystemInitialize_t> *getSystemInitializeSubscriber() override
    {
        return testSystemInitializeProvider.getSubscriber();
    }

    virtual Subscriber<TelemetryPosition_t> *getTelemetryPositionSubscriber() override
    {
        return testTelemetryPositionProvider.getSubscriber();
    }

    virtual Subscriber<TelemetryOrientation_t> *getTelemetryOrientationSubscriber() override
    {
        return testTelemetryOrientationProvider.getSubscriber();
    }

    virtual Subscriber<Sensors_t> *getSensorSubscriber() override
    {
        return testSensorProvider.getSubscriber();
    }

    virtual Subscriber<CalibrateSensor_t> *getCalibrateSubscriber() override
    {
        return testCalibrateProvider.getSubscriber();
    }

    virtual Subscriber<Command_t> *getControlCommandSubscriber() override
    {
        return testControlCommandProvider.getSubscriber();
    }

    virtual Subscriber<PID_t> *getControlPidSubscriber() override
    {
        return testControlPidProvider.getSubscriber();
    }

    virtual Subscriber<uint8_t> *getSystemSubscriber() override
    {
        return testSystemProvider.getSubscriber();
    }

    virtual Subscriber<TelemetrySystem_t> *getTelemetrySystemSubscriber() override
    {
        return testTelemetrySystemProvider.getSubscriber();
    }

    virtual Subscriber<TelemetryPressure_t> *getTelemetryPressureSubscriber() override
    {
        return testTelemetryPressureProvider.getSubscriber();
    }

    virtual Subscriber<::bst::comms::TelemetryControl_t> *getTelemetryControlSubscriber() override
    {
        return testTelemetryControlProvider.getSubscriber();
    }

    virtual Subscriber<gcs::TelemetryGCS_t> *getTelemetryGCSSubscriber() override
    {
        return testTelemetryGCSProvider.getSubscriber();
    }

    virtual Subscriber<PayloadControl_t> *getPayloadControlSubscriber() override
    {
        return testPayloadControlProvider.getSubscriber();
    }

    virtual Scheduler *getScheduler() override { return nullptr; }

    EventEmitterMiddlewareProvider<BstRequestMessage> testBstRequestMessageProvider;
    EventEmitterMiddlewareProvider<BstReplyMessage> testBst2KpsrReplyMessageProvider;
    EventEmitterMiddlewareProvider<WaypointCommandMessage> testBstWaypointCommandMessageProvider;
    EventEmitterMiddlewareProvider<geometry::PoseEventData> testPoseEventDataProvider;
    EventEmitterMiddlewareProvider<SystemInitialize_t> testSystemInitializeProvider;
    EventEmitterMiddlewareProvider<TelemetryPosition_t> testTelemetryPositionProvider;
    EventEmitterMiddlewareProvider<TelemetryOrientation_t> testTelemetryOrientationProvider;
    EventEmitterMiddlewareProvider<Sensors_t> testSensorProvider;
    EventEmitterMiddlewareProvider<CalibrateSensor_t> testCalibrateProvider;
    EventEmitterMiddlewareProvider<Command_t> testControlCommandProvider;
    EventEmitterMiddlewareProvider<PID_t> testControlPidProvider;
    EventEmitterMiddlewareProvider<uint8_t> testSystemProvider;
    EventEmitterMiddlewareProvider<TelemetrySystem_t> testTelemetrySystemProvider;
    EventEmitterMiddlewareProvider<TelemetryPressure_t> testTelemetryPressureProvider;
    EventEmitterMiddlewareProvider<::bst::comms::TelemetryControl_t> testTelemetryControlProvider;
    EventEmitterMiddlewareProvider<gcs::TelemetryGCS_t> testTelemetryGCSProvider;
    EventEmitterMiddlewareProvider<PayloadControl_t> testPayloadControlProvider;
};
} // namespace test
} // namespace bst
} // namespace kpsr

#endif // BST_CLIENT_TEST_MIDDLEWARE_PROVIDER_H
