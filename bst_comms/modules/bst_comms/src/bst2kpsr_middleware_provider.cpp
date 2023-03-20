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

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/bst_comms/bst2kpsr_middleware_provider.h>

#include <spdlog/spdlog.h>

kpsr::high_performance::EventLoopMiddlewareProvider<256> bst2KpsrInternalMessageProvider(nullptr);

void kpsr::bst::Bst2KpsrMiddlewareProvider::start(Container *container)
{
    bst2KpsrInternalMessageProvider.start();
    spdlog::info("{}. started.", __PRETTY_FUNCTION__);
    if (container != nullptr) {
        container->attach(
            &bst2KpsrInternalMessageProvider
                 .getPublisher<kpsr::bst::Bst2KpsrInternalMessage>("bst_internal_message",
                                                                   0,
                                                                   nullptr,
                                                                   nullptr)
                 ->_publicationStats);
        bst2KpsrInternalMessageProvider
            .getSubscriber<kpsr::bst::Bst2KpsrInternalMessage>("bst_internal_message")
            ->_container = container;
    }
}

void kpsr::bst::Bst2KpsrMiddlewareProvider::stop()
{
    bst2KpsrInternalMessageProvider.stop();
}

kpsr::Subscriber<kpsr::bst::Bst2KpsrInternalMessage>
    *kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessageSubscriber()
{
    return bst2KpsrInternalMessageProvider.getSubscriber<kpsr::bst::Bst2KpsrInternalMessage>(
        "bst_internal_message");
}

kpsr::Publisher<kpsr::bst::Bst2KpsrInternalMessage>
    *kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()
{
    return bst2KpsrInternalMessageProvider
        .getPublisher<kpsr::bst::Bst2KpsrInternalMessage>("bst_internal_message",
                                                          0,
                                                          nullptr,
                                                          nullptr);
}
