
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
 * @copyright 2023 Klepsydra Technologies AG
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
