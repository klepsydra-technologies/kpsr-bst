// Copyright 2023 Klepsydra Technologies AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include <algorithm>
#include <sstream>
#include <functional>

#include <spdlog/spdlog.h>

#include <klepsydra/serialization/json_cereal_mapper.h>

#include <klepsydra/trajectory_monitoring/rest_trajectory_monitoring_interface.h>
#include <klepsydra/trajectory_monitoring/cereal/trajectory_monitoring_response_serializer.h>

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCurrentPosition(const Pistache::Rest::Request& request,
                                                                                      Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_POSITION;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<Position, std::string> mapper;
            mapper.toMiddleware(response.position, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_CURRENT_POSITION {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCurrentGpsPosition(const Pistache::Rest::Request& request,
                                                                                         Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_GPS_POSITION;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<GpsPosition, std::string> mapper;
            mapper.toMiddleware(response.gpsPosition, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_CURRENT_GPS_POSITION {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCurrentOrientation(const Pistache::Rest::Request& request,
                                                                                         Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_ORIENTATION;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<Orientation, std::string> mapper;
            mapper.toMiddleware(response.orientation, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_CURRENT_ORIENTATION {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCurrentLinearVelocity(const Pistache::Rest::Request& request,
                                                                                            Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_LINEAR_VELOCITY;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<LinearVelocity, std::string> mapper;
            mapper.toMiddleware(response.linearVelocity, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_CURRENT_LINEAR_VELOCITY {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCurrentAngularRates(const Pistache::Rest::Request& request,
                                                                                          Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_CURRENT_ANGULAR_RATES;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<AngularRates, std::string> mapper;
            mapper.toMiddleware(response.angularRates, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_CURRENT_ANGULAR_RATES {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCommandedPosition(const Pistache::Rest::Request& request,
                                                                                        Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_POSITION;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<Position, std::string> mapper;
            mapper.toMiddleware(response.position, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_COMMANDED_POSITION {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCommandGpsPosition(const Pistache::Rest::Request& request,
                                                                                         Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_GPS_POSITION;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<GpsPosition, std::string> mapper;
            mapper.toMiddleware(response.gpsPosition, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_COMMANDED_GPS_POSITION {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCommandedOrientation(const Pistache::Rest::Request& request,
                                                                                           Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_ORIENTATION;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<Orientation, std::string> mapper;
            mapper.toMiddleware(response.orientation, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_COMMANDED_ORIENTATION {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCommandedLinearVelocity(const Pistache::Rest::Request& request,
                                                                                              Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_LINEAR_VELOCITY;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<LinearVelocity, std::string> mapper;
            mapper.toMiddleware(response.linearVelocity, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_COMMANDED_LINEAR_VELOCITY {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::getCommandedAngularRates(const Pistache::Rest::Request& request,
                                                                                            Pistache::Http::ResponseWriter responseWriter) {
    kpsr::trajectory::TrajectoryMonitoringRequest trajectoryRequest;
    trajectoryRequest.id = _seq++;
    trajectoryRequest.type = kpsr::trajectory::TrajectoryCommandType::GET_COMMANDED_ANGULAR_RATES;
    std::function<std::string(kpsr::trajectory::TrajectoryMonitoringResponse)> replyToString = [] (kpsr::trajectory::TrajectoryMonitoringResponse response) {
        std::string restReply;
        if (response.result) {
            Mapper<AngularRates, std::string> mapper;
            mapper.toMiddleware(response.angularRates, restReply);
        }
        else {
            Mapper<bool, std::string> mapper;
            mapper.toMiddleware(response.result, restReply);
        }
        spdlog::info("kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface. Publish GET_COMMANDED_ANGULAR_RATES {}", response.id);
        return restReply;
    };
    performRequest(responseWriter, trajectoryRequest, replyToString);
}

void kpsr::trajectory::restapi::RestTrajectoryMonitoringInterface::setupRoutes(Pistache::Rest::Router & router) {
    Pistache::Rest::Routes::Get(router, "/trajectory/getCurrentPosition", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCurrentPosition, this));
    Pistache::Rest::Routes::Get(router, "/trajectory/getCurrentGpsPosition", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCurrentGpsPosition, this));
    Pistache::Rest::Routes::Get(router, "/trajectory/getCurrentOrientation", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCurrentOrientation, this));
    Pistache::Rest::Routes::Get(router, "/trajectory/getCurrentLinearVelocity", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCurrentLinearVelocity, this));
    Pistache::Rest::Routes::Get(router, "/trajectory/getCurrentAngularRates", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCurrentAngularRates, this));

    Pistache::Rest::Routes::Get(router, "/trajectory/getCommandedPosition", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCommandedPosition, this));
    Pistache::Rest::Routes::Get(router, "/trajectory/getCommandedGpsPosition", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCommandGpsPosition, this));
    Pistache::Rest::Routes::Get(router, "/trajectory/getCommandedOrientation", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCommandedOrientation, this));
    Pistache::Rest::Routes::Get(router, "/trajectory/getCommandedLinearVelocity", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCommandedLinearVelocity, this));
    Pistache::Rest::Routes::Get(router, "/trajectory/getCommandedAngularRates", Pistache::Rest::Routes::bind(&RestTrajectoryMonitoringInterface::getCommandedAngularRates, this));
}
