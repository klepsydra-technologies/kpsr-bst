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

#ifndef BST_SERVER_MIDDLEWARE_PROVIDER_H
#define BST_SERVER_MIDDLEWARE_PROVIDER_H

/* BST */
#include "bst_module_basic.h"
#include "comm_packets.h"
#include "gcs.h"
#include "multicopter.h"

#include <klepsydra/core/publisher.h>
#include <klepsydra/core/scheduler.h>
#include <klepsydra/core/subscriber.h>

#include <klepsydra/geometry/pose_event_data.h>

#include <klepsydra/bst_comms/bst_reply_message.h>
#include <klepsydra/bst_comms/bst_request_message.h>
#include <klepsydra/bst_comms/waypoint_command_message.h>

namespace kpsr {
namespace bst {
/**
 * @brief The BstServerMiddlewareProvider class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Server interface to all BST provided messages publishers and request subscriber.
 */
class BstServerMiddlewareProvider
{
public:
    /**
     * @brief start underlying middleware queues
     */
    virtual void start() = 0;

    /**
     * @brief stop underlying middleware queues
     */
    virtual void stop() = 0;

    /**
     * @brief BST Command request subscriber used for sending commands to BST Autopilot.
     * @return
     */
    virtual Subscriber<BstRequestMessage> *getBstRequestMessageSubscriber() = 0;

    /**
     * @brief BST Command request publisher for sending waypoints commmands
     * @return
     */
    virtual Subscriber<WaypointCommandMessage> *getBstWaypointCommandMessageSubscriber() = 0;

    /**
     * @brief Klepsydra reply queue with the result of the request command.
     * @return
     */
    virtual Publisher<BstReplyMessage> *getBst2KpsrReplyMessagePublisher() = 0;

    /**
     * @brief Klepsydra geometry telemetry message publisher.
     * @return
     */
    virtual Publisher<geometry::PoseEventData> *getPoseEventDataPublisher() = 0;

    /**
     * @brief BST SystemInitialize_t publisher
     * @return
     */
    virtual Subscriber<SystemInitialize_t> *getSystemInitializeSubscriber() = 0;

    /**
     * @brief BST SystemInitialize_t publisher
     * @return
     */
    virtual Publisher<SystemInitialize_t> *getSystemInitializePublisher() = 0;

    /**
     * @brief BST TelemetryPosition_t publisher
     * @return
     */
    virtual Subscriber<TelemetryPosition_t> *getTelemetryPositionSubscriber() = 0;

    /**
     * @brief BST TelemetryPosition_t publisher
     * @return
     */
    virtual Publisher<TelemetryPosition_t> *getTelemetryPositionPublisher() = 0;

    /**
     * @brief BST TelemetryOrientation_t publisher
     * @return
     */
    virtual Subscriber<TelemetryOrientation_t> *getTelemetryOrientationSubscriber() = 0;

    /**
     * @brief BST TelemetryOrientation_t publisher
     * @return
     */
    virtual Publisher<TelemetryOrientation_t> *getTelemetryOrientationPublisher() = 0;

    /**
     * @brief BST Sensors_t publisher
     * @return
     */
    virtual Publisher<Sensors_t> *getSensorPublisher() = 0;

    /**
     * @brief BST CalibrateSensor_t publisher
     * @return
     */
    virtual Publisher<CalibrateSensor_t> *getCalibratePublisher() = 0;

    /**
     * @brief BST Command_t publisher
     * @return
     */
    virtual Publisher<Command_t> *getControlCommandPublisher() = 0;

    /**
     * @brief BST PID_t publisher
     * @return
     */
    virtual Publisher<PID_t> *getControlPidPublisher() = 0;

    /**
     * @brief BST uint8_t publisher
     * @return
     */
    virtual Publisher<uint8_t> *getSystemPublisher() = 0;

    /**
     * @brief BST TelemetrySystem_t publisher
     * @return
     */
    virtual Publisher<TelemetrySystem_t> *getTelemetrySystemPublisher() = 0;

    /**
     * @brief BST TelemetryPressure_t publisher
     * @return
     */
    virtual Publisher<TelemetryPressure_t> *getTelemetryPressurePublisher() = 0;

    /**
     * @brief BST ::bst::comms::TelemetryControl_t publisher
     * @return
     */
    virtual Publisher<::bst::comms::TelemetryControl_t> *getTelemetryControlPublisher() = 0;

    /**
     * @brief BST gcs::TelemetryGCS_t publisher
     * @return
     */
    virtual Publisher<gcs::TelemetryGCS_t> *getTelemetryGCSPublisher() = 0;

    /**
     * @brief BST PayloadControl_t publisher
     * @return
     */
    virtual Publisher<PayloadControl_t> *getPayloadControlPublisher() = 0;
};
} // namespace bst
} // namespace kpsr

#endif // BST_SERVER_MIDDLEWARE_PROVIDER_H
