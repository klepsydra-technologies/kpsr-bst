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
static const std::string ZMQ_TOPIC_NAME_BST_REQUEST = ZMQ_TOPIC_NAME_BST_REQUEST;
static const std::string ZMQ_TOPIC_NAME_BST_REPLY = ZMQ_TOPIC_NAME_BST_REPLY;
static const std::string ZMQ_TOPIC_NAME_BST_WP_CMD = ZMQ_TOPIC_NAME_BST_WP_CMD;
static const std::string ZMQ_TOPIC_NAME_POSE_EVENT = ZMQ_TOPIC_NAME_POSE_EVENT;
static const std::string ZMQ_TOPIC_NAME_SYSTEM_INIT = ZMQ_TOPIC_NAME_SYSTEM_INIT;
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_POS = ZMQ_TOPIC_NAME_TELEMETRY_POS;
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_ORI = ZMQ_TOPIC_NAME_TELEMETRY_ORI;
static const std::string ZMQ_TOPIC_NAME_SENSOR = ZMQ_TOPIC_NAME_SENSOR;
static const std::string ZMQ_TOPIC_NAME_CALIBRATE = ZMQ_TOPIC_NAME_CALIBRATE;
static const std::string ZMQ_TOPIC_NAME_CONTROL_CMD = ZMQ_TOPIC_NAME_CONTROL_CMD;
static const std::string ZMQ_TOPIC_NAME_CONTROL_PID = ZMQ_TOPIC_NAME_CONTROL_PID;
static const std::string ZMQ_TOPIC_NAME_SYSTEM = ZMQ_TOPIC_NAME_SYSTEM;
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_SYS = ZMQ_TOPIC_NAME_TELEMETRY_SYS;
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_PRES = ZMQ_TOPIC_NAME_TELEMETRY_PRES;
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_CTRL = ZMQ_TOPIC_NAME_TELEMETRY_CTRL;
static const std::string ZMQ_TOPIC_NAME_TELEMETRY_GCS = ZMQ_TOPIC_NAME_TELEMETRY_GCS;
static const std::string ZMQ_TOPIC_NAME_PAYLOAD_CONTROL = ZMQ_TOPIC_NAME_PAYLOAD_CONTROL;
}
}
}

#endif // BST_ZMQ_TOPIC_NAMES_H
