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

#ifndef LAT_LONG_UTILS_H
#define LAT_LONG_UTILS_H

namespace kpsr {
namespace bst {
static const double PI = 3.14159265358979323846, earthDiameterMeters = 6371.0 * 2 * 1000;
/**
 * @brief The LatLongUtils class
 *
 * @copyright 2023 Klepsydra Technologies AG
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
    static double CoordinatesToAngle(double latitude1,
                                     const double longitude1,
                                     double latitude2,
                                     const double longitude2);

    /**
     * @brief CoordinatesToMeters
     * @param latitude1
     * @param longitude1
     * @param latitude2
     * @param longitude2
     * @return
     */
    static double CoordinatesToMeters(double latitude1,
                                      double longitude1,
                                      double latitude2,
                                      double longitude2);

private:
    static double degreeToRadian(const double degree);
    static double radianToDegree(const double radian);
};
} // namespace bst
} // namespace kpsr
#endif // LAT_LONG_UTILS_H
