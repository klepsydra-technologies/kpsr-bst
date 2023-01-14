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

#ifndef BST_CLIENT_H
#define BST_CLIENT_H

#include <klepsydra/core/cache_listener.h>

#include <klepsydra/bst_client_server/client_state_machine.h>

namespace kpsr {
namespace bst {
class BstClient : public Service {
public:
    BstClient(Container * container,
              Environment * environment,
              Subscriber<std::string> * clientStateMachineExtSubscriber,
              Publisher<std::string> * clientStateMachineExtPublisher,
              Subscriber<std::string> * clientStateMachineSubscriber,
              Publisher<std::string> * clientStateMachinePublisher,
              kpsr::mem::CacheListener<std::string> & clientStateMachineListener,
              BstClientMiddlewareProvider * bstClientMiddlewareProvider,
              kpsr::mem::CacheListener<TelemetrySystem_t> * telemetrySystemEventListener);

    void start() override;

    void stop() override;

    void execute() override;

    bool acquirePayloadControl();

    bool releasePayloadControl();

    bool takeoff();

    bool land();

    bool sendCommand(const BstRequestMessage & command);

    bool sendWaypoints(const WaypointCommandMessage & command);

    std::string getCurrentState();

private:
    Subscriber<std::string> * _clientStateMachineSubscriber;
    Publisher<std::string> * _clientStateMachineExtPublisher;

    kpsr::mem::CacheListener<std::string> & _clientStateMachineListener;

    BstClientMiddlewareProvider * _bstClientMiddlewareProvider;
    ClientStateMachine _clientStateMachine;
};
}
}
#endif // BST_CLIENT_H
