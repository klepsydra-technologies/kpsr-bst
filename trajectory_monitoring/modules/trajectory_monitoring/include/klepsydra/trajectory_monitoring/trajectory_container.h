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
 * @copyright Klepsydra Robotics 2017-2018.
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
