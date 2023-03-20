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

#include <klepsydra/bst_comms/telemetry_pose_service.h>

#include <spdlog/spdlog.h>

#include <klepsydra/bst_comms/lat_long_utils.h>
#include <klepsydra/bst_comms/quaternion_utils.h>

kpsr::bst::TelemetryPoseService::TelemetryPoseService(
    Environment *environment,
    Subscriber<TelemetryPosition_t> *telemetryPositionSubscriber,
    Subscriber<TelemetryOrientation_t> *telemetryOrientationSubscriber,
    Publisher<kpsr::geometry::PoseEventData> *poseEventDataPublisher)
    : Service(environment, "telemetry_pose_service")
    , _telemetryPositionSubscriber(telemetryPositionSubscriber)
    , _telemetryOrientationSubscriber(telemetryOrientationSubscriber)
    , _poseEventDataPublisher(poseEventDataPublisher)
{}

void kpsr::bst::TelemetryPoseService::start()
{
    std::function<void(TelemetryPosition_t)> telemetryPositionListener =
        std::bind(&TelemetryPoseService::onTelemetryPositionReceived, this, std::placeholders::_1);
    _telemetryPositionSubscriber->registerListener("telemetry_pose_service",
                                                   telemetryPositionListener);

    std::function<void(TelemetryOrientation_t)> telemetryOrientationListener =
        std::bind(&TelemetryPoseService::onTelemetryOrientationReceived,
                  this,
                  std::placeholders::_1);
    _telemetryOrientationSubscriber->registerListener("telemetry_pose_service",
                                                      telemetryOrientationListener);
}

void kpsr::bst::TelemetryPoseService::stop()
{
    _telemetryPositionSubscriber->removeListener("telemetry_pose_service");
    _telemetryOrientationSubscriber->removeListener("telemetry_pose_service");
}

void kpsr::bst::TelemetryPoseService::onTelemetryPositionReceived(
    const ::bst::comms::TelemetryPosition_t &eventData)
{
    spdlog::debug("{} onTelemetryPositionReceived. x: {} y: {} z: {}",
                  __PRETTY_FUNCTION__,
                  eventData.latitude,
                  eventData.longitude,
                  eventData.altitude);
    if (!telemetryPositionReceived) {
        originLatitude = eventData.latitude;
        originLongitude = eventData.longitude;
        telemetryPositionReceived = true;
    }
    updatePositionAndPublish(eventData);
}

void kpsr::bst::TelemetryPoseService::onTelemetryOrientationReceived(
    const ::bst::comms::TelemetryOrientation_t &eventData)
{
    spdlog::debug("{} onTelemetryPositionReceived", __PRETTY_FUNCTION__);
    telemetryOrientationReceived = true;
    updateOrientationAndPublish(eventData);
}

void kpsr::bst::TelemetryPoseService::updatePositionAndPublish(
    const ::bst::comms::TelemetryPosition_t &telemetryPosition)
{
    this->_telemetryPosition = telemetryPosition;
    if (telemetryPositionReceived && telemetryOrientationReceived) {
        float x = LatLongUtils::CoordinatesToMeters(originLatitude,
                                                    telemetryPosition.latitude,
                                                    telemetryPosition.longitude,
                                                    telemetryPosition.longitude);
        float y = LatLongUtils::CoordinatesToMeters(telemetryPosition.latitude,
                                                    telemetryPosition.latitude,
                                                    originLongitude,
                                                    telemetryPosition.longitude);

        kpsr::geometry::PoseEventData
            poseEventDataEvent("body",
                               1,
                               true,
                               false,
                               x,
                               y,
                               telemetryPosition.altitude,
                               QuaternionUtils::quat_to_roll(_telemetryOrientation.q),
                               QuaternionUtils::quat_to_pitch(_telemetryOrientation.q),
                               QuaternionUtils::quat_to_yaw(_telemetryOrientation.q),
                               _telemetryOrientation.q[0],
                               _telemetryOrientation.q[1],
                               _telemetryOrientation.q[2],
                               _telemetryOrientation.q[3],
                               std::vector<double>(0));
        _poseEventDataPublisher->publish(poseEventDataEvent);
    }
}

void kpsr::bst::TelemetryPoseService::updateOrientationAndPublish(
    const ::bst::comms::TelemetryOrientation_t &telemetryOrientation)
{
    this->_telemetryOrientation = telemetryOrientation;
    if (telemetryPositionReceived && telemetryOrientationReceived) {
        float x = LatLongUtils::CoordinatesToMeters(originLatitude,
                                                    _telemetryPosition.latitude,
                                                    _telemetryPosition.longitude,
                                                    _telemetryPosition.longitude);
        float y = LatLongUtils::CoordinatesToMeters(_telemetryPosition.latitude,
                                                    _telemetryPosition.latitude,
                                                    originLongitude,
                                                    _telemetryPosition.longitude);
        kpsr::geometry::PoseEventData
            poseEventDataEvent("body",
                               1,
                               true,
                               false,
                               x,
                               y,
                               _telemetryPosition.altitude,
                               QuaternionUtils::quat_to_roll(telemetryOrientation.q),
                               QuaternionUtils::quat_to_pitch(telemetryOrientation.q),
                               QuaternionUtils::quat_to_yaw(telemetryOrientation.q),
                               telemetryOrientation.q[0],
                               telemetryOrientation.q[1],
                               telemetryOrientation.q[2],
                               telemetryOrientation.q[3],
                               std::vector<double>(0));
        _poseEventDataPublisher->publish(poseEventDataEvent);
    }
}
