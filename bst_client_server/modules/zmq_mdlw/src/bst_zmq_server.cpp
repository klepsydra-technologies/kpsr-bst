// Copyright 2023 Klepsydra Technologies AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/* STD LIBS */
#include <getopt.h>
#include <string>
#include <time.h>
#include <unistd.h>

/* KPSR LIBS */
#include <klepsydra/serialization/void_caster_mapper.h>

#include <klepsydra/zmq_core/zmq_env.h>

#include <klepsydra/zmq_geometry/pose_event_data_serializer.h>

#include <klepsydra/zmq_bst_comms/bst_server_zmq_provider.h>

#include <klepsydra/bst_comms/cereal/bst_reply_message_serializer.h>
#include <klepsydra/bst_comms/cereal/bst_request_message_serializer.h>
#include <klepsydra/bst_comms/cereal/waypoint_command_message_serializer.h>

#include <klepsydra/bst_client_server/bst_main_helper.h>
#include <klepsydra/bst_client_server/bst_server.h>
#include <klepsydra/bst_client_server/bst_server_user_input.h>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[])
{
    std::string fileName = kpsr::bst::BstMainHelper::getConfFileFromParams(argc, argv);

    kpsr::YamlEnvironment yamlEnv(fileName);
    std::string logFileName, currentDateTime;

    currentDateTime = kpsr::bst::BstMainHelper::currentDateTime();
    yamlEnv.getPropertyString("log_file_path", logFileName);
    logFileName = logFileName + currentDateTime + ".log";

    auto kpsrLogger = spdlog::basic_logger_mt("kpsr_logger", logFileName);
    kpsrLogger->set_level(spdlog::level::debug);
    spdlog::set_default_logger(kpsrLogger);

    std::string configListenUrl;
    std::string configWriteUrl;
    std::string configTopic;

    yamlEnv.getPropertyString("kpsr_zmq_env_listen_url", configListenUrl);
    yamlEnv.getPropertyString("kpsr_zmq_env_write_url", configWriteUrl);
    yamlEnv.getPropertyString("kpsr_zmq_env_topic_name", configTopic);

    //  Prepare our context and publisher
    zmq::context_t context(1);

    //  Socket to talk to server
    spdlog::info("Setup configuration publisher\n");
    zmq::socket_t envPublisher(context, ZMQ_PUB);
    envPublisher.connect(configWriteUrl);

    spdlog::info("Setup configuration subcriber\n");
    zmq::socket_t envSubscriber(context, ZMQ_SUB);
    envSubscriber.connect(configListenUrl);
    envSubscriber.setsockopt(ZMQ_SUBSCRIBE, configTopic.c_str(), configTopic.size());

    kpsr::zmq_mdlw::ZMQEnv environment(&yamlEnv, envPublisher, envSubscriber);

    std::string bstServerPublishUrl;
    std::string bstServerSubscribeUrl;
    std::string bstClientSubscribeUrl;
    std::string bstServerBindAddress;

    environment.getPropertyString("bst_server_publish_url", bstServerPublishUrl);
    environment.getPropertyString("bst_server_subscribe_url", bstServerSubscribeUrl);
    environment.getPropertyString("bst_client_subscribe_url", bstClientSubscribeUrl);
    environment.getPropertyString("bst_server_bind_address", bstServerBindAddress);

    zmq::socket_t bstServerPublisher(context, ZMQ_PUB);
    bstServerPublisher.bind(bstServerPublishUrl);
    bstServerPublisher.bind(bstServerBindAddress);

    int numberServerSubscribers = 3;
    std::vector<std::shared_ptr<zmq::socket_t>> bstServerSubscribers(numberServerSubscribers);
    for (int i = 0; i < numberServerSubscribers; i++) {
        bstServerSubscribers[i] = std::shared_ptr<zmq::socket_t>(
            new zmq::socket_t(context, ZMQ_SUB));
        bstServerSubscribers[i]->connect(bstServerSubscribeUrl);
        bstServerSubscribers[i]->setsockopt(ZMQ_SUBSCRIBE, "", 0);
    }

    int numberClientSubscribers = 2;
    std::vector<std::shared_ptr<zmq::socket_t>> bstClientSubscribers(numberClientSubscribers);
    for (int i = 0; i < numberClientSubscribers; i++) {
        bstClientSubscribers[i] = std::shared_ptr<zmq::socket_t>(
            new zmq::socket_t(context, ZMQ_SUB));
        bstClientSubscribers[i]->connect(bstClientSubscribeUrl);
        bstClientSubscribers[i]->setsockopt(ZMQ_SUBSCRIBE, "", 0);
    }

    kpsr::Container *container = nullptr;

    kpsr::zmq_mdlw::FromZmqMiddlewareProvider fromZmqMiddlewareProvider;
    kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZmqMiddlewareProvider(container, bstServerPublisher);

    kpsr::high_performance::EventLoopMiddlewareProvider<4096> eventloopProvider(container);

    kpsr::bst::zmq_mdlw::BstServerZMQProvider<4096>
        bstServerZmqProvider(eventloopProvider,
                             fromZmqMiddlewareProvider,
                             toZmqMiddlewareProvider,
                             *bstClientSubscribers[0].get(),
                             *bstClientSubscribers[1].get(),
                             *bstServerSubscribers[0].get(),
                             *bstServerSubscribers[1].get(),
                             *bstServerSubscribers[2].get(),
                             100);

    kpsr::bst::BstServer bstServer(container, &environment, &bstServerZmqProvider);

    BstServerUserInput bstServerUserInput;

    eventloopProvider.start();
    bstServerZmqProvider.start();

    bstServer.startup();
    bstServerUserInput.run();
    bstServer.shutdown();

    bstServerZmqProvider.stop();
    eventloopProvider.stop();

    printf("Disconnected, exiting.\n\n");
}
