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


#include <klepsydra/mem_trajectory_monitoring/sf_traj_mon_comms_provider.h>

kpsr::trajectory::mem::BasicTrajectoryCommsProvider::BasicTrajectoryCommsProvider()
    : _trajectoryRequestProvider(nullptr, "trajectory_request", 10, 0, nullptr, nullptr, false)
    , _trajectoryResponseProvider(nullptr, "trajectory_response", 10, 0, nullptr, nullptr, false)
    , trajectoryRequestSubscriber(_trajectoryRequestProvider.getSubscriber())
    , trajectoryRequestPublisher(_trajectoryRequestProvider.getPublisher())
    , trajectoryResponseSubscriber(_trajectoryResponseProvider.getSubscriber())
    , trajectoryResponsePublisher(_trajectoryResponseProvider.getPublisher())
    , trajectoryCallbackHandlerProvider(trajectoryRequestPublisher, trajectoryResponseSubscriber, true)
{}

void kpsr::trajectory::mem::BasicTrajectoryCommsProvider::start() {
    _trajectoryRequestProvider.start();
    _trajectoryResponseProvider.start();
}

void kpsr::trajectory::mem::BasicTrajectoryCommsProvider::stop() {
    _trajectoryRequestProvider.stop();
    _trajectoryResponseProvider.stop();
}
