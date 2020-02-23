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

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

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
    void updateCurrentState(const std::string &currentState);

    void sendCommand(const BstRequestMessage command);

private:

    void sendControlCommandAndUpdateOnAck(unsigned char id, float value, std::string eventPrefix);
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
    kpsr::MultiThreadCallbackHandler<kpsr::bst::BstRequestMessage, kpsr::bst::BstReplyMessage> _callbackHandler;
    kpsr::mem::CacheListener<TelemetrySystem_t> * _telemetrySystemEventListener;
};
}
}

#endif // CLIENT_STATE_MACHINE_H
