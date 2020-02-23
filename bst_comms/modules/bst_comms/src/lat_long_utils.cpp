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
#include<iomanip>
#include<cmath>

#include <klepsydra/bst_comms/lat_long_utils.h>

double kpsr::bst::LatLongUtils::degreeToRadian (const double degree) {
    return (degree * PI / 180);
}

double kpsr::bst::LatLongUtils::radianToDegree (const double radian) {
    return (radian * 180 / PI);
}

double kpsr::bst::LatLongUtils::CoordinatesToAngle (double latitude1, const double longitude1, double latitude2, const double longitude2) {
  const double longitudeDifference = degreeToRadian(longitude2 - longitude1);
  latitude1 = degreeToRadian(latitude1);
  latitude2 = degreeToRadian(latitude2);

  const double x = (std::cos(latitude1) * std::sin(latitude2)) - (std::sin(latitude1) * std::cos(latitude2) * std::cos(longitudeDifference));
  const double y = std::sin(longitudeDifference) * std::cos(latitude2);

  const double degree = radianToDegree(atan2(y, x));
  return (degree >= 0)? degree : (degree + 360);
}

double kpsr::bst::LatLongUtils::CoordinatesToMeters (double latitude1, double longitude1, double latitude2, double longitude2) {
  latitude1 = degreeToRadian(latitude1);
  longitude1 = degreeToRadian(longitude1);
  latitude2 = degreeToRadian(latitude2);
  longitude2 = degreeToRadian(longitude2);

  double x = std::sin((latitude2 - latitude1) / 2), y = std::sin((longitude2 - longitude1) / 2);
  return earthDiameterMeters * std::asin(sqrt((x * x) + (std::cos(latitude1) * std::cos(latitude2) * y * y)));
}

