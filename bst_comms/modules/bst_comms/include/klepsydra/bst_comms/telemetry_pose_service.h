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

#ifndef TELEMETRY_POSE_SERVICE_H
#define TELEMETRY_POSE_SERVICE_H

/* BST */
#include "bst_module_basic.h"
#include "bst_module_flight_plan.h"
#include "bst_protocol.h"
#include "comm_interface.h"
#include "comm_packets.h"
#include "comm_protocol.h"
#include "helper_functions.h"
#include "netuas_socket.h"

#include <klepsydra/core/publisher.h>
#include <klepsydra/core/service.h>
#include <klepsydra/core/subscriber.h>

#include <klepsydra/geometry/pose_event_data.h>

namespace kpsr {
namespace bst {
/**
 * @brief The TelemetryPoseService class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Conversion from Latlong to position and orientation.
 */
class TelemetryPoseService : public Service
{
public:
    /**
     * @brief TelemetryPoseService
     * @param environment
     * @param telemetryPositionSubscriber
     * @param telemetryOrientationSubscriber
     * @param poseEventDataPublisher
     */
    TelemetryPoseService(Environment *environment,
                         Subscriber<TelemetryPosition_t> *telemetryPositionSubscriber,
                         Subscriber<TelemetryOrientation_t> *telemetryOrientationSubscriber,
                         Publisher<kpsr::geometry::PoseEventData> *poseEventDataPublisher);

    /**
     * @brief onTelemetryPositionReceived
     * @param eventData
     */
    void onTelemetryPositionReceived(const ::bst::comms::TelemetryPosition_t &eventData);

    /**
     * @brief onTelemetryOrientationReceived
     * @param eventData
     */
    void onTelemetryOrientationReceived(const ::bst::comms::TelemetryOrientation_t &eventData);

    /**
     * @brief updatePositionAndPublish
     * @param telemetryPosition
     */
    void updatePositionAndPublish(const ::bst::comms::TelemetryPosition_t &telemetryPosition);

    /**
     * @brief updateOrientationAndPublish
     * @param telemetryOrientation
     */
    void updateOrientationAndPublish(
        const ::bst::comms::TelemetryOrientation_t &telemetryOrientation);

    /**
     * @brief start
     */
    void start();

    /**
     * @brief stop
     */
    void stop();

    /**
     * @brief execute
     */
    void execute() {}

private:
    Subscriber<TelemetryPosition_t> *_telemetryPositionSubscriber;
    Subscriber<TelemetryOrientation_t> *_telemetryOrientationSubscriber;
    Publisher<kpsr::geometry::PoseEventData> *_poseEventDataPublisher;

    bool telemetryPositionReceived = false;
    bool telemetryOrientationReceived = false;
    ::bst::comms::TelemetryPosition_t _telemetryPosition;
    ::bst::comms::TelemetryOrientation_t _telemetryOrientation;

    double originLatitude, originLongitude;
};
} // namespace bst
} // namespace kpsr
#endif // TELEMETRY_POSE_SERVICE_H
