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

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "gtest/gtest.h"

#include <klepsydra/core/event_emitter_middleware_provider.h>
#include <klepsydra/core/cache_listener.h>

#include <klepsydra/mem_core/mem_env.h>

#include <klepsydra/trajectory_monitoring/bst_trajectory_data_provider.h>
#include <klepsydra/trajectory_monitoring/trajectory_container.h>

TEST(TrajectoryContainerTest, AvailableDataLeadsToMessageResponse) {

    spdlog::info("TrajectoryContainerTest");
    kpsr::mem::MemEnv environment;

    kpsr::EventEmitterMiddlewareProvider<::bst::comms::TelemetryPosition_t> bstTelemetryPositionProvider(nullptr, "bst_telemetry_position", 0, nullptr, nullptr);
    kpsr::EventEmitterMiddlewareProvider<::bst::comms::TelemetryOrientation_t> bstTelemetryOrientationProvider(nullptr, "bst_telemetry_orientation", 0, nullptr, nullptr);
    kpsr::EventEmitterMiddlewareProvider<kpsr::geometry::PoseEventData> bstPositionAndOrientationProvider(nullptr, "bst_kpsr_pose", 0, nullptr, nullptr);
    kpsr::EventEmitterMiddlewareProvider<kpsr::bst::WaypointCommandMessage> bstWaypointComandsProvider(nullptr, "bst_waypoint_command", 0, nullptr, nullptr);
    kpsr::EventEmitterMiddlewareProvider<kpsr::bst::BstRequestMessage> bstBstRequestMessagesProvider(nullptr, "bst_request", 0, nullptr, nullptr);

    kpsr::trajectory::bst::BstTrajectoryDataProvider trajectoryDataProvider(&environment,
                                                                            bstTelemetryPositionProvider.getSubscriber(),
                                                                            bstTelemetryOrientationProvider.getSubscriber(),
                                                                            bstPositionAndOrientationProvider.getSubscriber(),
                                                                            bstWaypointComandsProvider.getSubscriber(),
                                                                            bstBstRequestMessagesProvider.getSubscriber(),
                                                                            false);

    kpsr::EventEmitterMiddlewareProvider<kpsr::trajectory::TrajectoryMonitoringRequest> trajectoryMonitoringRequestProvider(nullptr, "bst_request", 0, nullptr, nullptr);
    kpsr::EventEmitterMiddlewareProvider<kpsr::trajectory::TrajectoryMonitoringResponse> trajectoryMonitoringResponseProvider(nullptr, "bst_response", 0, nullptr, nullptr);

    kpsr::trajectory::TrajectoryContainer sut(&environment, "TEST", &trajectoryDataProvider,
                                              trajectoryMonitoringRequestProvider.getSubscriber(),
                                              trajectoryMonitoringResponseProvider.getPublisher());
    trajectoryDataProvider.start();
    sut.start();

    kpsr::mem::CacheListener<kpsr::trajectory::TrajectoryMonitoringResponse> responseListener;
    trajectoryMonitoringResponseProvider.getSubscriber()->registerListener("TEST", responseListener.cacheListenerFunction);

    int id = 0;
    kpsr::trajectory::TrajectoryMonitoringRequest request;

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_ANGULAR_RATES;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_GPS_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_LINEAR_VELOCITY;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_ORIENTATION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_ANGULAR_RATES;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_GPS_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_LINEAR_VELOCITY;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_ORIENTATION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    ::bst::comms::TelemetryPosition_t telemetryPosition;
    telemetryPosition.latitude = 1.0;
    telemetryPosition.longitude = 2.0;
    telemetryPosition.altitude = 100.0;
    telemetryPosition.velocity.x = 10.0;
    telemetryPosition.velocity.y = 20.0;
    telemetryPosition.velocity.z = 30.0;
    bstTelemetryPositionProvider.getPublisher()->publish(telemetryPosition);

    // First trial gets data.
    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_GPS_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->gpsPosition.latitude, telemetryPosition.latitude);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->gpsPosition.longitude, telemetryPosition.longitude);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->gpsPosition.altitude, telemetryPosition.altitude);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_LINEAR_VELOCITY;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->linearVelocity.u, telemetryPosition.velocity.x);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->linearVelocity.v, telemetryPosition.velocity.y);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->linearVelocity.w, telemetryPosition.velocity.z);

    // Second does not.
    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_GPS_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_LINEAR_VELOCITY;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);

    ::bst::comms::TelemetryOrientation_t telemetryOrientation;
    telemetryOrientation.omega.x = 0.1;
    telemetryOrientation.omega.y = 0.2;
    telemetryOrientation.omega.z = 0.3;
    bstTelemetryOrientationProvider.getPublisher()->publish(telemetryOrientation);

    // First trial gets data.
    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_ANGULAR_RATES;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->angularRates.p, telemetryOrientation.omega.x);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->angularRates.q, telemetryOrientation.omega.y);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->angularRates.r, telemetryOrientation.omega.z);

    // Second does not.
    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_ANGULAR_RATES;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);

    kpsr::geometry::PoseEventData poseEventData;
    poseEventData.x = 50;
    poseEventData.y = 60;
    poseEventData.z = 70;
    poseEventData.roll = 0.5;
    poseEventData.pitch = 0.6;
    poseEventData.yaw = 0.7;
    bstPositionAndOrientationProvider.getPublisher()->publish(poseEventData);

//    // First trial gets data.
    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->position.x, poseEventData.x);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->position.y, poseEventData.y);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->position.z, poseEventData.z);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_ORIENTATION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->orientation.roll, poseEventData.roll);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->orientation.pitch, poseEventData.pitch);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->orientation.yaw, poseEventData.yaw);

//    // Second does not.
    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_ORIENTATION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);

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

    // First, we send an invalid waypoint id
    kpsr::bst::BstRequestMessage message;
    message.id = CONTROL_COMMAND;
    message.type = CMD_WAYPOINT;
    message.value = 1000;

    bstBstRequestMessagesProvider.getPublisher()->publish(message);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_GPS_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_FALSE(responseListener.getLastReceivedEvent()->result);

    // Now, we send a valid waypoint id
    message.value = 80;
    bstBstRequestMessagesProvider.getPublisher()->publish(message);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_GPS_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->gpsPosition.latitude, waypointComand.plan[0].latitude);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->gpsPosition.longitude, waypointComand.plan[0].longitude);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->gpsPosition.altitude, waypointComand.plan[0].altitude);

    request.id = id++;
    request.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_GPS_POSITION;
    trajectoryMonitoringRequestProvider.getPublisher()->publish(request);
    ASSERT_TRUE(responseListener.getLastReceivedEvent()->result);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->gpsPosition.latitude, waypointComand.plan[0].latitude);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->gpsPosition.longitude, waypointComand.plan[0].longitude);
    ASSERT_FLOAT_EQ(responseListener.getLastReceivedEvent()->gpsPosition.altitude, waypointComand.plan[0].altitude);
}
