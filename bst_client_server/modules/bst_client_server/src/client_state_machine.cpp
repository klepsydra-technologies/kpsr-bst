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

#include <iostream>
#include <spdlog/spdlog.h>

#include <klepsydra/bst_client_server/client_state_machine.h>

kpsr::bst::ClientStateMachine::ClientStateMachine(
    Environment *environment,
    BstClientMiddlewareProvider *bstClientMiddlewareProvider,
    Publisher<std::string> *clientStateMachinePublisher,
    Subscriber<std::string> *clientStateMachineExtSubscriber,
    kpsr::mem::CacheListener<TelemetrySystem_t> *telemetrySystemEventListener)
    : kpsr::Service(environment, "BST_CLIENT_STATE_MACHINE")
    , _environment(environment)
    , _bstClientMiddlewareProvider(bstClientMiddlewareProvider)
    , _stateMachineFactory()
    , _stateMachineConfiguration()
    , _stateMachine(
          _stateMachineFactory.createStateMachine(_stateMachineConfiguration.stateMachineConf))
    , _clientStateMachinePublisher(clientStateMachinePublisher)
    , _clientStateMachineExtSubscriber(clientStateMachineExtSubscriber)
    , _clientStateMachineListener(_stateMachineConfiguration.stateMachineConf.id)
    , _correlator()
    , _flightPlanCorrelator()
    , _callbackHandler("bst_test_client",
                       _bstClientMiddlewareProvider->getBstRequestMessagePublisher(),
                       _bstClientMiddlewareProvider->getBst2KpsrReplyMessageSubscriber(),
                       _correlator.correlationFunction)
    , _flightPlanCallbackHandler("bst_test_client",
                                 _bstClientMiddlewareProvider
                                     ->getBstWaypointCommandMessagePublisher(),
                                 _bstClientMiddlewareProvider->getBst2KpsrReplyMessageSubscriber(),
                                 _flightPlanCorrelator.correlationFunction)
    , _telemetrySystemEventListener(telemetrySystemEventListener)
{}

void kpsr::bst::ClientStateMachine::start()
{
    _stateMachine->registerObserver(std::bind(&kpsr::bst::ClientStateMachine::updateCurrentState,
                                              this,
                                              std::placeholders::_1,
                                              std::placeholders::_2));
    _stateMachine->registerObserver(_clientStateMachineListener.getObserverFunc());
    _stateMachine->start();

    _clientStateMachineExtSubscriber->registerListener("BST_CLIENT",
                                                       [this](const std::string &event) {
                                                           _stateMachine->enqueueEvent(event);
                                                       });

    _bstClientMiddlewareProvider->getTelemetrySystemSubscriber()
        ->registerListener("BST_CLIENT", _telemetrySystemEventListener->cacheListenerFunction);

    _bstClientMiddlewareProvider->getTelemetrySystemSubscriber()->registerListenerOnce(
        [this](const TelemetrySystem_t &event) {
            _stateMachine->enqueueEvent("telemetrySystemRx");
        });

    _clientStateMachineListener
        .addAction("ready", std::bind(&kpsr::bst::ClientStateMachine::addActionsOnReadyState, this));
}

void kpsr::bst::ClientStateMachine::sendControlCommandAndUpdateOnAck(unsigned char type,
                                                                     float value,
                                                                     const std::string &eventPrefix)
{
    kpsr::bst::BstRequestMessage message;
    message.id = CONTROL_COMMAND;
    message.type = type;
    message.value = value;

    _callbackHandler.requestAndReply(message,
                                     [this, eventPrefix](const kpsr::bst::BstReplyMessage &reply) {
                                         if (reply.ack) {
                                             _stateMachine->enqueueEvent(eventPrefix + "AckRx");
                                         } else {
                                             _stateMachine->enqueueEvent(eventPrefix + "NackRx");
                                         }
                                     });
}

void kpsr::bst::ClientStateMachine::execute()
{
    _stateMachine->update();
}

void kpsr::bst::ClientStateMachine::stop()
{
    _stateMachine->stop();
}

void kpsr::bst::ClientStateMachine::updateCurrentState(const std::string &currentState,
                                                       bool stateChanged)
{
    if (stateChanged) {
        std::cout << " #  bstClientStateMachine -->  " << currentState << std::endl;
        _clientStateMachinePublisher->publish(currentState);

        if (currentState == "bstClientStateMachine:payloadControlReady" ||
            currentState == "bstClientStateMachine:error") {
            try {
                _bstClientMiddlewareProvider->getTelemetrySystemSubscriber()->removeListener(
                    "bst_state_machine_payloadcontrol");
            } catch (const std::logic_error &e) {
                spdlog::info(
                    "{}. Could not remove event listener: bst_state_machine_payloadcontrol.",
                    __PRETTY_FUNCTION__);
            }
        } else if (currentState == "bstClientStateMachine:launchModeReq" ||
                   currentState == "bstClientStateMachine:error") {
            try {
                _bstClientMiddlewareProvider->getTelemetrySystemSubscriber()->removeListener(
                    "bst_state_machine_preflight");
            } catch (const std::logic_error &e) {
                spdlog::info("{}. Could not remove event listener: bst_state_machine_preflight.",
                             __PRETTY_FUNCTION__);
            }
        } else if (currentState == "bstClientStateMachine:enableEngineReq" ||
                   currentState == "bstClientStateMachine:error") {
            try {
                _bstClientMiddlewareProvider->getTelemetrySystemSubscriber()->removeListener(
                    "bst_state_machine_launch_mode");
            } catch (const std::logic_error &e) {
                spdlog::info("{}. Could not remove event listener: bst_state_machine_launch_mode.",
                             __PRETTY_FUNCTION__);
            }
        } else if (currentState == "bstClientStateMachine:flying" ||
                   currentState == "bstClientStateMachine:error") {
            try {
                _bstClientMiddlewareProvider->getTelemetrySystemSubscriber()->removeListener(
                    "bst_state_machine_launch");
            } catch (const std::logic_error &e) {
                spdlog::info("{}. Could not remove event listener: bst_state_machine_launch.",
                             __PRETTY_FUNCTION__);
            }
        }
    }
}

void kpsr::bst::ClientStateMachine::addActionsOnReadyState()
{
    _bstClientMiddlewareProvider->getTelemetrySystemSubscriber()
        ->registerListener("bst_state_machine", [this](const TelemetrySystem_t &event) {
            spdlog::debug("{}. launchReq. TelemetrySystem Received: {}",
                          __PRETTY_FUNCTION__,
                          event.flight_mode);
        });

    _clientStateMachineListener
        .addAction("payloadControlReadyReq", [this](const std::string &eventId) {
            spdlog::info("{}. payloadControlReady requested.", __PRETTY_FUNCTION__);
            sendControlCommandAndUpdateOnAck(CMD_PAYLOAD_CONTROL,
                                             PAYLOAD_CTRL_READY,
                                             "payloadControlReady");
        });

    _clientStateMachineListener
        .addAction("payloadControlReqAccepted", [this](const std::string &eventId) {
            spdlog::info("{}. payloadControlReady accepted.", __PRETTY_FUNCTION__);
            checkAndWaitUntilFlightMode({FLIGHT_MODE_INVALID_MODE,
                                         FLIGHT_MODE_LANDED,
                                         FLIGHT_MODE_PREFLIGHT,
                                         FLIGHT_MODE_LAUNCH},
                                        "bst_state_machine_payloadcontrol",
                                        "validFlightModeRx",
                                        "notValidFlightModeRx");
        });

    _clientStateMachineListener.addAction("preFlightReq", [this](const std::string &eventId) {
        spdlog::info("{}. preFlight mode requested.", __PRETTY_FUNCTION__);
        if (_telemetrySystemEventListener->getLastReceivedEvent()->flight_mode ==
                FLIGHT_MODE_PREFLIGHT ||
            _telemetrySystemEventListener->getLastReceivedEvent()->flight_mode ==
                FLIGHT_MODE_LAUNCH) {
            _stateMachine->enqueueEvent("alreadyPreFlight");
        } else {
            sendControlCommandAndUpdateOnAck(CMD_FLIGHT_MODE, FLIGHT_MODE_PREFLIGHT, "preFlightReq");
        }
    });

    _clientStateMachineListener.addAction("preFlightReqAccepted", [this](const std::string &eventId) {
        spdlog::info("{}. preFlight mode accepted.", __PRETTY_FUNCTION__);
        checkAndWaitUntilFlightMode({FLIGHT_MODE_PREFLIGHT},
                                    "bst_state_machine_preflight",
                                    "preFlightModeRx",
                                    "notPreFlightModeRx");
    });

    _clientStateMachineListener.addAction("launchModeReq", [this](const std::string &eventId) {
        spdlog::info("{}. launch mode requested.", __PRETTY_FUNCTION__);
        if (_telemetrySystemEventListener->getLastReceivedEvent()->flight_mode ==
            FLIGHT_MODE_PREFLIGHT) {
            spdlog::debug("{}. launch mode requested. sendControlCommandAndUpdateOnAck",
                          __PRETTY_FUNCTION__);
            sendControlCommandAndUpdateOnAck(CMD_FLIGHT_MODE, FLIGHT_MODE_LAUNCH, "launchModeReq");
        } else if (_telemetrySystemEventListener->getLastReceivedEvent()->flight_mode ==
                   FLIGHT_MODE_LAUNCH) {
            spdlog::debug("{}. launch mode requested. already on FLIGHT_MODE_LAUNCH.",
                          __PRETTY_FUNCTION__);
            _stateMachine->enqueueEvent("alreadyLaunchMode");
        }
    });

    _clientStateMachineListener
        .addAction("launchModeReqAccepted", [this](const std::string &eventId) {
            spdlog::info("{}. launch mode accepted.", __PRETTY_FUNCTION__);
            checkAndWaitUntilFlightMode({FLIGHT_MODE_LAUNCH},
                                        "bst_state_machine_launch_mode",
                                        "launchModeRx",
                                        "notLaunchModeRx");
        });

    _clientStateMachineListener.addAction("enableEngineReq", [this](const std::string &eventId) {
        spdlog::info("{}. kill engine requested.", __PRETTY_FUNCTION__);
        sendControlCommandAndUpdateOnAck(CMD_ENGINE_KILL, 0, "enableEngineReq");
    });

    _clientStateMachineListener.addAction("launchReq", [this](const std::string &eventId) {
        spdlog::info("{}. launch requested.", __PRETTY_FUNCTION__);
        sendControlCommandAndUpdateOnAck(CMD_LAUNCH, 1, "launchReq");
    });

    _clientStateMachineListener.addAction("launchReqAccepted", [this](const std::string &eventId) {
        spdlog::info("{}. launch accepted.", __PRETTY_FUNCTION__);
        checkAndWaitUntilFlightMode({FLIGHT_MODE_FLYING},
                                    "bst_state_machine_launch",
                                    "flyingModeRx",
                                    "notFlyingModeRx");
    });

    _clientStateMachineListener.addAction("landReq", [this](const std::string &eventId) {
        spdlog::info("{}. land requested.", __PRETTY_FUNCTION__);
        sendControlCommandAndUpdateOnAck(CMD_LAND, 1, "landReq");
    });

    _clientStateMachineListener.addAction("landReqAccepted", [this](const std::string &eventId) {
        spdlog::info("{}. land accepted.", __PRETTY_FUNCTION__);
        checkAndWaitUntilFlightMode({FLIGHT_MODE_LANDING},
                                    "bst_state_machine_land",
                                    "landingModeRx",
                                    "notLandingModeRx");
    });

    _clientStateMachineListener.addAction("payloadControlOffReq", [this](const std::string &eventId) {
        spdlog::info("{}. paylonad control off requested.", __PRETTY_FUNCTION__);
        sendControlCommandAndUpdateOnAck(CMD_PAYLOAD_CONTROL, PAYLOAD_CTRL_OFF, "payloadControlOff");
    });
}

void kpsr::bst::ClientStateMachine::sendCommand(const kpsr::bst::BstRequestMessage &command)
{
    if (command.id != CONTROL_COMMAND) {
        spdlog::debug("{}. NON-CONTROL_COMMAND command.id: {}", __PRETTY_FUNCTION__, command.id);
        _callbackHandler.requestAndReply(command, [this](const kpsr::bst::BstReplyMessage &reply) {
            if (reply.ack) {
                std::cout << " Command ACK received." << std::endl;
                spdlog::info("{}. Command ACK received.", __PRETTY_FUNCTION__);
            } else {
                std::cout << " Command NACK received." << std::endl;
                spdlog::info("{}. Command NACK received.", __PRETTY_FUNCTION__);
            }
        });
        return;
    }

    _clientStateMachineListener
        .addOneOffAction("controlCommandReq", [this, command](const std::string &eventId) {
            spdlog::debug("{}. CONTROL_COMMAND command.id: {}", __PRETTY_FUNCTION__, command.id);
            _callbackHandler
                .requestAndReply(command, [this](const kpsr::bst::BstReplyMessage &reply) {
                    if (reply.ack) {
                        _stateMachine->enqueueEvent("controlCommandReqAckRx");
                    } else {
                        _stateMachine->enqueueEvent("controlCommandReqNackRx");
                    }
                });
        });

    _stateMachine->enqueueEvent("controlCommandRx");
}

bool kpsr::bst::ClientStateMachine::sendWaypoints(const WaypointCommandMessage &command)
{
    _clientStateMachineListener
        .addOneOffAction("flightPlanReq", [this, command](const std::string &eventId) {
            spdlog::debug("{}. Flight Plan requested.id: {}", __PRETTY_FUNCTION__, command.id);
            _flightPlanCallbackHandler
                .requestAndReply(command, [this](const kpsr::bst::BstReplyMessage &reply) {
                    if (reply.ack) {
                        spdlog::warn("Flight Plan reply.ack: {}", reply.ack);
                        _stateMachine->enqueueEvent("flightPlanReqAckRx");
                    } else {
                        spdlog::warn("Flight Plan reply.ack: {}", reply.ack);
                        _stateMachine->enqueueEvent("flightPlanReqNackRx");
                    }
                });
        });

    _stateMachine->enqueueEvent("flightPlanRx");
    return true;
}

void kpsr::bst::ClientStateMachine::checkAndWaitUntilFlightMode(
    std::vector<FlightMode_t> validFlyingModes,
    std::string listenerName,
    std::string validModeEvent,
    std::string notValidModeEvent)
{
    int maxCounter;
    _environment->getPropertyInt("bst_client_flight_mode_attempts", maxCounter);

    struct Counter
    {
    public:
        Counter(int max)
            : _max(max)
        {}

        bool increaseAndCheck() { return (++_counter < _max); }

        bool alive = true;

    private:
        int _counter = 0;
        int _max = 5;
    };

    std::shared_ptr<Counter> counter(new Counter(maxCounter));
    _bstClientMiddlewareProvider->getTelemetrySystemSubscriber()->registerListener(
        listenerName,
        [this, counter, validFlyingModes, listenerName, validModeEvent, notValidModeEvent](
            const TelemetrySystem_t &event) {
            spdlog::debug("{}. TelemetrySystem_t received with flight mode: {}",
                          __PRETTY_FUNCTION__,
                          event.flight_mode);
            if (counter->alive) {
                if (std::find(validFlyingModes.begin(), validFlyingModes.end(), event.flight_mode) !=
                    validFlyingModes.end()) {
                    spdlog::debug("{}. TelemetrySystem_t event is valid!", __PRETTY_FUNCTION__);
                    _stateMachine->enqueueEvent(validModeEvent);
                    counter->alive = false;
                } else {
                    spdlog::debug("{}. TelemetrySystem_t event is NOT valid!", __PRETTY_FUNCTION__);
                    if (!counter->increaseAndCheck()) {
                        spdlog::warn("{}. Counter reached!", __PRETTY_FUNCTION__);
                        _stateMachine->enqueueEvent(notValidModeEvent);
                        counter->alive = false;
                    }
                }
            }
        });
    spdlog::debug("{}. Added event listener: {}", __PRETTY_FUNCTION__, listenerName);
}
