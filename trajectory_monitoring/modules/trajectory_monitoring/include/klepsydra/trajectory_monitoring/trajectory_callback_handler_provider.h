
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

#ifndef TRAJECTORY_CALLBACK_HANDLER_PROVIDER_H
#define TRAJECTORY_CALLBACK_HANDLER_PROVIDER_H

#include <string>
#include <memory>

#include <klepsydra/core/callback_handler.h>

#include <klepsydra/trajectory_monitoring/trajectory_monitoring_request.h>
#include <klepsydra/trajectory_monitoring/trajectory_monitoring_response.h>

namespace kpsr {
namespace trajectory {
/**
 * @brief The TrajectoryCallbackHandlerProvider class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-trajectory-monitoring-rest
 *
 * @details Callback handler of trajectory request reply messages.
 */
class TrajectoryCallbackHandlerProvider {

public:
    /**
     * @brief TrajectoryCallbackHandlerProvider
     * @param trajectoryRequestPublisher
     * @param trajectoryResponseSubscriber
     */
    TrajectoryCallbackHandlerProvider(Publisher<TrajectoryMonitoringRequest> * trajectoryRequestPublisher,
                                      Subscriber<TrajectoryMonitoringResponse> * trajectoryResponseSubscriber,
                                      bool threadSafe)
        : _callbackHandler(threadSafe ?
                               new kpsr::MultiThreadCallbackHandler<TrajectoryMonitoringRequest, TrajectoryMonitoringResponse>(
                               "trajectory_callback_handler", trajectoryRequestPublisher, trajectoryResponseSubscriber,
                               std::bind(&TrajectoryCallbackHandlerProvider::requestReplyCorrelation, this,
                                         std::placeholders::_1, std::placeholders::_2)):
                           new kpsr::CallbackHandler<TrajectoryMonitoringRequest, TrajectoryMonitoringResponse>(
                               "trajectory_callback_handler", trajectoryRequestPublisher, trajectoryResponseSubscriber,
                               std::bind(&TrajectoryCallbackHandlerProvider::requestReplyCorrelation, this,
                                         std::placeholders::_1, std::placeholders::_2)))
    {}

    kpsr::CallbackHandler<TrajectoryMonitoringRequest, TrajectoryMonitoringResponse> * getCallbackHandler() {
        return _callbackHandler.get();
    }

private:
    std::unique_ptr<kpsr::CallbackHandler<TrajectoryMonitoringRequest, TrajectoryMonitoringResponse>> _callbackHandler;

    bool requestReplyCorrelation(const kpsr::trajectory::TrajectoryMonitoringRequest & request,
                                 const kpsr::trajectory::TrajectoryMonitoringResponse & reply) {
        return ((request.id == reply.id) && (request.type == reply.type));
    }
};
}
}

#endif // TRAJECTORY_CALLBACK_HANDLER_PROVIDER_H

