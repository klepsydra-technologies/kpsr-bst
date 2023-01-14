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

#include <spdlog/spdlog.h>

#include <klepsydra/trajectory_monitoring/trajectory_container.h>

kpsr::trajectory::TrajectoryContainer::TrajectoryContainer(Environment *env, std::string serviceName,
                                                           TrajectoryDataProvider * trajectoryDataProvider,
                                                           Subscriber<TrajectoryMonitoringRequest> *trajectoryRequestSubscriber, Publisher<TrajectoryMonitoringResponse> *trajectoryResponsePublisher)
      : Service(env, serviceName)
      , _trajectoryDataProvider(trajectoryDataProvider)
      , _trajectoryRequestSubscriber(trajectoryRequestSubscriber)
      , _trajectoryResponsePublisher(trajectoryResponsePublisher)
  {}

void kpsr::trajectory::TrajectoryContainer::start() {
    std::function<void(kpsr::trajectory::TrajectoryMonitoringRequest)> trajectoryRequestListener = std::bind(&kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest, this, std::placeholders::_1);
    _trajectoryRequestSubscriber->registerListener("trajectory_monitoring_container", trajectoryRequestListener);
}

void kpsr::trajectory::TrajectoryContainer::stop() {
    _trajectoryRequestSubscriber->removeListener("trajectory_monitoring_container");
}

void kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest(const kpsr::trajectory::TrajectoryMonitoringRequest &request) {
    spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest");
    kpsr::trajectory::TrajectoryMonitoringResponse response;
    response.id = request.id;
    response.type = request.type;

    switch (request.type) {
    case GET_COMMANDED_ANGULAR_RATES: {
        bool isAvailable = _trajectoryDataProvider->getCommandedAngularRates(response.angularRates);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_COMMANDED_ANGULAR_RATES {}", response.id);
        break;
    }
    case GET_COMMANDED_GPS_POSITION: {
        bool isAvailable = _trajectoryDataProvider->getCommandedGpsPosition(response.gpsPosition);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_COMMANDED_GPS_POSITION {}", response.id);
        break;
    }
    case GET_COMMANDED_LINEAR_VELOCITY: {
        bool isAvailable = _trajectoryDataProvider->getCommandedLinearVelocity(response.linearVelocity);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_COMMANDED_LINEAR_VELOCITY {}", response.id );
        break;
    }
    case GET_COMMANDED_ORIENTATION: {
        bool isAvailable = _trajectoryDataProvider->getCommandedOrientation(response.orientation);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_COMMANDED_ORIENTATION {}", response.id);
        break;
    }
    case GET_COMMANDED_POSITION: {
        bool isAvailable = _trajectoryDataProvider->getCommandedPosition(response.position);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_COMMANDED_POSITION {}", response.id);
        break;
    }
    case GET_CURRENT_ANGULAR_RATES: {
        bool isAvailable = _trajectoryDataProvider->getCurrentAngularRates(response.angularRates);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_CURRENT_ANGULAR_RATES {}", response.id);
        break;
    }
    case GET_CURRENT_GPS_POSITION: {
        bool isAvailable = _trajectoryDataProvider->getCurrentGpsPosition(response.gpsPosition);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_CURRENT_GPS_POSITION {}", response.id);
        break;
    }
    case GET_CURRENT_LINEAR_VELOCITY: {
        bool isAvailable = _trajectoryDataProvider->getCurrentLinearVelocity(response.linearVelocity);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_CURRENT_LINEAR_VELOCITY {}", response.id);
        break;
    }
    case GET_CURRENT_ORIENTATION: {
        bool isAvailable = _trajectoryDataProvider->getCurrentOrientation(response.orientation);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_CURRENT_ORIENTATION {}", response.id);
        break;
    }
    case GET_CURRENT_POSITION: {
        bool isAvailable = _trajectoryDataProvider->getCurrentPosition(response.position);
        response.result = isAvailable;
        spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Publish GET_CURRENT_POSITION {}", response.id);
        break;
    }
    default:
                               spdlog::info("kpsr::trajectory::TrajectoryContainer::onTrajectoryRequest. Unknown request. {}", response.id);
        break;
    }
    _trajectoryResponsePublisher->publish(response);
}

