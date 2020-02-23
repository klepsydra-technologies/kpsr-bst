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

#ifndef SF_REST_TRAJECTORY_CONTAINER_PROVIDER_H
#define SF_REST_TRAJECTORY_CONTAINER_PROVIDER_H

#include <klepsydra/trajectory_monitoring/rest_trajectory_monitoring_interface.h>
#include <klepsydra/trajectory_monitoring/trajectory_container.h>

#include <klepsydra/mem_trajectory_monitoring/sf_traj_mon_comms_provider.h>

namespace kpsr {
namespace trajectory {
namespace restapi {
/**
 * @brief The BasicRestTrajectoryContainerProvider class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-mem-trajectory
 *
 * @details Facility class to create a safequeue based implemtation of the trajectory container and rest server.
 */
class BasicRestTrajectoryContainerProvider
{
public:
    /**
     * @brief BasicRestTrajectoryContainerProvider
     * @param noThreads
     * @param port
     * @param env
     * @param applicationName
     */
    BasicRestTrajectoryContainerProvider(kpsr::restapi::RestEndpoint & restEndpoint,
                                             kpsr::trajectory::TrajectoryDataProvider * trajectoryDataProvider,
                                             Environment * env, std::string applicationName);

    /**
     * @brief getContainer
     * @return
     */
    kpsr::trajectory::TrajectoryContainer & getContainer();

    /**
     * @brief start
     */
    void start();

    /**
     * @brief stop
     */
    void stop();

private:
    kpsr::restapi::RestEndpoint & _restEndpoint;
    kpsr::trajectory::mem::BasicTrajectoryCommsProvider _trajectoryCommsProvider;
    kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface _restTrajMonInterface;
    kpsr::trajectory::TrajectoryContainer _container;
};
}
}
}

#endif // SF_REST_TRAJECTORY_CONTAINER_PROVIDER_H
