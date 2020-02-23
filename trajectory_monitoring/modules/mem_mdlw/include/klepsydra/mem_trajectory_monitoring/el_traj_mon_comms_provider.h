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

#ifndef EVENT_LOOP_TRAJECTORY_COMM_PROVIDER_H
#define EVENT_LOOP_TRAJECTORY_COMM_PROVIDER_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/trajectory_monitoring/trajectory_monitoring_request.h>
#include <klepsydra/trajectory_monitoring/trajectory_monitoring_response.h>

#include <klepsydra/trajectory_monitoring/trajectory_callback_handler_provider.h>

namespace kpsr {
namespace trajectory {
namespace mem {
template<std::size_t BufferSize>
/**
 * @brief The EventLoopTrajectoryCommsProvider class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-mem-trajectory
 *
 * @details Eventloop based implementation of the trajectory request and reply queues.
 */
class EventLoopTrajectoryCommsProvider
{
public:
    /**
     * @brief EventLoopTrajectoryCommsProvider
     * @param eventloopProvider
     */
    EventLoopTrajectoryCommsProvider(kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & eventloopProvider)
        : trajectoryRequestSubscriber(eventloopProvider.template getSubscriber<TrajectoryMonitoringRequest>("trajectory_request"))
        , trajectoryRequestPublisher(eventloopProvider.template getPublisher<TrajectoryMonitoringRequest>("trajectory_request", 0, nullptr, nullptr))
        , trajectoryResponseSubscriber(eventloopProvider.template getSubscriber<TrajectoryMonitoringResponse>("trajectory_response"))
        , trajectoryResponsePublisher(eventloopProvider.template getPublisher<TrajectoryMonitoringResponse>("trajectory_response", 0, nullptr, nullptr))
        , trajectoryCallbackHandlerProvider(trajectoryRequestPublisher, trajectoryResponseSubscriber, true)
    {}

    /**
     * @brief trajectoryRequestSubscriber
     */
    kpsr::Subscriber<TrajectoryMonitoringRequest> * trajectoryRequestSubscriber;

    /**
     * @brief trajectoryRequestPublisher
     */
    kpsr::Publisher<TrajectoryMonitoringRequest> * trajectoryRequestPublisher;

    /**
     * @brief trajectoryResponseSubscriber
     */
    kpsr::Subscriber<TrajectoryMonitoringResponse> * trajectoryResponseSubscriber;

    /**
     * @brief trajectoryResponsePublisher
     */
    kpsr::Publisher<TrajectoryMonitoringResponse> *trajectoryResponsePublisher;

    /**
     * @brief trajectoryCallbackHandlerProvider
     */
    TrajectoryCallbackHandlerProvider trajectoryCallbackHandlerProvider;
};
}
}
}

#endif // EVENT_LOOP_TRAJECTORY_COMM_PROVIDER_H
