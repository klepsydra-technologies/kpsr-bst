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


#include <algorithm>
#include <iostream>

#include <klepsydra/trajectory_monitoring/bst_trajectory_data_provider.h>

kpsr::trajectory::bst::BstTrajectoryDataProvider::BstTrajectoryDataProvider(
        kpsr::Environment * environment,
        kpsr::Subscriber<::bst::comms::TelemetryPosition_t> * bstTelemetryPosition,
        kpsr::Subscriber<::bst::comms::TelemetryOrientation_t> * bstTelemetryOrientation,
        kpsr::Subscriber<kpsr::geometry::PoseEventData> * bstPositionAndOrientation,
        kpsr::Subscriber<kpsr::bst::WaypointCommandMessage> * bstWaypointComands,
        kpsr::Subscriber<kpsr::bst::BstRequestMessage> * bstBstRequestMessages,
        bool threadSafe)
    : kpsr::Service(environment, "bst_trajectory_data_service")
    , _bstTelemetryPosition(bstTelemetryPosition)
    , _bstTelemetryOrientation(bstTelemetryOrientation)
    , _bstPositionAndOrientation(bstPositionAndOrientation)
    , _bstWaypointComands(bstWaypointComands)
    , _bstBstRequestMessages(bstBstRequestMessages)
    , _telemetryPositionElement(threadSafe ? new kpsr::mem::MultiThreadCacheListener<::bst::comms::TelemetryPosition_t>() : new kpsr::mem::CacheListener<::bst::comms::TelemetryPosition_t>())
    , _telemetryOrientationElement(threadSafe ? new kpsr::mem::MultiThreadCacheListener<::bst::comms::TelemetryOrientation_t>() : new kpsr::mem::CacheListener<::bst::comms::TelemetryOrientation_t>())
    , _poseEventDataElement(threadSafe ? new kpsr::mem::MultiThreadCacheListener<kpsr::geometry::PoseEventData>() : new kpsr::mem::CacheListener<kpsr::geometry::PoseEventData>())
    , _waypointCommandElement(threadSafe ? new kpsr::mem::MultiThreadCacheListener<kpsr::bst::WaypointCommandMessage>() : new kpsr::mem::CacheListener<kpsr::bst::WaypointCommandMessage>())
    , _commandedPositionElement(threadSafe ? new kpsr::mem::MultiThreadCacheListener<Position>() : new kpsr::mem::CacheListener<Position>())
    , _commandedGpsPositionElement(threadSafe ? new kpsr::mem::MultiThreadCacheListener<GpsPosition>() : new kpsr::mem::CacheListener<GpsPosition>())
    , _commandedOrientationElement(threadSafe ? new kpsr::mem::MultiThreadCacheListener<Orientation>() : new kpsr::mem::CacheListener<Orientation>())
    , _commandedLinearVelocityElement(threadSafe ? new kpsr::mem::MultiThreadCacheListener<LinearVelocity>() : new kpsr::mem::CacheListener<LinearVelocity>())
    , _commandedAngularRatesElement(threadSafe ? new kpsr::mem::CacheListener<AngularRates>(): new kpsr::mem::CacheListener<AngularRates>())
{}

void kpsr::trajectory::bst::BstTrajectoryDataProvider::start() {
    _bstTelemetryPosition->registerListener("bst_traj_data_provider", _telemetryPositionElement->cacheListenerFunction);
    _bstTelemetryOrientation->registerListener("bst_traj_data_provider", _telemetryOrientationElement->cacheListenerFunction);
    _bstPositionAndOrientation->registerListener("bst_traj_data_provider", _poseEventDataElement->cacheListenerFunction);

    _bstWaypointComands->registerListener("bst_traj_data_provider", _waypointCommandElement->cacheListenerFunction);

    std::function<void(const kpsr::bst::BstRequestMessage &)> bstBstRequestMessagesListener = std::bind(&kpsr::trajectory::bst::BstTrajectoryDataProvider::onBstRequestMessage, this, std::placeholders::_1);
    _bstBstRequestMessages->registerListener("bst_traj_data_provider", bstBstRequestMessagesListener);
}

void kpsr::trajectory::bst::BstTrajectoryDataProvider::stop() {
    _bstTelemetryPosition->removeListener("bst_traj_data_provider");
    _bstTelemetryOrientation->removeListener("bst_traj_data_provider");
    _bstPositionAndOrientation->removeListener("bst_traj_data_provider");
    _bstWaypointComands->removeListener("bst_traj_data_provider");
    _bstBstRequestMessages->removeListener("bst_traj_data_provider");
}

void kpsr::trajectory::bst::BstTrajectoryDataProvider::execute() {}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCurrentPosition(Position & currentPosition) {
    if (_poseEventDataElement->counter == 0) {
        return false;
    }
    std::shared_ptr<kpsr::geometry::PoseEventData> eventData = _poseEventDataElement->getLastReceivedEvent();
    currentPosition = Position(_currPosSeq++, eventData->x, eventData->y, eventData->z);
    return true;
}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCurrentGpsPosition(GpsPosition & currentGpsPosition) {
    if (_telemetryPositionElement->counter == 0) {
        return false;
    }
    std::shared_ptr<::bst::comms::TelemetryPosition_t> eventData = _telemetryPositionElement->getLastReceivedEvent();
    currentGpsPosition = GpsPosition(_currGpsSeq++, eventData->latitude, eventData->longitude, eventData->altitude);
    return true;
}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCurrentOrientation(Orientation & currentOrientation) {
    if (_poseEventDataElement->counter == 0) {
        return false;
    }
    std::shared_ptr<kpsr::geometry::PoseEventData> eventData = _poseEventDataElement->getLastReceivedEvent();
    currentOrientation = Orientation(_currOriSeq++, eventData->roll, eventData->pitch, eventData->yaw);
    return true;
}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCurrentLinearVelocity(LinearVelocity & currentLinearVelocity) {
    if (_telemetryPositionElement->counter == 0) {
        return false;
    }
    std::shared_ptr<::bst::comms::TelemetryPosition_t> eventData = _telemetryPositionElement->getLastReceivedEvent();
    currentLinearVelocity = LinearVelocity (_currLinSeq++, eventData->velocity.x, eventData->velocity.y, eventData->velocity.z);
    return true;
}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCurrentAngularRates(AngularRates & currentAngularRates) {
    if (_telemetryOrientationElement->counter == 0) {
        return false;
    }
    std::shared_ptr<::bst::comms::TelemetryOrientation_t> eventData = _telemetryOrientationElement->getLastReceivedEvent();
    currentAngularRates = AngularRates(_currAngSeq++, eventData->omega.x, eventData->omega.y, eventData->omega.z);
    return true;
}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCommandedPosition(Position & commandedPosition) {
    if (_commandedPositionElement->counter == 0) {
        return false;
    }
    commandedPosition = * _commandedPositionElement->getLastReceivedEvent().get();
    return true;
}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCommandedGpsPosition(GpsPosition & commandedGpsPosition) {
    if (_commandedGpsPositionElement->counter == 0) {
        return false;
    }
    commandedGpsPosition = * _commandedGpsPositionElement->getLastReceivedEvent().get();
    return true;
}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCommandedOrientation(Orientation & commandedOrientation) {
    if (_commandedOrientationElement->counter == 0) {
        return false;
    }
    commandedOrientation = * _commandedOrientationElement->getLastReceivedEvent().get();
    return true;
}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCommandedLinearVelocity(LinearVelocity & commandedLinearVelocity) {
    if (_commandedLinearVelocityElement->counter == 0) {
        return false;
    }
    commandedLinearVelocity = * _commandedLinearVelocityElement->getLastReceivedEvent().get();
    return true;
}

bool kpsr::trajectory::bst::BstTrajectoryDataProvider::getCommandedAngularRates(AngularRates & commandedAngularRates) {
    if (_commandedAngularRatesElement->counter == 0) {
        return false;
    }
    commandedAngularRates = * _commandedAngularRatesElement->getLastReceivedEvent();
    return true;
}

void kpsr::trajectory::bst::BstTrajectoryDataProvider::onBstRequestMessage(const kpsr::bst::BstRequestMessage & event) {
    if ((event.id != CONTROL_COMMAND) || (event.type != CMD_WAYPOINT)) {
        return;
    }
    if (_waypointCommandElement->counter == 0) {
        return;
    }

    std::shared_ptr<kpsr::bst::WaypointCommandMessage> waypointCommand = _waypointCommandElement->getLastReceivedEvent();
    unsigned char value = event.value;
    std::vector<kpsr::bst::WaypointMessage>::iterator it =
            std::find_if(waypointCommand->plan.begin(), waypointCommand->plan.end(),
                         [&value](kpsr::bst::WaypointMessage & message) { return (message.num == value); });
    if (it != std::end(waypointCommand->plan)) {
        _commandedGpsPositionElement->onEventReceivedRaw(
                    std::shared_ptr<GpsPosition>(new GpsPosition(_cmdGpsSeq++,
                                                                 it->latitude,
                                                                 it->longitude,
                                                                 it->altitude)));
    }
}
