
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

#ifndef TRAJECTORY_CONTAINER_H
#define TRAJECTORY_CONTAINER_H

#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/core/service.h>

#include <klepsydra/trajectory_monitoring/trajectory_monitoring_request.h>
#include <klepsydra/trajectory_monitoring/trajectory_monitoring_response.h>

#include <klepsydra/trajectory_monitoring/trajectory_data_provider.h>

namespace kpsr {
namespace trajectory {
/**
 * @brief The TrajectoryContainer class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-trajectory-monitoring
 *
 * @details TrajectoryMonitoring implementation of the container. It handles trajectory requests async.
 */
class TrajectoryContainer : public Service {
public:

    /**
     * @brief TrajectoryContainer
     * @param env
     * @param serviceName
     * @param trajectoryRequestSubscriber
     * @param trajectoryResponsePublisher
     */
    TrajectoryContainer(Environment * env, std::string serviceName,
                        TrajectoryDataProvider * _trajectoryDataProvider,
                        Subscriber<TrajectoryMonitoringRequest> * trajectoryRequestSubscriber,
                        Publisher<TrajectoryMonitoringResponse> * trajectoryResponsePublisher);

    /**
     * @brief start
     */
    void start() override;

    /**
     * @brief stop
     */
    void stop() override;

    void execute() override {}

private:
    void onTrajectoryRequest(const TrajectoryMonitoringRequest & request);

    TrajectoryDataProvider * _trajectoryDataProvider;
    Subscriber<TrajectoryMonitoringRequest> * _trajectoryRequestSubscriber;
    Publisher<TrajectoryMonitoringResponse> * _trajectoryResponsePublisher;
};
}
}

#endif // TRAJECTORY_CONTAINER_H
