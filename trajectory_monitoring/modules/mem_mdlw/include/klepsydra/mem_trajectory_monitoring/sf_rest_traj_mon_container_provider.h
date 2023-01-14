
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
 * @copyright 2023 Klepsydra Technologies AG
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
