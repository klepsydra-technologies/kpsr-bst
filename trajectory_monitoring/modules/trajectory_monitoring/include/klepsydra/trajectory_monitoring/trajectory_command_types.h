
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

#ifndef TRAJECTORY_COMMAND_TYPES_H
#define TRAJECTORY_COMMAND_TYPES_H

namespace kpsr {
namespace trajectory {
/**
 * @brief The TrajectoryCommandType enum
 *
 * @copyright 2023 Klepsydra Technologies AG
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
