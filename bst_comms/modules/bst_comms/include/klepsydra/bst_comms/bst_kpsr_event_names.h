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
