
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

#ifndef EL_REST_TRAJ_MON_CONTAINER_PROVIDER_H
#define EL_REST_TRAJ_MON_CONTAINER_PROVIDER_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/trajectory_monitoring/rest_trajectory_monitoring_interface.h>
#include <klepsydra/trajectory_monitoring/trajectory_container.h>

#include <klepsydra/mem_trajectory_monitoring/el_traj_mon_comms_provider.h>

namespace kpsr {
namespace trajectory {
namespace restapi {
template <std::size_t BufferSize>
/**
 * @brief The EventLoopRestTrajectoryContainerProvider class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-mem-trajectory
 *
 * @details Facility class to create an eventloop based implemtation of the trajectory container and rest server.
 * Example of use:
@code
    kpsr::high_performance::EventLoopMiddlewareProvider<32> eventloopProvider(nullptr);
    kpsr::trajectory::restapi::EventLoopRestTrajectoryContainerProvider<32> trajectoryProvider(eventloopProvider, 1, 9090, &environment, "sample1");

    // Given a service "positionService" we attach it to the container
    trajectoryProvider.getContainer().attach(&positionService);
    eventloopProvider.start();
    trajectoryProvider.start();
@endcode
 */
class EventLoopRestTrajectoryContainerProvider
{
public:
    /**
     * @brief EventLoopRestTrajectoryContainerProvider
     * @param eventloopProvider
     * @param noThreads
     * @param port
     * @param env
     * @param applicationName
     */
    EventLoopRestTrajectoryContainerProvider(kpsr::restapi::RestEndpoint & restEndpoint,
                                             kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & eventloopProvider,
                                             TrajectoryDataProvider * _trajectoryDataProvider,
                                             Environment * env, std::string applicationName)
        : _restEndpoint(restEndpoint)
        , _trajectoryCommsProvider(eventloopProvider)
        , _restTrajMonInterface(_trajectoryCommsProvider.trajectoryCallbackHandlerProvider.getCallbackHandler())
        , _container(env, applicationName, _trajectoryDataProvider,
                     _trajectoryCommsProvider.trajectoryRequestSubscriber,
                     _trajectoryCommsProvider.trajectoryResponsePublisher)
    {
        _restEndpoint.attachApplication(&_restTrajMonInterface);
    }

    /**
     * @brief getContainer
     * @return
     */
    kpsr::trajectory::TrajectoryContainer & getContainer() {
        return _container;
    }

    /**
     * @brief start
     */
    void start() {
        _container.start();
    }

    /**
     * @brief stop
     */
    void stop() {
        _container.stop();
    }

private:
    kpsr::restapi::RestEndpoint & _restEndpoint;
    kpsr::trajectory::mem::EventLoopTrajectoryCommsProvider<BufferSize> _trajectoryCommsProvider;
    kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface _restTrajMonInterface;
    kpsr::trajectory::TrajectoryContainer _container;
};
}
}
}

#endif // EL_REST_CONTAINER_PROVIDER_H
