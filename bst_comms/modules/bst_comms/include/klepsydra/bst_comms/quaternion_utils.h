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

#ifndef QUATERNION_UTILS_H
#define QUATERNION_UTILS_H

#include <cmath>

namespace kpsr {
namespace bst {
/**
 * @brief The QuaternionUtils class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Quaternion to Euler calculation utils.
 */
class QuaternionUtils {
public:
    /**
     * @brief  Convert quaternion to roll angle
     * @param  data pointer to quaternion
     * @retval roll angle in radians
     */
    static float quat_to_roll(const float V[4]) {
        return std::atan2( 2*(V[2]*V[3] + V[0]*V[1]), V[0]*V[0] - V[1]*V[1] - V[2]*V[2] + V[3]*V[3] );
    }

    /**
     * @brief  Convert quaternion to pitch angle
     * @param  data pointer to quaternion
     * @retval pitch angle in radians
     */
    static float quat_to_pitch(const float V[4]) {
        return std::asin( -2*(V[1]*V[3] - V[0]*V[2]) );
    }

    /**
     * @brief  Convert quaternion to yaw angle
     * @param  data pointer to quaternion
     * @retval yaw angle in radians
     */
    static float quat_to_yaw(const float V[4]) {
        return std::atan2( 2*(V[1]*V[2] + V[0]*V[3]), V[0]*V[0] + V[1]*V[1] - V[2]*V[2] - V[3]*V[3] );
    }

#endif // QUATERNION_UTILS_H

};
}
}
