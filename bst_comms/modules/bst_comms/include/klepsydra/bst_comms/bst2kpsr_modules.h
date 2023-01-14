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

#ifndef BST2KPSR_MODULES_H
#define BST2KPSR_MODULES_H

#include <inttypes.h>

namespace kpsr
{
namespace bst
{
namespace Bst2KpsrModules
{
namespace BasicModule {
    void sendCommand(uint8_t type, void * data, uint16_t size, void * parameter);
    void receive(uint8_t type, void * data, uint16_t size, const void * parameter);
    uint8_t receiveCommand(uint8_t type, void * data, uint16_t size, const void * parameter);
    void receiveReply(uint8_t type, void * data, uint16_t size, bool ack, const void * parameter);
    bool publish(uint8_t type, uint8_t param);
}
namespace FlightPlan {
    void sendCommand(uint8_t type, void * data, uint16_t size, void * parameter);
    void receive(uint8_t type, void * data, uint16_t size, const void * parameter);
    uint8_t receiveCommand(uint8_t type, void * data, uint16_t size, const void * parameter);
    void receiveReply(uint8_t type, void * data, uint16_t size, bool ack, const void * parameter);
    bool publish(uint8_t type, uint8_t param);
}
}
}
}

#endif
