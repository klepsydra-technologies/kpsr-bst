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
 * @copyright Klepsydra Robotics 2017-2018.
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

