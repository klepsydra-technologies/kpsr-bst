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
#ifndef BST_CLIENT_SERVER_MEM_PROVIDER_H
#define BST_CLIENT_SERVER_MEM_PROVIDER_H

/* BST */
#include "bst_module_basic.h"
#include "comm_packets.h"
#include "gcs.h"
#include "multicopter.h"

#include <klepsydra/mem_core/basic_middleware_provider.h>
#include <klepsydra/mem_core/basic_scheduler.h>

#include <klepsydra/geometry/pose_event_data.h>

#include <klepsydra/bst_comms/bst_client_middleware_provider.h>
#include <klepsydra/bst_comms/bst_server_middleware_provider.h>

#include <klepsydra/bst_comms/bst_request_message.h>
#include <klepsydra/bst_comms/bst_reply_message.h>
#include <klepsydra/bst_comms/waypoint_command_message.h>

namespace kpsr
{
namespace bst
{
namespace mem
{
/**
 * @brief The BstClientServerMemProvider class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-mem-bst
 *
 * @details Client and Server in-memory blocking queue implementation to all BST provided messages
 * publishers and request subscribers. Code example:
@code
    kpsr::bst::mem::BstClientServerMemProvider bstClientServerMemProvider(container) override;
    kpsr::bst::BstServerMiddlewareProvider * serverMiddlewareProvider = &bstClientServerMemProvider;
    kpsr::bst::BstServiceWizard serviceWizard(container, environment, serverMiddlewareProvider) override;
    kpsr::bst::BstServer bstTestServer(&serviceWizard) override;

    bstTestServer.start() override;
    kpsr::bst::BstClientMiddlewareProvider * bstClientMiddlewareProvider = &bstClientServerMemProvider;
@endcode
 */
class BstClientServerMemProvider : public BstClientMiddlewareProvider, public BstServerMiddlewareProvider {
public:

    /**
     * @brief BstClientServerMemProvider
     * @param container
     */
    BstClientServerMemProvider(Container * container);

    /**
     * @brief start
     */
    void start() override;

    /**
     * @brief stop
     */
    void stop() override;

    /**
     * @brief getBstRequestMessagePublisher
     * @return
     */
    Publisher<BstRequestMessage> * getBstRequestMessagePublisher() override;

    /**
     * @brief getBstRequestMessageSubscriber
     * @return
     */
    Subscriber<BstRequestMessage> * getBstRequestMessageSubscriber() override;

    /**
     * @brief getBstWaypointCommandMessagePublisher
     * @return
     */
    Publisher<WaypointCommandMessage> * getBstWaypointCommandMessagePublisher() override;

    /**
     * @brief getBstWaypointCommandMessageSubscriber
     * @return
     */
    Subscriber<WaypointCommandMessage> * getBstWaypointCommandMessageSubscriber() override;

    /**
     * @brief getBst2KpsrReplyMessageSubscriber
     * @return
     */
    Subscriber<BstReplyMessage> * getBst2KpsrReplyMessageSubscriber() override;

    /**
     * @brief getBst2KpsrReplyMessagePublisher
     * @return
     */
    Publisher<BstReplyMessage> * getBst2KpsrReplyMessagePublisher() override;

    /**
     * @brief getPoseEventDataSubscriber
     * @return
     */
    Subscriber<geometry::PoseEventData> * getPoseEventDataSubscriber() override;

    /**
     * @brief getPoseEventDataPublisher
     * @return
     */
    Publisher<geometry::PoseEventData> * getPoseEventDataPublisher() override;

    /**
     * @brief getSystemInitializeSubscriber
     * @return
     */
    Subscriber<SystemInitialize_t> * getSystemInitializeSubscriber() override;

    /**
     * @brief getSystemInitializePublisher
     * @return
     */
    Publisher<SystemInitialize_t> * getSystemInitializePublisher() override;

    /**
     * @brief getTelemetryPositionSubscriber
     * @return
     */
    Subscriber<TelemetryPosition_t> * getTelemetryPositionSubscriber() override;

    /**
     * @brief getTelemetryPositionPublisher
     * @return
     */
    Publisher<TelemetryPosition_t> * getTelemetryPositionPublisher() override;

    /**
     * @brief getTelemetryOrientationSubscriber
     * @return
     */
    Subscriber<TelemetryOrientation_t> * getTelemetryOrientationSubscriber() override;

    /**
     * @brief getTelemetryOrientationPublisher
     * @return
     */
    Publisher<TelemetryOrientation_t> * getTelemetryOrientationPublisher() override;

    /**
     * @brief getSensorSubscriber
     * @return
     */
    Subscriber<Sensors_t> * getSensorSubscriber() override;

    /**
     * @brief getSensorPublisher
     * @return
     */
    Publisher<Sensors_t> * getSensorPublisher() override;

    /**
     * @brief getCalibrateSubscriber
     * @return
     */
    Subscriber<CalibrateSensor_t> * getCalibrateSubscriber() override;

    /**
     * @brief getCalibratePublisher
     * @return
     */
    Publisher<CalibrateSensor_t> * getCalibratePublisher() override;

    /**
     * @brief getControlCommandSubscriber
     * @return
     */
    Subscriber<Command_t> * getControlCommandSubscriber() override;

    /**
     * @brief getControlCommandPublisher
     * @return
     */
    Publisher<Command_t> * getControlCommandPublisher() override;

    /**
     * @brief getControlPidSubscriber
     * @return
     */
    Subscriber<PID_t> * getControlPidSubscriber() override;

    /**
     * @brief getControlPidPublisher
     * @return
     */
    Publisher<PID_t> * getControlPidPublisher() override;

    /**
     * @brief getSystemSubscriber
     * @return
     */
    Subscriber<uint8_t> * getSystemSubscriber() override;

    /**
     * @brief getSystemPublisher
     * @return
     */
    Publisher<uint8_t> * getSystemPublisher() override;

    /**
     * @brief getTelemetrySystemSubscriber
     * @return
     */
    Subscriber<TelemetrySystem_t> * getTelemetrySystemSubscriber() override;

    /**
     * @brief getTelemetrySystemPublisher
     * @return
     */
    Publisher<TelemetrySystem_t> * getTelemetrySystemPublisher() override;

    /**
     * @brief getTelemetryPressureSubscriber
     * @return
     */
    Subscriber<TelemetryPressure_t> * getTelemetryPressureSubscriber() override;

    /**
     * @brief getTelemetryPressurePublisher
     * @return
     */
    Publisher<TelemetryPressure_t> * getTelemetryPressurePublisher() override;

    /**
     * @brief getTelemetryControlSubscriber
     * @return
     */
    Subscriber<::bst::comms::TelemetryControl_t> * getTelemetryControlSubscriber() override;

    /**
     * @brief getTelemetryControlPublisher
     * @return
     */
    Publisher<::bst::comms::TelemetryControl_t> * getTelemetryControlPublisher() override;

    /**
     * @brief getTelemetryGCSSubscriber
     * @return
     */
    Subscriber<gcs::TelemetryGCS_t> * getTelemetryGCSSubscriber() override;

    /**
     * @brief getTelemetryGCSPublisher
     * @return
     */
    Publisher<gcs::TelemetryGCS_t> * getTelemetryGCSPublisher() override;

    /**
     * @brief getPayloadControlSubscriber
     * @return
     */
    Subscriber<PayloadControl_t> * getPayloadControlSubscriber() override;

    /**
     * @brief getPayloadControlPublisher
     * @return
     */
    Publisher<PayloadControl_t> * getPayloadControlPublisher() override;

    /**
     * @brief getScheduler
     * @return
     */
    Scheduler * getScheduler() override;

private:
    kpsr::mem::BasicMiddlewareProvider<BstRequestMessage> _bstRequestMessageMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<WaypointCommandMessage> _bstWaypointCommandMessageMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<BstReplyMessage> _bstReplyMessageMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<geometry::PoseEventData> _poseEventDataMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<SystemInitialize_t> _systemInitializeMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<TelemetryPosition_t> _telemetryPositionMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<TelemetryOrientation_t> _telemetryOrientationMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<Sensors_t> _sensorMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<CalibrateSensor_t> _calibrateMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<Command_t> _controlCommandMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<PID_t> _controlPidMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<uint8_t> _systemMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<TelemetrySystem_t> _telemetrySystemMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<TelemetryPressure_t> _telemetryPressureMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<::bst::comms::TelemetryControl_t> _telemetryControlMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<gcs::TelemetryGCS_t> _telemetryGCSMiddlewareProvider;
    kpsr::mem::BasicMiddlewareProvider<PayloadControl_t> _payloadControlMiddlewareProvider;

    kpsr::mem::BasicScheduler _multithreadScheduler;
};
}
}
}

#endif // BST_CLIENT_SERVER_MEM_PROVIDER_H

