/*****************************************************************************
*
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

#ifndef TRAJECTORY_COMMAND_TYPES_H
#define TRAJECTORY_COMMAND_TYPES_H

namespace kpsr {
namespace trajectory {
/**
 * @brief The TrajectoryCommandType enum
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-trajectory-monitoring
 *
 * @details List of commands that a Klepsydra trajectory monitoring can accept.
 */
enum TrajectoryCommandType {
    GET_CURRENT_POSITION = 0,
    GET_CURRENT_GPS_POSITION,
    GET_CURRENT_ORIENTATION,
    GET_CURRENT_LINEAR_VELOCITY,
    GET_CURRENT_ANGULAR_RATES,
    GET_COMMANDED_POSITION,
    GET_COMMANDED_GPS_POSITION,
    GET_COMMANDED_ORIENTATION,
    GET_COMMANDED_LINEAR_VELOCITY,
    GET_COMMANDED_ANGULAR_RATES
};
}
}

#endif // TRAJECTORY_COMMAND_TYPES_H
