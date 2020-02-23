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
 * @copyright Klepsydra Robotics 2017-2018.
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

