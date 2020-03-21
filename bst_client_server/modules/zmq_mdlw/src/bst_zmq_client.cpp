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
#include <time.h>
#include <unistd.h>
#include <getopt.h>

/* KPSR LIBS */
#include <klepsydra/serialization/void_caster_mapper.h>

#include <klepsydra/zmq_core/zmq_env.h>

#include <klepsydra/zmq_geometry/pose_event_data_serializer.h>

#include <klepsydra/rest_interface/hp_rest_admin_container_provider.h>

#include <klepsydra/zmq_bst_comms/bst_client_zmq_provider.h>

#include <klepsydra/bst_comms/cereal/bst_request_message_serializer.h>
#include <klepsydra/bst_comms/cereal/bst_reply_message_serializer.h>
#include <klepsydra/bst_comms/cereal/waypoint_command_message_serializer.h>

#include <klepsydra/bst_client_server/bst_test_client.h>
#include <klepsydra/bst_client_server/bst_main_helper.h>
#include <klepsydra/bst_client_server/bst_client_eventloop_provider.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

void printHelp() {
    printf("Usage: test [OPTIONS]\n");
    printf("  Klepsydra paramerters:\n");
    printf("    -a <admint port>   : default null\n");
    printf("    -f <configuration file> : default null\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    std::string fileName = kpsr::bst::BstMainHelper::getConfFileFromParams(argc, argv);

    kpsr::YamlEnvironment yamlEnv(fileName);

    std::string logFileName;
    yamlEnv.getPropertyString("log_file_path", logFileName);
    auto  kpsrLogger = spdlog::basic_logger_mt("kpsr_logger", logFileName);
    kpsrLogger->set_level(spdlog::level::debug);
    spdlog::set_default_logger(kpsrLogger);

    std::string configListenUrl;
    std::string configWriteUrl;
    std::string configTopic;

    yamlEnv.getPropertyString("kpsr_zmq_env_listen_url", configListenUrl);
    yamlEnv.getPropertyString("kpsr_zmq_env_write_url", configWriteUrl);
    yamlEnv.getPropertyString("kpsr_zmq_env_topic_name", configTopic);

    //  Prepare our context and publisher
    zmq::context_t context (1);

    //  Socket to talk to server
    spdlog::info("Setup configuration publisher\n");
    zmq::socket_t envPublisher (context, ZMQ_PUB);
    envPublisher.connect(configWriteUrl);

    spdlog::info("Setup configuration subcriber\n");
    zmq::socket_t envSubscriber (context, ZMQ_SUB);
    envSubscriber.connect(configListenUrl);
    envSubscriber.setsockopt(ZMQ_SUBSCRIBE, configTopic.c_str(), configTopic.size());

    kpsr::zmq_mdlw::ZMQEnv environment(&yamlEnv, envPublisher, envSubscriber);

    std::string bstClientPublishUrl;
    std::string bstServerSubscribeUrl;
    std::string bstclientBindAddress;

    environment.getPropertyString("bst_client_publish_url", bstClientPublishUrl);
    environment.getPropertyString("bst_server_subscribe_url", bstServerSubscribeUrl);
    environment.getPropertyString("bst_client_bind_address", bstclientBindAddress);

    zmq::socket_t bstClientPublisher(context, ZMQ_PUB);
    bstClientPublisher.bind(bstClientPublishUrl);
    bstClientPublisher.bind(bstclientBindAddress);

    int numberServerSubscribers = 15;
    std::vector<std::shared_ptr<zmq::socket_t>> bstServerSubscribers(numberServerSubscribers);
    for (int i = 0; i < numberServerSubscribers; i ++) {
        bstServerSubscribers[i] = std::shared_ptr<zmq::socket_t>(new zmq::socket_t(context, ZMQ_SUB));
        bstServerSubscribers[i]->connect(bstServerSubscribeUrl);
        bstServerSubscribers[i]->setsockopt(ZMQ_SUBSCRIBE, "", 0);
    }

    kpsr::restapi::RestEndpoint * restEndpoint = nullptr;
    kpsr::high_performance::EventLoopMiddlewareProvider<32> * restEventloopProvider = nullptr;
    kpsr::admin::restapi::EventLoopRestAdminContainerProvider<32> * adminProvider = nullptr;
    kpsr::Container * container = nullptr;

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
        restEventloopProvider = new kpsr::high_performance::EventLoopMiddlewareProvider<32>(nullptr);
        adminProvider = new kpsr::admin::restapi::EventLoopRestAdminContainerProvider<32>(*restEndpoint, * restEventloopProvider, &environment, "BST_Container");
        container = &adminProvider->getContainer();
    }

    kpsr::zmq_mdlw::FromZmqMiddlewareProvider fromZmqMiddlewareProvider;
    kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZmqMiddlewareProvider(container, bstClientPublisher);

    kpsr::high_performance::EventLoopMiddlewareProvider<256> eventloopProvider(container);
    kpsr::bst::zmq_mdlw::BstClientZMQProvider<256> bstClientZmqProvider(eventloopProvider,
                                                                        fromZmqMiddlewareProvider,
                                                                        toZmqMiddlewareProvider,
                                                                        * bstServerSubscribers[0].get(),
                                                                        * bstServerSubscribers[1].get(),
                                                                        * bstServerSubscribers[2].get(),
                                                                        * bstServerSubscribers[3].get(),
                                                                        * bstServerSubscribers[4].get(),
                                                                        * bstServerSubscribers[5].get(),
                                                                        * bstServerSubscribers[6].get(),
                                                                        * bstServerSubscribers[7].get(),
                                                                        * bstServerSubscribers[8].get(),
                                                                        * bstServerSubscribers[9].get(),
                                                                        * bstServerSubscribers[10].get(),
                                                                        * bstServerSubscribers[11].get(),
                                                                        * bstServerSubscribers[12].get(),
                                                                        * bstServerSubscribers[13].get(),
                                                                        * bstServerSubscribers[14].get(),
                                                                        100);

    kpsr::bst::BstClientEventloopProvider<256> bstClientProvider(container, &environment,
                                                                 eventloopProvider,
                                                                 &bstClientZmqProvider);
    BstTestClient bstTestClient(bstClientProvider.getBstClient());

    eventloopProvider.start();
    bstClientZmqProvider.start();
    if (enableAdminContainer) {
        restEventloopProvider->start();
        adminProvider->start();
    }

    if (restEndpoint != nullptr) {
        restEndpoint->start();
    }

    bstClientProvider.start();

    bstTestClient.run();

    bstClientProvider.stop();

    if (restEndpoint != nullptr) {
        restEndpoint->shutdown();
    }

    if (enableAdminContainer) {
        adminProvider->stop();
        restEventloopProvider->stop();
    }
    bstClientZmqProvider.stop();
    eventloopProvider.stop();
    printf("Disconnected, exiting.\n\n");
}
