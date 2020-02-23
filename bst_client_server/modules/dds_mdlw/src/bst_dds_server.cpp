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
#include <unistd.h>

/* KPSR LIBS */
#include <klepsydra/rest_interface/hp_rest_admin_container_provider.h>

#include <klepsydra/dds_core/dds_env.h>

#include <klepsydra/dds_bst_comms/bst_server_dds_provider.h>
#include <klepsydra/dds_bst_comms/bst_dds_topic_names.h>

#include <klepsydra/bst_client_server/bst_server.h>
#include <klepsydra/bst_client_server/bst_main_helper.h>

#include <klepsydra/bst_client_server/bst_server_user_input.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

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
    dds::sub::DataReader<kpsr_dds_bst::BstRequestData> bstRequestDataReader(subscriber, bstRequestTopic);

    dds::topic::Topic<kpsr_dds_bst::WaypointCommandData> bstWaypointCommandTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_BST_WP_CMD);
    dds::sub::DataReader<kpsr_dds_bst::WaypointCommandData> bstWaypointCommandDataReader(subscriber, bstWaypointCommandTopic);

    dds::topic::Topic<kpsr_dds_bst::BstReplyData> bstReplyTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_BST_REPLY);
    dds::pub::DataWriter<kpsr_dds_bst::BstReplyData> bstReplyDataWriter(publisher, bstReplyTopic);

    dds::topic::Topic<kpsr_dds_geometry::PoseEventData> poseTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_POSE_EVENT);
    dds::pub::DataWriter<kpsr_dds_geometry::PoseEventData> poseDataWriter(publisher, poseTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> systemInitiazeTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_SYSTEM_INIT);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> systemInitializeWriter(publisher, systemInitiazeTopic);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> systemInitializeReader(subscriber, systemInitiazeTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryPositionTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_POS);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> telemetryPositionWriter(publisher, telemetryPositionTopic);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> telemetryPositionReader(subscriber, telemetryPositionTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryOrientationTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_ORI);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> telemetryOrientationWriter(publisher, telemetryOrientationTopic);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> telemetryOrientationReader(subscriber, telemetryOrientationTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> sensorsTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_SENSOR);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> sensorsWriter(publisher, sensorsTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> calibrateSensorTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_CALIBRATE);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> calibrateSensorWriter(publisher, calibrateSensorTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> commandTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_CONTROL_CMD);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> commandWriter(publisher, commandTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> pidTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_CONTROL_PID);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> pidWriter(publisher, pidTopic);

    dds::topic::Topic<kpsr_dds_serialization::OctetData> systemTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_SYSTEM);
    dds::pub::DataWriter<kpsr_dds_serialization::OctetData> systemWriter(publisher, systemTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetrySystemTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_SYS);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> telemetrySystemWriter(publisher, telemetrySystemTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryPressureTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_PRES);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> telemetryPressureWriter(publisher, telemetryPressureTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryControlTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_CTRL);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> telemetryControlWriter(publisher, telemetryControlTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> telemetryGCSTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_TELEMETRY_GCS);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> telemetryGCSWriter(publisher, telemetryGCSTopic);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> payloadControlTopic(dp, kpsr::bst::dds_mdlw::DDS_TOPIC_NAME_PAYLOAD_CONTROL);
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> payloadControlWriter(publisher, payloadControlTopic);

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

    kpsr::high_performance::EventLoopMiddlewareProvider<256> eventloopProvider(container);

    kpsr::bst::dds_mdlw::BstServerDDSProvider<256> bstServerDdsProvider(container,
                                                                        eventloopProvider,
                                                                        &bstReplyDataWriter,
                                                                        &poseDataWriter,
                                                                        &systemInitializeWriter,
                                                                        &telemetryPositionWriter,
                                                                        &telemetryOrientationWriter,
                                                                        &sensorsWriter,
                                                                        &calibrateSensorWriter,
                                                                        &commandWriter,
                                                                        &pidWriter,
                                                                        &systemWriter,
                                                                        &telemetrySystemWriter,
                                                                        &telemetryPressureWriter,
                                                                        &telemetryControlWriter,
                                                                        &telemetryGCSWriter,
                                                                        &payloadControlWriter,
                                                                        &bstRequestDataReader,
                                                                        &bstWaypointCommandDataReader,
                                                                        &systemInitializeReader,
                                                                        &telemetryPositionReader,
                                                                        &telemetryOrientationReader);

    kpsr::bst::BstServer bstServer(container, &environment, &bstServerDdsProvider);

    BstServerUserInput bstServerUserInput;

    eventloopProvider.start();
    bstServerDdsProvider.start();
    if (enableAdminContainer) {
        restEventloopProvider->start();
        adminProvider->start();
    }

    if (restEndpoint != nullptr) {
        restEndpoint->start();
    }

    bstServer.startup();
    bstServerUserInput.run();
    bstServer.shutdown();

    if (restEndpoint != nullptr) {
        restEndpoint->shutdown();
    }

    if (enableAdminContainer) {
        adminProvider->stop();
        restEventloopProvider->stop();
    }

    bstServerDdsProvider.stop();
    eventloopProvider.stop();

    printf("Disconnected, exiting.\n\n");
}
