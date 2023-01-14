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

#ifndef BST_DDS_TOPIC_NAMES_H
#define BST_DDS_TOPIC_NAMES_H

#include <string>

namespace kpsr
{
namespace bst
{
namespace dds_mdlw
{
static const std::string DDS_TOPIC_NAME_BST_REQUEST = "kpsr_bst_request";
static const std::string DDS_TOPIC_NAME_BST_REPLY = "kpsr_bst_reply";
static const std::string DDS_TOPIC_NAME_BST_WP_CMD = "kpsr_bst_waypoint_cmd";
static const std::string DDS_TOPIC_NAME_POSE_EVENT = "kpsr_bst_pose_event";
static const std::string DDS_TOPIC_NAME_SYSTEM_INIT = "kpsr_bst_system_init";
static const std::string DDS_TOPIC_NAME_TELEMETRY_POS = "kpsr_bst_telemetry_position";
static const std::string DDS_TOPIC_NAME_TELEMETRY_ORI = "kpsr_bst_telemtry_orientation";
static const std::string DDS_TOPIC_NAME_SENSOR = "kpsr_bst_sensor";
static const std::string DDS_TOPIC_NAME_CALIBRATE = "kpsr_bst_calibrate";
static const std::string DDS_TOPIC_NAME_CONTROL_CMD = "kpsr_bst_control_cmd";
static const std::string DDS_TOPIC_NAME_CONTROL_PID = "kpsr_bst_control_pid";
static const std::string DDS_TOPIC_NAME_SYSTEM = "kpsr_bst_system";
static const std::string DDS_TOPIC_NAME_TELEMETRY_SYS = "kpsr_bst_telemetry_system";
static const std::string DDS_TOPIC_NAME_TELEMETRY_PRES = "kpsr_bst_telemetry_pressure";
static const std::string DDS_TOPIC_NAME_TELEMETRY_CTRL = "kpsr_bst_telemetry_control";
static const std::string DDS_TOPIC_NAME_TELEMETRY_GCS = "kpsr_bst_telemetry_gcs";
static const std::string DDS_TOPIC_NAME_PAYLOAD_CONTROL = "kpsr_bst_payload_control";
}
}
}

#endif // BST_DDS_TOPIC_NAMES_H
