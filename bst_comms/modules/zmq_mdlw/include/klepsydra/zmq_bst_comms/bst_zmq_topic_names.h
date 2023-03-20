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

#ifndef BST_ZMQ_TOPIC_NAMES_H
#define BST_ZMQ_TOPIC_NAMES_H

#include <string>

namespace kpsr {
namespace bst {
namespace zmq_mdlw {
static const std::string ZMQ_TOPIC_NAME_BST_REQUEST = "BST_REQUEST";
static const std::string ZMQ_TOPIC_NAME_BST_REPLY = "BST_REPLY";
static const std::string ZMQ_TOPIC_NAME_BST_WP_CMD = "BST_WP_CMD";
static const std::string ZMQ_TOPIC_NAME_POSE_EVENT = "POSE_EVENT";
static const std::string ZMQ_TOPIC_NAME_SYSTEM_INIT = "SYSTEM_INIT";
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_POS = "TELEMETRY_POS";
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_ORI = "TELEMETRY_ORI";
static const std::string ZMQ_TOPIC_NAME_SENSOR = "SENSOR";
static const std::string ZMQ_TOPIC_NAME_CALIBRATE = "CALIBRATE";
static const std::string ZMQ_TOPIC_NAME_CONTROL_CMD = "CONTROL_CMD";
static const std::string ZMQ_TOPIC_NAME_CONTROL_PID = "CONTROL_PID";
static const std::string ZMQ_TOPIC_NAME_SYSTEM = "SYSTEM";
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_SYS = "TELEMETRY_SYS";
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_PRES = "TELEMETRY_PRES";
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_CTRL = "TELEMETRY_CTRL";
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_GCS = "TELEMETRY_GCS";
static const std::string ZMQ_TOPIC_NAME_PAYLOAD_CONTROL = "PAYLOAD_CONTROL";
} // namespace zmq_mdlw
} // namespace bst
} // namespace kpsr

#endif // BST_ZMQ_TOPIC_NAMES_H
