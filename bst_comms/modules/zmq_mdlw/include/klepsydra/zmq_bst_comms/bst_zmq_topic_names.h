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
#ifndef BST_ZMQ_TOPIC_NAMES_H
#define BST_ZMQ_TOPIC_NAMES_H

#include <string>

namespace kpsr
{
namespace bst
{
namespace zmq_mdlw
{
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
}
}
}

#endif // BST_ZMQ_TOPIC_NAMES_H
