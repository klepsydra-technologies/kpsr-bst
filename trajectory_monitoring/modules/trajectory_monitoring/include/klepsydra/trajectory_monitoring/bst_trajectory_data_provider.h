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
