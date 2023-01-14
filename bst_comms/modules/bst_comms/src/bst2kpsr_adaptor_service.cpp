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

#include <functional>

#include <spdlog/spdlog.h>

#include <klepsydra/bst_comms/bst2kpsr_adaptor_service.h>
#include <klepsydra/bst_comms/bst2kpsr_middleware_provider.h>

kpsr::bst::Bst2KpsrAdaptorService::Bst2KpsrAdaptorService(Environment *environment,
                                                          Container * container,
                                                          Bst2KpsrBroadcaster & bst2KpsrBroadcaster)
    : Service(environment, "bst2kpsr_adaptor_service")
    , _container(container)
    , _bst2KpsrBroadcaster(bst2KpsrBroadcaster)
{}

void kpsr::bst::Bst2KpsrAdaptorService::start() {
    Bst2KpsrMiddlewareProvider::start(_container);
    std::function<void(Bst2KpsrInternalMessage)> bst2KpsrAdaptorListener = std::bind(&Bst2KpsrAdaptorService::onMessageReceived, this, std::placeholders::_1);
    Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessageSubscriber()->registerListener("Bst2KpsrAdaptorService", bst2KpsrAdaptorListener);
}

void kpsr::bst::Bst2KpsrAdaptorService::stop() {
    Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessageSubscriber()->removeListener("Bst2KpsrAdaptorService");
    Bst2KpsrMiddlewareProvider::stop();
}

void kpsr::bst::Bst2KpsrAdaptorService::onMessageReceived(const Bst2KpsrInternalMessage &message) {
    spdlog::debug("{}{} - {}", __PRETTY_FUNCTION__, message.commsFunction, message.commsModule);
    switch (message.commsFunction) {
    case COMMS_FUNCTION::PUBLISH:
        spdlog::debug("{} PUBLISH", __PRETTY_FUNCTION__);
        _bst2KpsrBroadcaster.publish(message.type, message.publishParam);
        break;
    case COMMS_FUNCTION::RECEIVE:
        spdlog::debug("{} RECEIVE", __PRETTY_FUNCTION__);
        _bst2KpsrBroadcaster.receive(message.type, message.data, message.size, message.receivedParameter);
        break;
    case COMMS_FUNCTION::RECEIVE_COMMAND:
        spdlog::debug("{} RECEIVE_COMMAND", __PRETTY_FUNCTION__);
        _bst2KpsrBroadcaster.receiveCommand(message.type, message.data, message.size, message.receivedParameter);
        break;
    case COMMS_FUNCTION::RECEIVE_REPLY:
        spdlog::debug("{} RECEIVE_REPLY: type={}, data={}, ack: {}", __PRETTY_FUNCTION__, (int) message.type, (int) message.data[0], (message.ack ? "ACK" : "NACK"));
        _bst2KpsrBroadcaster.receiveReply(message.type, message.data, message.size, message.ack, message.receivedParameter);
        break;
    }
}
