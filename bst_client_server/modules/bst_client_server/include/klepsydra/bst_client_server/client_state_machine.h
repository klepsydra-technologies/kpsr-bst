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

#ifndef CLIENT_STATE_MACHINE_H
#define CLIENT_STATE_MACHINE_H

#include <klepsydra/core/service.h>
#include <klepsydra/core/callback_handler.h>
#include <klepsydra/core/cache_listener.h>

#include <klepsydra/state_machine/state_machine.h>
#include <klepsydra/state_machine/state_machine_listener.h>
#include <klepsydra/state_machine/sm_factory_impl.h>

#include <klepsydra/bst_comms/bst_client_middleware_provider.h>
#include <klepsydra/bst_comms/request_reply_correlation.h>

#include <klepsydra/bst_client_server/client_state_machine_conf.h>

#include <spdlog/spdlog.h>

namespace kpsr {
namespace bst {
class ClientStateMachine : public Service {
public:
    ClientStateMachine(Environment * environment,
                       BstClientMiddlewareProvider * bstClientMiddlewareProvider,
                       Publisher<std::string> * clientStateMachinePublisher,
                       Subscriber<std::string> * clientStateMachineExtSubscriber,
                       kpsr::mem::CacheListener<TelemetrySystem_t> * telemetrySystemEventListener);

    void start() override;
    void stop() override;
    void execute() override;
    void updateCurrentState(const std::string &currentState, bool stateChanged);

    void sendCommand(const BstRequestMessage & command);

    bool sendWaypoints(const WaypointCommandMessage & command);

private:

    void sendControlCommandAndUpdateOnAck(unsigned char id, float value, const std::string & eventPrefix);
    void checkAndWaitUntilFlightMode(std::vector<FlightMode_t> validFlyingModes, std::string listenerName,
                                     std::string validModeEvent, std::string notValidModeEvent);
    void addActionsOnReadyState();

    Environment * _environment;
    BstClientMiddlewareProvider * _bstClientMiddlewareProvider;
    kpsr::fsm::SMFactoryImpl _stateMachineFactory;
    ClientStateMachineConfiguration _stateMachineConfiguration;
    std::shared_ptr<kpsr::fsm::StateMachine> _stateMachine;
    Publisher<std::string> * _clientStateMachinePublisher;
    Subscriber<std::string> * _clientStateMachineExtSubscriber;
    kpsr::fsm::StateMachineListener _clientStateMachineListener;
    kpsr::bst::RequestReplyCorrelation _correlator;
    kpsr::bst::FlightPlantReplyCorrelation _flightPlanCorrelator;
    kpsr::MultiThreadCallbackHandler<kpsr::bst::BstRequestMessage, kpsr::bst::BstReplyMessage> _callbackHandler;
    kpsr::MultiThreadCallbackHandler<kpsr::bst::WaypointCommandMessage, kpsr::bst::BstReplyMessage> _flightPlanCallbackHandler;
    kpsr::mem::CacheListener<TelemetrySystem_t> * _telemetrySystemEventListener;
};
}
}

#endif // CLIENT_STATE_MACHINE_H
