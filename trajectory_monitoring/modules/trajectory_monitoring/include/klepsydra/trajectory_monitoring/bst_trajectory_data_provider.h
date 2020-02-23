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

#ifndef BST_TRAJECTORY_DATA_PROVIDER_H
#define BST_TRAJECTORY_DATA_PROVIDER_H

#include <memory>

/* BST */
#include "bst_module_basic.h"
#include "comm_packets.h"
#include "gcs.h"
#include "multicopter.h"

/* KPSR */
#include <klepsydra/core/service.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/core/cache_listener.h>

#include <klepsydra/geometry/pose_event_data.h>

#include <klepsydra/bst_comms/waypoint_command_message.h>
#include <klepsydra/bst_comms/bst_request_message.h>

#include <klepsydra/trajectory_monitoring/trajectory_data_provider.h>

namespace kpsr {
namespace trajectory {
namespace bst {
class BstTrajectoryDataProvider : public TrajectoryDataProvider, public Service {
public:
    BstTrajectoryDataProvider(kpsr::Environment * environment,
                              kpsr::Subscriber<::bst::comms::TelemetryPosition_t> * bstTelemetryPosition,
                              kpsr::Subscriber<::bst::comms::TelemetryOrientation_t> * bstTelemetryOrientation,
                              kpsr::Subscriber<kpsr::geometry::PoseEventData> * bstPositionAndOrientation,
                              kpsr::Subscriber<kpsr::bst::WaypointCommandMessage> * bstWaypointComands,
                              kpsr::Subscriber<kpsr::bst::BstRequestMessage> * bstBstRequestMessages,
                              bool threadSafe);

    void start() override;
    void stop() override;
    void execute() override;

    bool getCurrentPosition(Position & currentPosition) override;
    bool getCurrentGpsPosition(GpsPosition & currentGpsPosition) override;
    bool getCurrentOrientation(Orientation & currentOrientation) override;
    bool getCurrentLinearVelocity(LinearVelocity & currentLinearVelocity) override;
    bool getCurrentAngularRates(AngularRates & currentAngularRates) override;

    bool getCommandedPosition(Position & commandedPosition) override;
    bool getCommandedGpsPosition(GpsPosition & commandedGpsPosition) override;
    bool getCommandedOrientation(Orientation & commandedOrientation) override;
    bool getCommandedLinearVelocity(LinearVelocity & commandedLinearVelocity) override;
    bool getCommandedAngularRates(AngularRates & commandedAngularRates) override;

private:

    void onBstRequestMessage(const kpsr::bst::BstRequestMessage & event);

    kpsr::Subscriber<::bst::comms::TelemetryPosition_t> * _bstTelemetryPosition;
    kpsr::Subscriber<::bst::comms::TelemetryOrientation_t> * _bstTelemetryOrientation;
    kpsr::Subscriber<kpsr::geometry::PoseEventData> * _bstPositionAndOrientation;
    kpsr::Subscriber<kpsr::bst::WaypointCommandMessage> * _bstWaypointComands;
    kpsr::Subscriber<kpsr::bst::BstRequestMessage> * _bstBstRequestMessages;

    std::unique_ptr<kpsr::mem::CacheListener<::bst::comms::TelemetryPosition_t>> _telemetryPositionElement;
    std::unique_ptr<kpsr::mem::CacheListener<::bst::comms::TelemetryOrientation_t>> _telemetryOrientationElement;
    std::unique_ptr<kpsr::mem::CacheListener<kpsr::geometry::PoseEventData>> _poseEventDataElement;

    std::unique_ptr<kpsr::mem::CacheListener<kpsr::bst::WaypointCommandMessage>> _waypointCommandElement;

    std::unique_ptr<kpsr::mem::CacheListener<Position>> _commandedPositionElement;
    std::unique_ptr<kpsr::mem::CacheListener<GpsPosition>> _commandedGpsPositionElement;
    std::unique_ptr<kpsr::mem::CacheListener<Orientation>> _commandedOrientationElement;
    std::unique_ptr<kpsr::mem::CacheListener<LinearVelocity>> _commandedLinearVelocityElement;
    std::unique_ptr<kpsr::mem::CacheListener<AngularRates>> _commandedAngularRatesElement;

    long _currPosSeq = 0, _currGpsSeq = 0, _currOriSeq = 0, _currLinSeq = 0, _currAngSeq = 0,
    _cmdPosSeq = 9, _cmdGpsSeq = 0, _cmdOriSeq = 0, _cmdLinSeq = 0, _cmdAngSeq = 0;
};
}
}
}

#endif // BST_TRAJECTORY_DATA_PROVIDER_H
