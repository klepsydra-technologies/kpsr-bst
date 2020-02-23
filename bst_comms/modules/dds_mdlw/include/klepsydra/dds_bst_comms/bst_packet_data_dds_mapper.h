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
