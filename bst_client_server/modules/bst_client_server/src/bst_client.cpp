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

#include <klepsydra/bst_client_server/bst_client.h>

kpsr::bst::BstClient::BstClient(kpsr::Container * container,
                                kpsr::Environment * environment,
                                Subscriber<std::string> * clientStateMachineExtSubscriber,
                                Publisher<std::string> * clientStateMachineExtPublisher,
                                Subscriber<std::string> * clientStateMachineSubscriber,
                                Publisher<std::string> * clientStateMachinePublisher,
                                kpsr::mem::CacheListener<std::string> & clientStateMachineListener,
                                BstClientMiddlewareProvider * bstClientMiddlewareProvider,
                                kpsr::mem::CacheListener<TelemetrySystem_t> * telemetrySystemEventListener)
    : Service(environment, "BST_CLIENT")
    , _clientStateMachineSubscriber(clientStateMachineSubscriber)
    , _clientStateMachineExtPublisher(clientStateMachineExtPublisher)
    , _clientStateMachineListener(clientStateMachineListener)
    , _bstClientMiddlewareProvider(bstClientMiddlewareProvider)
    , _clientStateMachine(environment, bstClientMiddlewareProvider, clientStateMachinePublisher,
                          clientStateMachineExtSubscriber, telemetrySystemEventListener)
{
    if (container) {
        container->attach(this);
        container->attach(&_clientStateMachine);
    }
}

void kpsr::bst::BstClient::start() {
    _clientStateMachineSubscriber->registerListener("BST_CLIENT", _clientStateMachineListener.cacheListenerFunction);
    _clientStateMachineSubscriber->registerListener("BST_CLIENT_DEBUG", [](const std::string & state){
        spdlog::debug("{}. clienStateMachine state received: {}", __PRETTY_FUNCTION__, state);
    });
    _clientStateMachine.start();

    int heartbeatPeriod;
    _environment->getPropertyInt("bst_client_heartbeat_period_microsecs", heartbeatPeriod);
    if (heartbeatPeriod > 0) {
        std::function<void ()> task = std::function<void ()>([this]() {
            kpsr::bst::BstRequestMessage heartbeatMessage;
            heartbeatMessage.id = TELEMETRY_HEARTBEAT;
            _bstClientMiddlewareProvider->getBstRequestMessagePublisher()->publish(heartbeatMessage);
        });
        _bstClientMiddlewareProvider->getScheduler()->startScheduledTask("BST_CLIENT_HEARTBEAT", heartbeatPeriod, true, task);
    }

    int controlPeriod;
    _environment->getPropertyInt("bst_client_control_period_microsecs", controlPeriod);
    if (controlPeriod > 0) {
        std::function<void ()> task = std::function<void ()>([this]() {
            kpsr::bst::BstRequestMessage controlMessage;
            controlMessage.id = SENSORS_AGL;
            _bstClientMiddlewareProvider->getBstRequestMessagePublisher()->publish(controlMessage);
        });
        _bstClientMiddlewareProvider->getScheduler()->startScheduledTask("BST_CLIENT_CONTROL_BEAT", controlPeriod, true, task);
    }
}

void kpsr::bst::BstClient::stop() {
    int heartbeatPeriod;
    _environment->getPropertyInt("bst_client_heartbeat_period_microsecs", heartbeatPeriod);
    if (heartbeatPeriod > 0) {
        _bstClientMiddlewareProvider->getScheduler()->stopScheduledTask("BST_CLIENT_HEARTBEAT");
    }

    int controlPeriod;
    _environment->getPropertyInt("bst_client_control_period_microsecs", controlPeriod);
    if (controlPeriod > 0) {
        _bstClientMiddlewareProvider->getScheduler()->stopScheduledTask("BST_CLIENT_CONTROL_BEAT");
    }
    _clientStateMachine.stop();
    _clientStateMachineSubscriber->removeListener("BST_CLIENT");
}

void kpsr::bst::BstClient::execute() {
    _clientStateMachine.execute();
}

bool kpsr::bst::BstClient::acquirePayloadControl() {
    spdlog::debug("{}", __PRETTY_FUNCTION__);
    if (* _clientStateMachineListener.getLastReceivedEvent().get() == "bstClientStateMachine:ready") {
        spdlog::debug("{}. Sending request to state machine", __PRETTY_FUNCTION__);
        _clientStateMachineExtPublisher->publish("payloadControlReqRx");
        return true;
    }
    return false;
}

bool kpsr::bst::BstClient::releasePayloadControl() {
    if (* _clientStateMachineListener.getLastReceivedEvent().get() == "bstClientStateMachine:payloadControlReady" ||
            * _clientStateMachineListener.getLastReceivedEvent().get() == "bstClientStateMachine:landed") {
        _clientStateMachineExtPublisher->publish("payloadControlOffReqRx");
        return true;
    }
    return false;
}

bool kpsr::bst::BstClient::takeoff() {
    if (* _clientStateMachineListener.getLastReceivedEvent().get() == "bstClientStateMachine:payloadControlReady") {
        _clientStateMachineExtPublisher->publish("takeoffReqRx");
        return true;
    }
    return false;
}

bool kpsr::bst::BstClient::land() {
    if (* _clientStateMachineListener.getLastReceivedEvent().get() == "bstClientStateMachine:flying") {
        _clientStateMachineExtPublisher->publish("landReqRx");
        return true;
    }
    return false;
}

bool kpsr::bst::BstClient::sendCommand(const BstRequestMessage & command) {
    _clientStateMachine.sendCommand(command);
    return true;
}

bool kpsr::bst::BstClient::sendWaypoints(const WaypointCommandMessage & command) {
    _clientStateMachine.sendWaypoints(command);
    return true;
}

std::string kpsr::bst::BstClient::getCurrentState() {
    return * _clientStateMachineListener.getLastReceivedEvent().get();
}
