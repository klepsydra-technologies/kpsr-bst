
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

#ifndef REST_TRAJECTORY_MONITORING_INTERFACE_H
#define REST_TRAJECTORY_MONITORING_INTERFACE_H

#include <atomic>

#include <pistache/http.h>
#include <pistache/router.h>

#include <klepsydra/rest_interface/rest_interface.h>

#include <klepsydra/trajectory_monitoring/trajectory_monitoring_request.h>
#include <klepsydra/trajectory_monitoring/trajectory_monitoring_response.h>

#include <klepsydra/trajectory_monitoring/trajectory_callback_handler_provider.h>

namespace kpsr {
namespace trajectory {
namespace restapi {
/**
 * @brief The RestTrajectoryMonitoringInterface class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-trajectory-rest
 *
 * @details This class exposes the trajectory request and reply messages via REST.
 */
class RestTrajectoryMonitoringInterface :  public kpsr::restapi::RestApplication,
        public kpsr::restapi::RestInterface<TrajectoryMonitoringRequest, TrajectoryMonitoringResponse> {

public:
    /**
     * @brief RestTrajectoryMonitoringInterface
     * @param noThreads
     * @param port
     * @param trajectoryRequestPublisher
     * @param trajectoryResponseSubscriber
     */
    RestTrajectoryMonitoringInterface(kpsr::CallbackHandler<TrajectoryMonitoringRequest, TrajectoryMonitoringResponse> * callbackHandler)
        : kpsr::restapi::RestInterface<TrajectoryMonitoringRequest, TrajectoryMonitoringResponse>(callbackHandler)
        , _seq(0)
    {}

    /**
     * @brief getCurrentPosition
     * @param request
     * @param response
     */
    void getCurrentPosition(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief getCurrentGpsPosition
     * @param request
     * @param response
     */
    void getCurrentGpsPosition(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief getCurrentLinearVelocity
     * @param request
     * @param response
     */
    void getCurrentLinearVelocity(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief getCurrentAngularRates
     * @param request
     * @param response
     */
    void getCurrentAngularRates(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief getCurrentPosition
     * @param request
     * @param response
     */
    void getCurrentOrientation(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief getCommandedPosition
     * @param request
     * @param response
     */
    void getCommandedPosition(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief getCommandGpsPosition
     * @param request
     * @param response
     */
    void getCommandGpsPosition(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief getCommandedOrientation
     * @param request
     * @param response
     */
    void getCommandedOrientation(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief getCommandedLinearVelocity
     * @param request
     * @param response
     */
    void getCommandedLinearVelocity(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief getCommandedAngularRates
     * @param request
     * @param response
     */
    void getCommandedAngularRates(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

    /**
     * @brief setupRoutes
     */
    void setupRoutes(Pistache::Rest::Router & router) override;

private:
    std::atomic<int> _seq;
};
}
}
}
#endif // REST_TRAJECTORY_MONITORING_INTERFACE_H
