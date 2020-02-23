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
#ifndef LAT_LONG_UTILS_H
#define LAT_LONG_UTILS_H

namespace kpsr
{
namespace bst
{
static const double PI = 3.14159265358979323846, earthDiameterMeters = 6371.0 * 2 * 1000;
/**
 * @brief The LatLongUtils class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details GPS to meters facility.
 */
class LatLongUtils
{
public:

    /**
     * @brief CoordinatesToAngle
     * @param latitude1
     * @param longitude1
     * @param latitude2
     * @param longitude2
     * @return
     */
    static double CoordinatesToAngle(double latitude1, const double longitude1, double latitude2, const double longitude2);

    /**
     * @brief CoordinatesToMeters
     * @param latitude1
     * @param longitude1
     * @param latitude2
     * @param longitude2
     * @return
     */
    static double CoordinatesToMeters(double latitude1, double longitude1, double latitude2, double longitude2);

private:
    static double degreeToRadian (const double degree);
    static double radianToDegree (const double radian);
};
}
}
#endif // LAT_LONG_UTILS_H
