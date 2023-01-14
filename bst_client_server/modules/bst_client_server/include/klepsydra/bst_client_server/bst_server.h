
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

#ifndef BST_SERVER_H
#define BST_SERVER_H

#include <string>
#include <unistd.h>
#include <spdlog/spdlog.h>
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
 * @copyright 2023 Klepsydra Technologies AG
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
