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
#include <time.h>
#include <unistd.h>
#include <getopt.h>

/* KPSR LIBS */

#ifdef KPSR_WITH_ADMIN
#include <klepsydra/rest_interface/hp_rest_admin_container_provider.h>
#endif

#include <klepsydra/dds_core/dds_env.h>

#include <klepsydra/dds_bst_comms/bst_client_dds_provider.h>
#include <klepsydra/dds_bst_comms/bst_dds_topic_names.h>

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

    std::string configKey;
    std::string configTopic;

    yamlEnv.getPropertyString("kpsr_dds_env_topic_name", configTopic);
    yamlEnv.getPropertyString("kpsr_dds_env_key", configKey);

    dds::domain::DomainParticipant dp(0);
    dds::pub::Publisher publisher(dp);
    dds::sub::Subscriber subscriber(dp);

    dds::topic::Topic<kpsr_dds_core::DDSEnvironmentData> topic(dp, configTopic);
    dds::pub::DataWriter<kpsr_dds_core::DDSEnvironmentData> datawriter(publisher, topic);
    dds::sub::DataReader<kpsr_dds_core::DDSEnvironmentData> datareader(subscriber, topic);

    kpsr::dds_mdlw::DDSEnv environment(fileName, configKey, &datawriter, &datareader);

    dds::topic::Topic<kpsr_dds_bst::BstRequestData> bstRequestTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_BST_REQUEST);
    dds::pub::DataWriter<kpsr_dds_bst::BstRequestData> bstRequestDataWriter(publisher, bstRequestTopic);

    dds::topic::Topic<kpsr_dds_bst::WaypointCommandData> bstWaypointCommandTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_BST_WP_CMD);
    dds::pub::DataWriter<kpsr_dds_bst::WaypointCommandData> bstWaypointCommandDataWriter(publisher, bstWaypointCommandTopic);

    dds::topic::Topic<kpsr_dds_bst::BstReplyData> bstReplyTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_BST_REPLY);
    dds::sub::DataReader<kpsr_dds_bst::BstReplyData> bstReplyDataReader(subscriber, bstReplyTopic);

    dds::topic::Topic<kpsr_dds_geometry::PoseEventData> poseTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_POSE_EVENT);
    dds::sub::DataReader<kpsr_dds_geometry::PoseEventData> poseDataReader(subscriber, poseTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> systemInitiazeTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_SYSTEM_INIT);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> systemInitializeReader(subscriber, systemInitiazeTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryPositionTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_POS);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> telemetryPositionReader(subscriber, telemetryPositionTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryOrientationTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_ORI);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> telemetryOrientationReader(subscriber, telemetryOrientationTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> sensorsTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_SENSOR);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> sensorsReader(subscriber, sensorsTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> calibrateSensorTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_CALIBRATE);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> calibrateSensorReader(subscriber, calibrateSensorTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> commandTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_CONTROL_CMD);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> commandReader(subscriber, commandTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> pidTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_CONTROL_PID);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> pidReader(subscriber, pidTopic);

    dds::topic::Topic<kpsr_dds_serialization::OctetData> systemTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_SYSTEM);
    dds::sub::DataReader<kpsr_dds_serialization::OctetData> systemReader(subscriber, systemTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetrySystemTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_SYS);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> telemetrySystemReader(subscriber, telemetrySystemTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryPressureTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_PRES);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> telemetryPressureReader(subscriber, telemetryPressureTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryControlTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_CTRL);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> telemetryControlReader(subscriber, telemetryControlTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryGCSTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_GCS);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> telemetryGCSReader(subscriber, telemetryGCSTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> payloadControlTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_PAYLOAD_CONTROL);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> payloadControlReader(subscriber, payloadControlTopic);

    kpsr::Container * container = nullptr;
#ifdef KPSR_WITH_ADMIN
    kpsr::restapi::RestEndpoint * restEndpoint = nullptr;
    kpsr::high_performance::EventLoopMiddlewareProvider<32> * restEventloopProvider = nullptr;
    kpsr::admin::restapi::EventLoopRestAdminContainerProvider<32> * adminProvider = nullptr;

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
#endif

    kpsr::high_performance::EventLoopMiddlewareProvider<256> eventloopProvider(container);
    kpsr::bst::dds_mdlw::BstClientDDSProvider<256> bstClientDdsProvider(container,
                                                                        eventloopProvider,
                                                                        &bstRequestDataWriter,
                                                                        &bstReplyDataReader,
                                                                        &bstWaypointCommandDataWriter,
                                                                        &poseDataReader,
                                                                        &systemInitializeReader,
                                                                        &telemetryPositionReader,
                                                                        &telemetryOrientationReader,
                                                                        &sensorsReader,
                                                                        &calibrateSensorReader,
                                                                        &commandReader,
                                                                        &pidReader,
                                                                        &systemReader,
                                                                        &telemetrySystemReader,
                                                                        &telemetryPressureReader,
                                                                        &telemetryControlReader,
                                                                        &telemetryGCSReader,
                                                                        &payloadControlReader);

    kpsr::bst::BstClientEventloopProvider<256> bstClientProvider(container, &environment,
                                                                 eventloopProvider,
                                                                 &bstClientDdsProvider);
    BstTestClient bstTestClient(bstClientProvider.getBstClient());

    eventloopProvider.start();
    bstClientDdsProvider.start();
#ifdef KPSR_WITH_ADMIN
    if (enableAdminContainer) {
        restEventloopProvider->start();
        adminProvider->start();
    }

    if (restEndpoint != nullptr) {
        restEndpoint->start();
    }
#endif

    bstClientProvider.start();

    bstTestClient.run();

    bstClientProvider.stop();

#ifdef KPSR_WITH_ADMIN
    if (restEndpoint != nullptr) {
        restEndpoint->shutdown();
    }

    if (enableAdminContainer) {
        adminProvider->stop();
        restEventloopProvider->stop();
    }
#endif

    bstClientDdsProvider.stop();
    eventloopProvider.stop();
    printf("Disconnected, exiting.\n\n");
}
