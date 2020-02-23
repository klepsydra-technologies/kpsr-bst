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
 * @copyright Klepsydra Robotics 2017-2018.
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
