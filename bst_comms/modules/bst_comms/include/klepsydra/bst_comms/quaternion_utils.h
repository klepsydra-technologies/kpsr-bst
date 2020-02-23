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
#ifndef QUATERNION_UTILS_H
#define QUATERNION_UTILS_H

#include <cmath>

namespace kpsr {
namespace bst {
/**
 * @brief The QuaternionUtils class
 *
 * @copyright Klepsydra Robotics 2017-2018.
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
