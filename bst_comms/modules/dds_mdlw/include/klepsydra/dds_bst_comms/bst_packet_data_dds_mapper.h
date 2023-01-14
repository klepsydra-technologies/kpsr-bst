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

#ifndef SYSTEM_INITIALIZE_DDS_MAPPER_H
#define SYSTEM_INITIALIZE_DDS_MAPPER_H

#include <streambuf>
#include <cstring>

/* BST */
#include "bst_module_basic.h"
#include "comm_packets.h"
#include "gcs.h"
#include "multicopter.h"

#include <klepsydra/core/environment.h>

#include <klepsydra/serialization/mapper.h>

#include <bst_packet_data.hpp>

namespace kpsr {
template<class T>
/**
 * @brief The Mapper<T, kpsr_dds_bst::BstPacketData> class
 */
class Mapper<T, kpsr_dds_bst::BstPacketData>
{
public:

    /**
    * @brief fromMiddleware
    * @param message
    * @param event
    */
    void fromMiddleware(const kpsr_dds_bst::BstPacketData & message, T & event) {
        uint8_t * pointer = (uint8_t *) & event;
        for (uint i = 0; i < sizeof(T); i++) {
            pointer[i] = message.data()[i];
        }
    }

    /**
    * @brief toMiddleware
    * @param event
    * @param message
    */
    void toMiddleware(const T & event, kpsr_dds_bst::BstPacketData & message) {
        std::vector<unsigned char> data((unsigned char*)(&event), (unsigned char*)&event + sizeof(T));
        message.data(data);
    }
};

}

#endif // SYSTEM_INITIALIZE_DDS_MAPPER_H
