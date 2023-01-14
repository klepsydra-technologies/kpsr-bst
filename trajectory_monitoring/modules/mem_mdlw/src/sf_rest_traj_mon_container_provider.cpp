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


#include <klepsydra/mem_trajectory_monitoring/sf_rest_traj_mon_container_provider.h>

kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider::BasicRestTrajectoryContainerProvider(kpsr::restapi::RestEndpoint & restEndpoint,
                                                                                                              kpsr::trajectory::TrajectoryDataProvider * trajectoryDataProvider,
                                                                                                              Environment * env, std::string applicationName)
    : _restEndpoint(restEndpoint)
    , _trajectoryCommsProvider()
    , _restTrajMonInterface(_trajectoryCommsProvider.trajectoryCallbackHandlerProvider.getCallbackHandler())
    , _container(env, applicationName, trajectoryDataProvider,
                 _trajectoryCommsProvider.trajectoryRequestSubscriber,
                 _trajectoryCommsProvider.trajectoryResponsePublisher)
{
    _restEndpoint.attachApplication(&_restTrajMonInterface);
}

kpsr::trajectory::TrajectoryContainer & kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider::getContainer() {
    return _container;
}

void kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider::start() {
    _container.start();
    _trajectoryCommsProvider.start();
}

void kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider::stop() {
    _trajectoryCommsProvider.stop();
    _container.stop();
}
