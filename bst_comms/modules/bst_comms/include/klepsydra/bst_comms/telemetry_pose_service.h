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
#ifndef TELEMETRY_POSE_SERVICE_H
#define TELEMETRY_POSE_SERVICE_H

/* BST */
#include "netuas_socket.h"
#include "bst_module_basic.h"
#include "bst_protocol.h"
#include "helper_functions.h"
#include "comm_packets.h"
#include "comm_interface.h"
#include "comm_protocol.h"
#include "bst_module_flight_plan.h"

#include <klepsydra/core/service.h>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>

#include <klepsydra/geometry/pose_event_data.h>

namespace kpsr
{
namespace bst
{
/**
 * @brief The TelemetryPoseService class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Conversion from Latlong to position and orientation.
 */
class TelemetryPoseService : public Service {

public:

    /**
     * @brief TelemetryPoseService
     * @param environment
     * @param telemetryPositionSubscriber
     * @param telemetryOrientationSubscriber
     * @param poseEventDataPublisher
     */
    TelemetryPoseService(Environment * environment,
                         Subscriber<TelemetryPosition_t> * telemetryPositionSubscriber,
                         Subscriber<TelemetryOrientation_t> * telemetryOrientationSubscriber,
                         Publisher<kpsr::geometry::PoseEventData> * poseEventDataPublisher);

    /**
     * @brief onTelemetryPositionReceived
     * @param eventData
     */
    void onTelemetryPositionReceived(const ::bst::comms::TelemetryPosition_t  & eventData);

    /**
     * @brief onTelemetryOrientationReceived
     * @param eventData
     */
    void onTelemetryOrientationReceived(const ::bst::comms::TelemetryOrientation_t  & eventData);

    /**
     * @brief updatePositionAndPublish
     * @param telemetryPosition
     */
    void updatePositionAndPublish(const ::bst::comms::TelemetryPosition_t & telemetryPosition);

    /**
     * @brief updateOrientationAndPublish
     * @param telemetryOrientation
     */
    void updateOrientationAndPublish(const ::bst::comms::TelemetryOrientation_t & telemetryOrientation);

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
    Subscriber<TelemetryPosition_t> * _telemetryPositionSubscriber;
    Subscriber<TelemetryOrientation_t> * _telemetryOrientationSubscriber;
    Publisher<kpsr::geometry::PoseEventData> * _poseEventDataPublisher;

    bool telemetryPositionReceived = false;
    bool telemetryOrientationReceived = false;
    ::bst::comms::TelemetryPosition_t _telemetryPosition;
    ::bst::comms::TelemetryOrientation_t _telemetryOrientation;

    double originLatitude, originLongitude;
};
}
}
#endif // TELEMETRY_POSE_SERVICE_H

