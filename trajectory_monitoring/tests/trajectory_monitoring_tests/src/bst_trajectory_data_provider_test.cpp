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


#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <math.h>

#include <sstream>
#include <fstream>

#include "gtest/gtest.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <klepsydra/core/event_emitter_middleware_provider.h>

#include <klepsydra/mem_core/mem_env.h>

#include <klepsydra/trajectory_monitoring/bst_trajectory_data_provider.h>

TEST(BstTrajectoryDataProviderTest, AvailableDataLeadsToMessageResponse) {
    spdlog::info("BstTrajectoryDataProviderTest");
    kpsr::mem::MemEnv environment;

    kpsr::EventEmitterMiddlewareProvider<::bst::comms::TelemetryPosition_t> bstTelemetryPositionProvider(nullptr, "bst_telemetry_position", 0, nullptr, nullptr);
    kpsr::EventEmitterMiddlewareProvider<::bst::comms::TelemetryOrientation_t> bstTelemetryOrientationProvider(nullptr, "bst_telemetry_orientation", 0, nullptr, nullptr);
    kpsr::EventEmitterMiddlewareProvider<kpsr::geometry::PoseEventData> bstPositionAndOrientationProvider(nullptr, "bst_kpsr_pose", 0, nullptr, nullptr);
    kpsr::EventEmitterMiddlewareProvider<kpsr::bst::WaypointCommandMessage> bstWaypointComandsProvider(nullptr, "bst_waypoint_command", 0, nullptr, nullptr);
    kpsr::EventEmitterMiddlewareProvider<kpsr::bst::BstRequestMessage> bstBstRequestMessagesProvider(nullptr, "bst_request", 0, nullptr, nullptr);

    kpsr::trajectory::bst::BstTrajectoryDataProvider sut(&environment,
                                                         bstTelemetryPositionProvider.getSubscriber(),
                                                         bstTelemetryOrientationProvider.getSubscriber(),
                                                         bstPositionAndOrientationProvider.getSubscriber(),
                                                         bstWaypointComandsProvider.getSubscriber(),
                                                         bstBstRequestMessagesProvider.getSubscriber(),
                                                         false);


    sut.start();

    // Before sending any command, no data is available
    kpsr::trajectory::AngularRates cmdAngularRates;
    kpsr::trajectory::GpsPosition cmdGpsPosition;
    kpsr::trajectory::LinearVelocity cmdLinearVelocity;
    kpsr::trajectory::Orientation cmdOrientation;
    kpsr::trajectory::Position cmdPosition;

    kpsr::trajectory::AngularRates currAngularRates;
    kpsr::trajectory::GpsPosition currGpsPosition;
    kpsr::trajectory::LinearVelocity currLinearVelocity;
    kpsr::trajectory::Orientation currOrientation;
    kpsr::trajectory::Position currPosition;

    ASSERT_FALSE(sut.getCommandedAngularRates(cmdAngularRates));
    ASSERT_FALSE(sut.getCommandedGpsPosition(cmdGpsPosition));
    ASSERT_FALSE(sut.getCommandedLinearVelocity(cmdLinearVelocity));
    ASSERT_FALSE(sut.getCommandedOrientation(cmdOrientation));
    ASSERT_FALSE(sut.getCommandedPosition(cmdPosition));

    ASSERT_FALSE(sut.getCurrentAngularRates(currAngularRates));
    ASSERT_FALSE(sut.getCurrentGpsPosition(currGpsPosition));
    ASSERT_FALSE(sut.getCurrentLinearVelocity(currLinearVelocity));
    ASSERT_FALSE(sut.getCurrentOrientation(currOrientation));
    ASSERT_FALSE(sut.getCurrentPosition(currPosition));

    ::bst::comms::TelemetryPosition_t telemetryPosition;
    telemetryPosition.latitude = 1.0;
    telemetryPosition.longitude = 2.0;
    telemetryPosition.altitude = 100.0;
    telemetryPosition.velocity.x = 10.0;
    telemetryPosition.velocity.y = 20.0;
    telemetryPosition.velocity.z = 30.0;
    bstTelemetryPositionProvider.getPublisher()->publish(telemetryPosition);

    // First trial gets data.
    ASSERT_TRUE(sut.getCurrentGpsPosition(currGpsPosition));
    ASSERT_FLOAT_EQ(currGpsPosition.latitude, telemetryPosition.latitude);
    ASSERT_FLOAT_EQ(currGpsPosition.longitude, telemetryPosition.longitude);
    ASSERT_FLOAT_EQ(currGpsPosition.altitude, telemetryPosition.altitude);

    ASSERT_TRUE(sut.getCurrentLinearVelocity(currLinearVelocity));
    ASSERT_FLOAT_EQ(currLinearVelocity.u, telemetryPosition.velocity.x);
    ASSERT_FLOAT_EQ(currLinearVelocity.v, telemetryPosition.velocity.y);
    ASSERT_FLOAT_EQ(currLinearVelocity.w, telemetryPosition.velocity.z);

    // Second does not.
    ASSERT_TRUE(sut.getCurrentGpsPosition(currGpsPosition));
    ASSERT_FLOAT_EQ(currGpsPosition.latitude, telemetryPosition.latitude);
    ASSERT_FLOAT_EQ(currGpsPosition.longitude, telemetryPosition.longitude);
    ASSERT_FLOAT_EQ(currGpsPosition.altitude, telemetryPosition.altitude);

    ASSERT_TRUE(sut.getCurrentLinearVelocity(currLinearVelocity));
    ASSERT_FLOAT_EQ(currLinearVelocity.u, telemetryPosition.velocity.x);
    ASSERT_FLOAT_EQ(currLinearVelocity.v, telemetryPosition.velocity.y);
    ASSERT_FLOAT_EQ(currLinearVelocity.w, telemetryPosition.velocity.z);

    ::bst::comms::TelemetryOrientation_t telemetryOrientation;
    telemetryOrientation.omega.x = 0.1;
    telemetryOrientation.omega.y = 0.2;
    telemetryOrientation.omega.z = 0.3;
    bstTelemetryOrientationProvider.getPublisher()->publish(telemetryOrientation);

    // First trial gets data.
    ASSERT_TRUE(sut.getCurrentAngularRates(currAngularRates));
    ASSERT_FLOAT_EQ(currAngularRates.p, telemetryOrientation.omega.x);
    ASSERT_FLOAT_EQ(currAngularRates.q, telemetryOrientation.omega.y);
    ASSERT_FLOAT_EQ(currAngularRates.r, telemetryOrientation.omega.z);

    // Second does not.
    ASSERT_TRUE(sut.getCurrentAngularRates(currAngularRates));
    ASSERT_FLOAT_EQ(currAngularRates.p, telemetryOrientation.omega.x);
    ASSERT_FLOAT_EQ(currAngularRates.q, telemetryOrientation.omega.y);
    ASSERT_FLOAT_EQ(currAngularRates.r, telemetryOrientation.omega.z);

    kpsr::geometry::PoseEventData poseEventData;
    poseEventData.x = 50;
    poseEventData.y = 60;
    poseEventData.z = 70;
    poseEventData.roll = 0.5;
    poseEventData.pitch = 0.6;
    poseEventData.yaw = 0.7;
    bstPositionAndOrientationProvider.getPublisher()->publish(poseEventData);

    // First trial gets data.
    ASSERT_TRUE(sut.getCurrentPosition(currPosition));
    ASSERT_FLOAT_EQ(currPosition.x, poseEventData.x);
    ASSERT_FLOAT_EQ(currPosition.y, poseEventData.y);
    ASSERT_FLOAT_EQ(currPosition.z, poseEventData.z);

    ASSERT_TRUE(sut.getCurrentOrientation(currOrientation));
    ASSERT_FLOAT_EQ(currOrientation.roll, poseEventData.roll);
    ASSERT_FLOAT_EQ(currOrientation.pitch, poseEventData.pitch);
    ASSERT_FLOAT_EQ(currOrientation.yaw, poseEventData.yaw);

    // Second does not.
    ASSERT_TRUE(sut.getCurrentPosition(currPosition));
    ASSERT_FLOAT_EQ(currPosition.x, poseEventData.x);
    ASSERT_FLOAT_EQ(currPosition.y, poseEventData.y);
    ASSERT_FLOAT_EQ(currPosition.z, poseEventData.z);

    ASSERT_TRUE(sut.getCurrentOrientation(currOrientation));
    ASSERT_FLOAT_EQ(currOrientation.roll, poseEventData.roll);
    ASSERT_FLOAT_EQ(currOrientation.pitch, poseEventData.pitch);
    ASSERT_FLOAT_EQ(currOrientation.yaw, poseEventData.yaw);

    kpsr::bst::WaypointCommandMessage waypointComand;
    waypointComand.plan.resize(6);

    waypointComand.plan[0].num = 80;
    waypointComand.plan[0].next = 81;
    waypointComand.plan[0].latitude = 40.021801;  // [deg]
    waypointComand.plan[0].longitude = -105.21730804;  // [deg]
    waypointComand.plan[0].altitude = 1635.020752;  // [m]
    waypointComand.plan[0].radius = 0.0;  // [m]

    waypointComand.plan[1].num = 81;
    waypointComand.plan[1].next = 82;
    waypointComand.plan[1].latitude = 40.021936;  // [deg]
    waypointComand.plan[1].longitude = -105.217308;  // [deg]
    waypointComand.plan[1].altitude = 1635.020752;  // [m]
    waypointComand.plan[1].radius = 0.0;  // [m]

    waypointComand.plan[2].num = 82;
    waypointComand.plan[2].next = 83;
    waypointComand.plan[2].latitude = 40.021936;  // [deg]
    waypointComand.plan[2].longitude = -105.217308;  // [deg]
    waypointComand.plan[2].altitude = 1640.020752;  // [m]
    waypointComand.plan[2].radius = 0.0;  // [m]

    waypointComand.plan[3].num = 83;
    waypointComand.plan[3].next = 84;
    waypointComand.plan[3].latitude = 40.021981;  // [deg]
    waypointComand.plan[3].longitude = -105.217249;  // [deg]
    waypointComand.plan[3].altitude = 1640.020752;  // [m]
    waypointComand.plan[3].radius = 0.0;  // [m]

    waypointComand.plan[4].num = 84;
    waypointComand.plan[4].next = 85;
    waypointComand.plan[4].latitude = 40.022026;  // [deg]
    waypointComand.plan[4].longitude = -105.217191;  // [deg]
    waypointComand.plan[4].altitude = 1640.020752;  // [m]
    waypointComand.plan[4].radius = 0.0;  // [m]

    waypointComand.plan[5].num = 85;
    waypointComand.plan[5].next = 80;
    waypointComand.plan[5].latitude = 40.021981;  // [deg]
    waypointComand.plan[5].longitude = -105.217249;  // [deg]
    waypointComand.plan[5].altitude = 1640.020752;  // [m]
    waypointComand.plan[5].radius = 0.0;  // [m]

    waypointComand.mode = ADD;

    bstWaypointComandsProvider.getPublisher()->publish(waypointComand);

    ASSERT_FALSE(sut.getCommandedGpsPosition(cmdGpsPosition));

    // First, we send an invalid waypoint id
    kpsr::bst::BstRequestMessage message;
    message.id = CONTROL_COMMAND;
    message.type = CMD_WAYPOINT;
    message.value = 1000;

    bstBstRequestMessagesProvider.getPublisher()->publish(message);

    ASSERT_FALSE(sut.getCommandedGpsPosition(cmdGpsPosition));

    // Now, we send a valid waypoint id
    message.value = 80;
    bstBstRequestMessagesProvider.getPublisher()->publish(message);

    ASSERT_TRUE(sut.getCommandedGpsPosition(cmdGpsPosition));
    ASSERT_FLOAT_EQ(cmdGpsPosition.latitude, waypointComand.plan[0].latitude);
    ASSERT_FLOAT_EQ(cmdGpsPosition.longitude, waypointComand.plan[0].longitude);
    ASSERT_FLOAT_EQ(cmdGpsPosition.altitude, waypointComand.plan[0].altitude);

    ASSERT_TRUE(sut.getCommandedGpsPosition(cmdGpsPosition));
    ASSERT_FLOAT_EQ(cmdGpsPosition.latitude, waypointComand.plan[0].latitude);
    ASSERT_FLOAT_EQ(cmdGpsPosition.longitude, waypointComand.plan[0].longitude);
    ASSERT_FLOAT_EQ(cmdGpsPosition.altitude, waypointComand.plan[0].altitude);
}
