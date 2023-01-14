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

#ifndef BST2KPSR_ADAPTOR_SERVICE_H
#define BST2KPSR_ADAPTOR_SERVICE_H

#include <klepsydra/core/service.h>

#include <klepsydra/bst_comms/bst2_kpsr_internal_message.h>
#include <klepsydra/bst_comms/bst2kpsr_broadcaster.h>

namespace kpsr
{
namespace bst
{
/**
 * @brief The Bst2KpsrAdaptorService class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst
 *
 * @details Message adaptor service to integrate BST and Klepsydra data domains.
 */
class Bst2KpsrAdaptorService : public Service {
public:
    /**
     * @brief Bst2KpsrAdaptorService
     * @param environment
     * @param container
     * @param bst2KpsrBroadcaster
     */
    Bst2KpsrAdaptorService(Environment *environment,
                           Container * container,
                           Bst2KpsrBroadcaster & bst2KpsrBroadcaster);

    /**
     * @brief onMessageReceived
     * @param message
     */
    void onMessageReceived(const Bst2KpsrInternalMessage & message);

    /**
     * @brief start
     */
    void start();

    /**
     * @brief stop
     */
    void stop();

    /**
     * @brief execute
     */
    void execute() {}

private:
    Container * _container;
    Bst2KpsrBroadcaster & _bst2KpsrBroadcaster;
};
}
}
#endif // BST2KPSR_ADAPTOR_SERVICE_H

