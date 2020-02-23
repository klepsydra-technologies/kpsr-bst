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
#ifndef BST_SERVER_H
#define BST_SERVER_H

#include <string>

#include <klepsydra/mem_core/basic_scheduler.h>

#include <klepsydra/bst_comms/comm_interface_service.h>
#include <klepsydra/bst_comms/bst2kpsr_adaptor_service.h>
#include <klepsydra/bst_comms/telemetry_pose_service.h>
#include <klepsydra/bst_comms/bst_server_middleware_provider.h>

namespace kpsr
{
namespace bst
{
/**
 * @brief The BstServer class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Server interface to all BST provided messages publishers and request subscriber. Code example:
 */
class BstServer : public Service {
public:

    /**
     * @brief BstServer
     * @param container
     * @param environment
     * @param serverMiddlewareProvider
     */
    BstServer(Container * container,
              Environment * environment,
              BstServerMiddlewareProvider * serverMiddlewareProvider);

protected:

    void start() override;

    void stop() override;

    void execute() override;

private:

    BstServerMiddlewareProvider * _serverMiddlewareProvider;
    Bst2KpsrAdaptorService * _bst2KpsrAdaptorService;
    TelemetryPoseService * _telemetryPoseService;
    CommInterfaceService * _commInterfaceService;
    Bst2KpsrBroadcaster * _bst2KpsrBroadcaster;

    kpsr::mem::BasicScheduler scheduler;
};
}
}

#endif // BST_SERVER_H
