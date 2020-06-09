/*****************************************************************************
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
#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/bst_comms/bst2kpsr_middleware_provider.h>

#include <spdlog/spdlog.h>

kpsr::high_performance::EventLoopMiddlewareProvider<256> bst2KpsrInternalMessageProvider(nullptr);

void kpsr::bst::Bst2KpsrMiddlewareProvider::start(Container *container) {
    bst2KpsrInternalMessageProvider.start();
    spdlog::info("{}. started.", __PRETTY_FUNCTION__);
    if (container != nullptr) {
        container->attach(&bst2KpsrInternalMessageProvider.getPublisher<kpsr::bst::Bst2KpsrInternalMessage>("bst_internal_message", 0, nullptr, nullptr)->_publicationStats);
        bst2KpsrInternalMessageProvider.getSubscriber<kpsr::bst::Bst2KpsrInternalMessage>("bst_internal_message")->_container = container;
    }
}

void kpsr::bst::Bst2KpsrMiddlewareProvider::stop() {
    bst2KpsrInternalMessageProvider.stop();
}

kpsr::Subscriber<kpsr::bst::Bst2KpsrInternalMessage> * kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessageSubscriber() {
    return bst2KpsrInternalMessageProvider.getSubscriber<kpsr::bst::Bst2KpsrInternalMessage>("bst_internal_message");
}

kpsr::Publisher<kpsr::bst::Bst2KpsrInternalMessage> * kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher() {
    return bst2KpsrInternalMessageProvider.getPublisher<kpsr::bst::Bst2KpsrInternalMessage>("bst_internal_message", 0, nullptr, nullptr);
}
