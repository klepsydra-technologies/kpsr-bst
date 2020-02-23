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
