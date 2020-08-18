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
        std::shared_ptr<std::function<void ()>> task = std::make_shared<std::function<void ()>>([this]() {
            kpsr::bst::BstRequestMessage heartbeatMessage;
            heartbeatMessage.id = TELEMETRY_HEARTBEAT;
            _bstClientMiddlewareProvider->getBstRequestMessagePublisher()->publish(heartbeatMessage);
        });
        _bstClientMiddlewareProvider->getScheduler()->startScheduledTask("BST_CLIENT_HEARTBEAT", heartbeatPeriod, true, task);
    }

    int controlPeriod;
    _environment->getPropertyInt("bst_client_control_period_microsecs", controlPeriod);
    if (controlPeriod > 0) {
        std::shared_ptr<std::function<void ()>> task = std::make_shared<std::function<void ()>>([this]() {
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

bool kpsr::bst::BstClient::sendCommand(const BstRequestMessage command) {
    _clientStateMachine.sendCommand(command);
    return true;
}

bool kpsr::bst::BstClient::sendWaypoints(const WaypointCommandMessage command) {
    if (* _clientStateMachineListener.getLastReceivedEvent().get() == "bstClientStateMachine:flying") {
        _bstClientMiddlewareProvider->getBstWaypointCommandMessagePublisher()->publish(command);
        return true;
    }
    return false;
}

std::string kpsr::bst::BstClient::getCurrentState() {
    return * _clientStateMachineListener.getLastReceivedEvent().get();
}
