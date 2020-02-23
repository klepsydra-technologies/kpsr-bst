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

#ifndef BASIC_TRAJECTORY_COMM_PROVIDER_H
#define BASIC_TRAJECTORY_COMM_PROVIDER_H

#include <klepsydra/mem_core/basic_middleware_provider.h>

#include <klepsydra/trajectory_monitoring/trajectory_monitoring_request.h>
#include <klepsydra/trajectory_monitoring/trajectory_monitoring_response.h>

#include <klepsydra/trajectory_monitoring/trajectory_callback_handler_provider.h>

namespace kpsr {
namespace trajectory {
namespace mem {
/**
 * @brief The BasicTrajectoryCommsProvider class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-mem-trajectory
 *
 * @details Safe Queue based implementation of the trajectory request and reply queues.
 */
class BasicTrajectoryCommsProvider
{

private:
    kpsr::mem::BasicMiddlewareProvider<TrajectoryMonitoringRequest> _trajectoryRequestProvider;
    kpsr::mem::BasicMiddlewareProvider<TrajectoryMonitoringResponse> _trajectoryResponseProvider;

public:
    /**
     * @brief BasicTrajectoryCommsProvider
     */
    BasicTrajectoryCommsProvider();

    /**
     * @brief start
     */
    void start();

    /**
     * @brief stop
     */
    void stop();

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

#endif // BASIC_TRAJECTORY_COMM_PROVIDER_H
