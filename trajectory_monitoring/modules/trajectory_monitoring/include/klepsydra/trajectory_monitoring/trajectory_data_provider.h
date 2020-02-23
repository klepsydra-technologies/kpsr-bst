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
