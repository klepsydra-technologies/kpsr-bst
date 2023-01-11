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
#include <string>
#include <getopt.h>
#include <time.h>

/* KPSR LIBS */
#include <klepsydra/core/yaml_environment.h>

#include <klepsydra/mem_core/mem_env.h>

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

    kpsr::bst::mem::BstClientServerMemProvider bstClientServerMemProvider(container);

    kpsr::bst::BstServer bstServer(container, &environment, &bstClientServerMemProvider);
    spdlog::info("Server Started.");

    kpsr::high_performance::EventLoopMiddlewareProvider<32> eventloopProvider(container);
    kpsr::bst::BstClientEventloopProvider<32> bstClientProvider(container,
                                                                &environment,
                                                                eventloopProvider,
                                                                &bstClientServerMemProvider);

    BstTestClient bstTestClient(bstClientProvider.getBstClient());

    eventloopProvider.start();
    bstClientServerMemProvider.start();

    bstServer.startup();
    bstClientProvider.start();

    bstTestClient.run();

    bstClientProvider.stop();
    bstServer.shutdown();

    bstClientServerMemProvider.stop();
    eventloopProvider.stop();

    printf("Disconnected, exiting.\n\n");
}
