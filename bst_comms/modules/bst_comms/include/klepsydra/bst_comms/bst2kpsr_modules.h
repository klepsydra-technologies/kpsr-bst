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
