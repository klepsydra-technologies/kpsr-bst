/*****************************************************************************
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
#ifndef BST_CLIENT_SERVER_EVENTLOOP_PROVIDER_H
#define BST_CLIENT_SERVER_EVENTLOOP_PROVIDER_H

/* BST */
#include "bst_module_basic.h"
#include "comm_packets.h"
#include "gcs.h"
#include "multicopter.h"

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/geometry/pose_event_data.h>

#include <klepsydra/bst_comms/bst_client_middleware_provider.h>
#include <klepsydra/bst_comms/bst_server_middleware_provider.h>

#include <klepsydra/bst_comms/bst_request_message.h>
#include <klepsydra/bst_comms/bst_reply_message.h>
#include <klepsydra/bst_comms/bst_kpsr_event_names.h>

namespace kpsr
{
namespace bst
{
namespace mem
{
template <std::size_t BufferSize>
/**
 * @brief The BstClientServerEventLoopProvider class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-mem-bst
 *
 * @details Client and Server in-memory eventloop implementation to all BST provided messages
 * publishers and request subscribers. Example:
@code
    kpsr::high_performance::EventLoopMiddlewareProvider<256> eventloopProvider(nullptr);
    kpsr::bst::mem::BstClientServerEventLoopProvider<256> bstClientServerMemProvider(eventloopProvider);
    kpsr::bst::BstServerMiddlewareProvider * serverMiddlewareProvider = &bstClientServerMemProvider;
    kpsr::bst::BstServiceWizard serviceWizard(nullptr, environment, serverMiddlewareProvider);

    eventloopProvider.start();

    kpsr::bst::BstServer bstTestServer(&serviceWizard);
    bstTestServer.start();

    kpsr::bst::BstClientMiddlewareProvider * bstClientMiddlewareProvider = &bstClientServerMemProvider;
    bstClientMiddlewareProvider->start();
@endcode
 */
class BstClientServerEventLoopProvider : public BstClientMiddlewareProvider, public BstServerMiddlewareProvider {
public:

    /**
     * @brief BstClientServerEventLoopProvider
     * @param eventloopProvider
     */
    BstClientServerEventLoopProvider(kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & eventloopProvider)
        : _eventloopProvider(eventloopProvider)
    {}

    /**
     * @brief start
     */
    void start() override {
    }

    /**
     * @brief stop
     */
    void stop() override {
    }

    /**
     * @brief getBstRequestMessageSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::bst::BstRequestMessage> * getBstRequestMessageSubscriber() override {
        return _eventloopProvider.template getSubscriber<kpsr::bst::BstRequestMessage>(EVENT_NAME_BST_REQUEST);
    }

    /**
     * @brief getBstRequestMessagePublisher
     * @return
     */
    kpsr::Publisher<kpsr::bst::BstRequestMessage> * getBstRequestMessagePublisher() override {
        return _eventloopProvider.template getPublisher<kpsr::bst::BstRequestMessage>(EVENT_NAME_BST_REQUEST, 0, nullptr, nullptr);
    }

    /**
     * @brief getBstWaypointCommandMessageSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::bst::WaypointCommandMessage> * getBstWaypointCommandMessageSubscriber() override {
        return _eventloopProvider.template getSubscriber<kpsr::bst::WaypointCommandMessage>(EVENT_NAME_BST_WP_CMD);
    }

    /**
     * @brief getBstWaypointCommandMessagePublisher
     * @return
     */
    kpsr::Publisher<kpsr::bst::WaypointCommandMessage> * getBstWaypointCommandMessagePublisher() override {
        return _eventloopProvider.template getPublisher<kpsr::bst::WaypointCommandMessage>(EVENT_NAME_BST_WP_CMD, 0, nullptr, nullptr);
    }

    /**
     * @brief getBst2KpsrReplyMessageSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::bst::BstReplyMessage> * getBst2KpsrReplyMessageSubscriber() override {
        return _eventloopProvider.template getSubscriber<kpsr::bst::BstReplyMessage>(EVENT_NAME_BST_REPLY);
    }

    /**
     * @brief getBst2KpsrReplyMessagePublisher
     * @return
     */
    kpsr::Publisher<kpsr::bst::BstReplyMessage> * getBst2KpsrReplyMessagePublisher() override {
        return _eventloopProvider.template getPublisher<kpsr::bst::BstReplyMessage>(EVENT_NAME_BST_REPLY, 0, nullptr, nullptr);
    }

    /**
     * @brief getPoseEventDataSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::geometry::PoseEventData> * getPoseEventDataSubscriber() override {
        return _eventloopProvider.template getSubscriber<kpsr::geometry::PoseEventData>(EVENT_NAME_POSE_EVENT);
    }

    /**
     * @brief getPoseEventDataPublisher
     * @return
     */
    kpsr::Publisher<kpsr::geometry::PoseEventData> * getPoseEventDataPublisher() override {
        return _eventloopProvider.template getPublisher<kpsr::geometry::PoseEventData>(EVENT_NAME_POSE_EVENT, 10, nullptr, nullptr);
    }

    /**
     * @brief getSystemInitializeSubscriber
     * @return
     */
    kpsr::Subscriber<SystemInitialize_t> * getSystemInitializeSubscriber() override {
        return _eventloopProvider.template getSubscriber<SystemInitialize_t>(EVENT_NAME_SYSTEM_INIT);
    }

    /**
     * @brief getSystemInitializePublisher
     * @return
     */
    kpsr::Publisher<SystemInitialize_t> * getSystemInitializePublisher() override {
        return _eventloopProvider.template getPublisher<SystemInitialize_t>(EVENT_NAME_SYSTEM_INIT, 0, nullptr, nullptr);
    }

    /**
     * @brief getTelemetryPositionSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryPosition_t> * getTelemetryPositionSubscriber() override {
        return _eventloopProvider.template getSubscriber<TelemetryPosition_t>(EVENT_NAME_TELEMETRY_POS);
    }

    /**
     * @brief getTelemetryPositionPublisher
     * @return
     */
    kpsr::Publisher<TelemetryPosition_t> * getTelemetryPositionPublisher() override {
        return _eventloopProvider.template getPublisher<TelemetryPosition_t>(EVENT_NAME_TELEMETRY_POS, 0, nullptr, nullptr);
    }

    /**
     * @brief getTelemetryOrientationSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryOrientation_t> * getTelemetryOrientationSubscriber() override {
        return _eventloopProvider.template getSubscriber<TelemetryOrientation_t>(EVENT_NAME_TELEMETRY_ORI);
    }

    /**
     * @brief getTelemetryOrientationPublisher
     * @return
     */
    kpsr::Publisher<TelemetryOrientation_t> * getTelemetryOrientationPublisher() override {
        return _eventloopProvider.template getPublisher<TelemetryOrientation_t>(EVENT_NAME_TELEMETRY_ORI, 0, nullptr, nullptr);
    }

    /**
     * @brief getSensorSubscriber
     * @return
     */
    kpsr::Subscriber<Sensors_t> * getSensorSubscriber() override {
        return _eventloopProvider.template getSubscriber<Sensors_t>(EVENT_NAME_SENSOR);
    }

    /**
     * @brief getSensorPublisher
     * @return
     */
    kpsr::Publisher<Sensors_t> * getSensorPublisher() override {
        return _eventloopProvider.template getPublisher<Sensors_t>(EVENT_NAME_SENSOR, 0, nullptr, nullptr);
    }

    /**
     * @brief getCalibrateSubscriber
     * @return
     */
    kpsr::Subscriber<CalibrateSensor_t> * getCalibrateSubscriber() override {
        return _eventloopProvider.template getSubscriber<CalibrateSensor_t>(EVENT_NAME_CALIBRATE);
    }

    /**
     * @brief getCalibratePublisher
     * @return
     */
    kpsr::Publisher<CalibrateSensor_t> * getCalibratePublisher() override {
        return _eventloopProvider.template getPublisher<CalibrateSensor_t>(EVENT_NAME_CALIBRATE, 0, nullptr, nullptr);
    }

    /**
     * @brief getControlCommandSubscriber
     * @return
     */
    kpsr::Subscriber<Command_t> * getControlCommandSubscriber() override {
        return _eventloopProvider.template getSubscriber<Command_t>(EVENT_NAME_CONTROL_CMD);
    }

    /**
     * @brief getControlCommandPublisher
     * @return
     */
    kpsr::Publisher<Command_t> * getControlCommandPublisher() override {
        return _eventloopProvider.template getPublisher<Command_t>(EVENT_NAME_CONTROL_CMD, 0, nullptr, nullptr);
    }

    /**
     * @brief getControlPidSubscriber
     * @return
     */
    kpsr::Subscriber<PID_t> * getControlPidSubscriber() override {
        return _eventloopProvider.template getSubscriber<PID_t>(EVENT_NAME_CONTROL_PID);
    }

    /**
     * @brief getControlPidPublisher
     * @return
     */
    kpsr::Publisher<PID_t> * getControlPidPublisher() override {
        return _eventloopProvider.template getPublisher<PID_t>(EVENT_NAME_CONTROL_PID, 0, nullptr, nullptr);
    }

    /**
     * @brief getSystemSubscriber
     * @return
     */
    kpsr::Subscriber<uint8_t> * getSystemSubscriber() override {
        return _eventloopProvider.template getSubscriber<uint8_t>(EVENT_NAME_SYSTEM);
    }

    /**
     * @brief getSystemPublisher
     * @return
     */
    kpsr::Publisher<uint8_t> * getSystemPublisher() override {
        return _eventloopProvider.template getPublisher<uint8_t>(EVENT_NAME_SYSTEM, 0, nullptr, nullptr);
    }

    /**
     * @brief getTelemetrySystemSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetrySystem_t> * getTelemetrySystemSubscriber() override {
        return _eventloopProvider.template getSubscriber<TelemetrySystem_t>(EVENT_NAME_TELEMETRY_SYS);
    }

    /**
     * @brief getTelemetrySystemPublisher
     * @return
     */
    kpsr::Publisher<TelemetrySystem_t> * getTelemetrySystemPublisher() override {
        return _eventloopProvider.template getPublisher<TelemetrySystem_t>(EVENT_NAME_TELEMETRY_SYS, 0, nullptr, nullptr);
    }

    /**
     * @brief getTelemetryPressureSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryPressure_t> * getTelemetryPressureSubscriber() override {
        return _eventloopProvider.template getSubscriber<TelemetryPressure_t>(EVENT_NAME_TELEMETRY_PRES);
    }

    /**
     * @brief getTelemetryPressurePublisher
     * @return
     */
    kpsr::Publisher<TelemetryPressure_t> * getTelemetryPressurePublisher() override {
        return _eventloopProvider.template getPublisher<TelemetryPressure_t>(EVENT_NAME_TELEMETRY_PRES, 0, nullptr, nullptr);
    }

    /**
     * @brief getTelemetryControlSubscriber
     * @return
     */
    kpsr::Subscriber<::bst::comms::TelemetryControl_t> * getTelemetryControlSubscriber() override {
        return _eventloopProvider.template getSubscriber<::bst::comms::TelemetryControl_t>(EVENT_NAME_TELEMETRY_CTRL);
    }

    /**
     * @brief getTelemetryControlPublisher
     * @return
     */
    kpsr::Publisher<::bst::comms::TelemetryControl_t> * getTelemetryControlPublisher() override {
        return _eventloopProvider.template getPublisher<::bst::comms::TelemetryControl_t>(EVENT_NAME_TELEMETRY_CTRL, 0, nullptr, nullptr);
    }

    /**
     * @brief getTelemetryGCSSubscriber
     * @return
     */
    kpsr::Subscriber<gcs::TelemetryGCS_t> * getTelemetryGCSSubscriber() override {
        return _eventloopProvider.template getSubscriber<gcs::TelemetryGCS_t>(EVENT_NAME_TELEMETRY_GCS);
    }

    /**
     * @brief getTelemetryGCSPublisher
     * @return
     */
    kpsr::Publisher<gcs::TelemetryGCS_t> * getTelemetryGCSPublisher() override {
        return _eventloopProvider.template getPublisher<gcs::TelemetryGCS_t>(EVENT_NAME_TELEMETRY_GCS, 0, nullptr, nullptr);
    }

    /**
     * @brief getPayloadControlSubscriber
     * @return
     */
    kpsr::Subscriber<PayloadControl_t> * getPayloadControlSubscriber() override {
        return _eventloopProvider.template getSubscriber<PayloadControl_t>(EVENT_NAME_PAYLOAD_CONTROL);
    }

    /**
     * @brief getPayloadControlPublisher
     * @return
     */
    kpsr::Publisher<PayloadControl_t> * getPayloadControlPublisher() override {
        return _eventloopProvider.template getPublisher<PayloadControl_t>(EVENT_NAME_PAYLOAD_CONTROL, 0, nullptr, nullptr);
    }

    /**
     * @brief getScheduler
     * @return
     */
    Scheduler * getScheduler() {
        return _eventloopProvider.getScheduler();
    }

private:
    kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & _eventloopProvider;
};
}
}
}

#endif // BST_CLIENT_SERVER_EVENTLOOP_PROVIDER_H

