// Copyright 2023 Klepsydra Technologies AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

