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

/* KPSR LIBS */
#include <klepsydra/core/yaml_environment.h>

#include <klepsydra/mem_core/mem_env.h>

#include <klepsydra/mem_bst_comms/bst_client_server_eventloop_provider.h>

#include <klepsydra/bst_client_server/bst_client_eventloop_provider.h>
#include <klepsydra/bst_client_server/bst_main_helper.h>
#include <klepsydra/bst_client_server/bst_server.h>
#include <klepsydra/bst_client_server/bst_test_client.h>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[])
{
    std::string fileName = kpsr::bst::BstMainHelper::getConfFileFromParams(argc, argv);

    kpsr::YamlEnvironment environment(fileName);
    std::string logFileName, currentDateTime;

    currentDateTime = kpsr::bst::BstMainHelper::currentDateTime();
    environment.getPropertyString("log_file_path", logFileName);
    logFileName = logFileName + currentDateTime + ".log";
    auto kpsrLogger = spdlog::basic_logger_mt("kpsr_logger", logFileName);
    spdlog::set_default_logger(kpsrLogger);
    spdlog::set_pattern("[%c] [%H:%M:%S %f] [%n] [%l] [%t] %v");
    spdlog::set_level(spdlog::level::debug); // Set global log level to info

    kpsr::Container *container = nullptr;

    kpsr::high_performance::EventLoopMiddlewareProvider<1024> eventloopProvider(container);
    kpsr::bst::mem::BstClientServerEventLoopProvider<1024> bstClientServerMemProvider(
        eventloopProvider);

    kpsr::bst::BstServer bstServer(container, &environment, &bstClientServerMemProvider);

    kpsr::bst::BstClientEventloopProvider<1024> bstClientProvider(container,
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
