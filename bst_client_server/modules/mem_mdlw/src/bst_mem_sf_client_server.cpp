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
/* STD LIBS */
#include <string>
#include <getopt.h>
#include <time.h>

/* KPSR LIBS */
#include <klepsydra/core/yaml_environment.h>

#include <klepsydra/mem_core/mem_env.h>

#ifdef KPSR_WITH_ADMIN
#include <klepsydra/rest_interface/basic_rest_admin_container_provider.h>
#include <klepsydra/trajectory_monitoring/bst_trajectory_data_provider.h>
#include <klepsydra/mem_trajectory_monitoring/sf_rest_traj_mon_container_provider.h>
#endif

#include <klepsydra/mem_bst_comms/bst_client_server_mem_provider.h>
#include <klepsydra/bst_client_server/bst_test_client.h>
#include <klepsydra/bst_client_server/bst_server.h>
#include <klepsydra/bst_client_server/bst_main_helper.h>
#include <klepsydra/bst_client_server/bst_client_eventloop_provider.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

int main(int argc, char *argv[])
{
    std::string fileName = kpsr::bst::BstMainHelper::getConfFileFromParams(argc, argv);

    kpsr::YamlEnvironment environment(fileName);

    std::string logFileName;
    environment.getPropertyString("log_file_path", logFileName);
    auto  kpsrLogger = spdlog::basic_logger_mt("kpsr_logger", logFileName);
    spdlog::set_default_logger(kpsrLogger);

    kpsr::Container * container = nullptr;
#ifdef KPSR_WITH_ADMIN
    kpsr::restapi::RestEndpoint * restEndpoint = nullptr;
    kpsr::admin::restapi::BasicRestAdminContainerProvider * adminProvider = nullptr;

    bool enableAdminContainer;
    environment.getPropertyBool("admin_container_enable", enableAdminContainer);
    if (enableAdminContainer) {
        if (restEndpoint == nullptr) {
            int restPort;
            environment.getPropertyInt("rest_port", restPort);

            int noThreads;
            environment.getPropertyInt("rest_number_threads", noThreads);

            restEndpoint = new kpsr::restapi::RestEndpoint(noThreads, restPort);
        }
        spdlog::info("starting the admin container now...");
        adminProvider = new kpsr::admin::restapi::BasicRestAdminContainerProvider(*restEndpoint, &environment, "BST_Container");
        container = &adminProvider->getContainer();
    }
#endif

    kpsr::bst::mem::BstClientServerMemProvider bstClientServerMemProvider(container);

    kpsr::bst::BstServer bstServer(container, &environment, &bstClientServerMemProvider);
    spdlog::info("Server Started.");

    kpsr::high_performance::EventLoopMiddlewareProvider<32> eventloopProvider(container);
    kpsr::bst::BstClientEventloopProvider<32> bstClientProvider(container,
                                                                &environment,
                                                                eventloopProvider,
                                                                &bstClientServerMemProvider);

#ifdef KPSR_WITH_ADMIN
    kpsr::trajectory::bst::BstTrajectoryDataProvider * trajectoryProvider = nullptr;
    kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider * trajectoryMonitoring = nullptr;

    bool enableTrajectoryMonitoring;
    environment.getPropertyBool("trajectory_monitor_enable", enableTrajectoryMonitoring);
    if (enableTrajectoryMonitoring) {
        if (restEndpoint == nullptr) {
            int restPort;
            environment.getPropertyInt("rest_port", restPort);

            int noThreads;
            environment.getPropertyInt("rest_number_threads", noThreads);

            restEndpoint = new kpsr::restapi::RestEndpoint(noThreads, restPort);
        }
        spdlog::info("starting the trajectory_monitor now...");
        trajectoryProvider = new kpsr::trajectory::bst::BstTrajectoryDataProvider(
                    &environment,
                    bstClientServerMemProvider.getTelemetryPositionSubscriber(),
                    bstClientServerMemProvider.getTelemetryOrientationSubscriber(),
                    bstClientServerMemProvider.getPoseEventDataSubscriber(),
                    bstClientServerMemProvider.getBstWaypointCommandMessageSubscriber(),
                    bstClientServerMemProvider.getBstRequestMessageSubscriber(), true);

        trajectoryMonitoring = new kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider(
                    * restEndpoint, trajectoryProvider, &environment,
                    "bst_mem_sf_client_server");
    }
#endif

    BstTestClient bstTestClient(bstClientProvider.getBstClient());

    eventloopProvider.start();
    bstClientServerMemProvider.start();
#ifdef KPSR_WITH_ADMIN
    if (enableAdminContainer) {
        adminProvider->start();
    }

    if (enableTrajectoryMonitoring) {
        trajectoryProvider->start();
        trajectoryMonitoring->start();
    }

    if (restEndpoint != nullptr) {
        restEndpoint->start();
    }
#endif

    bstServer.startup();
    bstClientProvider.start();

    bstTestClient.run();

    bstClientProvider.stop();
    bstServer.shutdown();

#ifdef KPSR_WITH_ADMIN
    if (restEndpoint != nullptr) {
        restEndpoint->shutdown();
    }

    if (enableTrajectoryMonitoring) {
        trajectoryMonitoring->stop();
        trajectoryProvider->stop();
    }

    if (enableAdminContainer) {
        adminProvider->stop();
    }
#endif

    bstClientServerMemProvider.stop();
    eventloopProvider.stop();

    printf("Disconnected, exiting.\n\n");
}
