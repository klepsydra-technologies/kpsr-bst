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

#ifndef BST_KPSR_EVENT_NAMES_H
#define BST_KPSR_EVENT_NAMES_H

#include <string>

namespace kpsr
{
namespace bst
{
static const std::string EVENT_NAME_BST_REQUEST = "bst_request";
static const std::string EVENT_NAME_BST_REPLY = "bst_reply";
static const std::string EVENT_NAME_BST_WP_CMD = "bst_waypoint_cmd";
static const std::string EVENT_NAME_POSE_EVENT = "bst_pose_event";
static const std::string EVENT_NAME_SYSTEM_INIT = "bst_system_init";
static const std::string EVENT_NAME_TELEMETRY_POS = "bst_telemetry_position";
static const std::string EVENT_NAME_TELEMETRY_ORI = "bst_telemtry_orientation";
static const std::string EVENT_NAME_SENSOR = "bst_sensor";
static const std::string EVENT_NAME_CALIBRATE = "bst_calibrate";
static const std::string EVENT_NAME_CONTROL_CMD = "bst_control_cmd";
static const std::string EVENT_NAME_CONTROL_PID = "bst_control_pid";
static const std::string EVENT_NAME_SYSTEM = "bst_system";
static const std::string EVENT_NAME_TELEMETRY_SYS = "bst_telemetry_system";
static const std::string EVENT_NAME_TELEMETRY_PRES = "bst_telemetry_pressure";
static const std::string EVENT_NAME_TELEMETRY_CTRL = "bst_telemetry_control";
static const std::string EVENT_NAME_TELEMETRY_GCS = "bst_telemetry_gcs";
static const std::string EVENT_NAME_PAYLOAD_CONTROL = "bst_payload_control";
}
}

#endif // BST_KPSR_EVENT_NAMES_H
