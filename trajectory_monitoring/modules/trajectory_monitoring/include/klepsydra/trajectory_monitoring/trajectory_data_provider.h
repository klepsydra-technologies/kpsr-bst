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


#ifndef TRAJECTORY_DATA_PROVIDER_H
#define TRAJECTORY_DATA_PROVIDER_H

#include <klepsydra/trajectory_monitoring/position.h>
#include <klepsydra/trajectory_monitoring/gps_position.h>
#include <klepsydra/trajectory_monitoring/orientation.h>
#include <klepsydra/trajectory_monitoring/linear_velocity.h>
#include <klepsydra/trajectory_monitoring/angular_rates.h>

namespace kpsr {
namespace trajectory {
class TrajectoryDataProvider
{
public:
    virtual bool getCurrentPosition(Position & currentPosition) = 0;
    virtual bool getCurrentGpsPosition(GpsPosition & currentGpsPosition) = 0;
    virtual bool getCurrentOrientation(Orientation & currentOrientation) = 0;
    virtual bool getCurrentLinearVelocity(LinearVelocity & currentLinearVelocity) = 0;
    virtual bool getCurrentAngularRates(AngularRates & currentAngularRates) = 0;

    virtual bool getCommandedPosition(Position & commandedPosition) = 0;
    virtual bool getCommandedGpsPosition(GpsPosition & commandedGpsPosition) = 0;
    virtual bool getCommandedOrientation(Orientation & commandedOrientation) = 0;
    virtual bool getCommandedLinearVelocity(LinearVelocity & commandedLinearVelocity) = 0;
    virtual bool getCommandedAngularRates(AngularRates & commandedAngularRates) = 0;
};
}
}

#endif // TRAJECTORY_DATA_PROVIDER_H
