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
#ifndef BST_SERVER_DDS_PROVIDER_H
#define BST_SERVER_DDS_PROVIDER_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/dds_serialization/primitive_type_dds_mapper.h>

#include <klepsydra/dds_core/to_dds_middleware_provider.h>
#include <klepsydra/dds_core/from_dds_middleware_provider.h>

#include <klepsydra/dds_geometry/pose_event_dds_mapper.h>

#include <klepsydra/bst_comms/bst_server_middleware_provider.h>

#include <klepsydra/dds_bst_comms/bst_packet_data_dds_mapper.h>

#include <klepsydra/bst_comms/dds/bst_reply_message_dds_mapper.h>
#include <klepsydra/bst_comms/dds/bst_request_message_dds_mapper.h>
#include <klepsydra/bst_comms/dds/waypoint_command_message_dds_mapper.h>
#include <klepsydra/bst_comms/bst_kpsr_event_names.h>

namespace kpsr
{
namespace bst
{
namespace dds_mdlw
{
template <std::size_t BufferSize>
/**
 * @brief The BstServerDDSProvider class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-dds-bst
 *
 * @details Server DDS implementation to all BST provided messages publishers and request subscribers. Code example:
@code
    // Instantiation of DDS pub / subs
    dds::domain::DomainParticipant dp(0);
    dds::pub::Publisher publisher(dp);
    dds::sub::Subscriber subscriber(dp);

    dds::topic::Topic<kpsr_dds_bst::BstRequestData> bstRequestTopic(dp, EVENT_NAME_BST_REQUEST);
    dds::sub::DataReader<kpsr_dds_bst::BstRequestData> bstRequestDataReader(subscriber, bstRequestTopic);

    dds::topic::Topic<kpsr_dds_bst::WaypointCommandData> bstWaypointCommandTopic(dp, EVENT_NAME_BST_WP_CMD);
    dds::sub::DataReader<kpsr_dds_bst::WaypointCommandData> bstWaypointCommandDataReader(subscriber, bstWaypointCommandTopic);

    dds::topic::Topic<kpsr_dds_bst::BstReplyData> bstReplyTopic(dp, EVENT_NAME_BST_REPLY);
    dds::pub::DataWriter<kpsr_dds_bst::BstReplyData> bstReplyDataWriter(publisher, bstReplyTopic);

    ...
    ...

    // Local event loop for the server
    kpsr::high_performance::EventLoopMiddlewareProvider<256> eventloopProvider(nullptr);

    // BST Server instantiation
    kpsr::bst::dds::BstServerDDSProvider<256> bstServerDdsProvider(
                nullptr,
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

    kpsr::dds_mdlw::DDSEnv environment(fileName, "dds_bst_dds_server", &datawriter, &datareader);

    kpsr::bst::BstServiceWizard serviceWizard(nullptr, &environment, &bstServerDdsProvider);

    BstServer bstServer(&serviceWizard);

    eventloopProvider.start();
    bstServer.start();

@endcode
 */
class BstServerDDSProvider : public BstServerMiddlewareProvider {
public:

    /**
     * @brief BstServerDDSProvider
     * @param container
     * @param eventloopProvider
     * @param bstReplyDataWriter
     * @param poseDataWriter
     * @param systemInitializeWriter
     * @param telemetryPositionWriter
     * @param telemetryOrientationWriter
     * @param sensorsWriter
     * @param calibrateSensorWriter
     * @param commandWriter
     * @param pidWriter
     * @param systemWriter
     * @param telemetrySystemWriter
     * @param telemetryPressureWriter
     * @param telemetryControlWriter
     * @param telemetryGCSWriter
     * @param payloadControlWriter
     * @param bstRequestDataReader
     * @param bstWaypointCommandDataReader
     * @param systemInitializeReader
     * @param telemetryPositionReader
     * @param telemetryOrientationReader
     */
    BstServerDDSProvider(Container * container,
                         kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & eventloopProvider,
                         dds::pub::DataWriter<kpsr_dds_bst::BstReplyData> * bstReplyDataWriter,
                         dds::pub::DataWriter<kpsr_dds_geometry::PoseEventData> * poseDataWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * systemInitializeWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * telemetryPositionWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * telemetryOrientationWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * sensorsWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * calibrateSensorWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * commandWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * pidWriter,
                         dds::pub::DataWriter<kpsr_dds_serialization::OctetData> * systemWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * telemetrySystemWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * telemetryPressureWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * telemetryControlWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * telemetryGCSWriter,
                         dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * payloadControlWriter,
                         dds::sub::DataReader<kpsr_dds_bst::BstRequestData> * bstRequestDataReader,
                         dds::sub::DataReader<kpsr_dds_bst::WaypointCommandData> * bstWaypointCommandDataReader,
                         dds::sub::DataReader<kpsr_dds_bst::BstPacketData> * systemInitializeReader,
                         dds::sub::DataReader<kpsr_dds_bst::BstPacketData> * telemetryPositionReader,
                         dds::sub::DataReader<kpsr_dds_bst::BstPacketData> * telemetryOrientationReader)
        : _eventloopProvider(eventloopProvider)
        , _toDDSMiddlewareProvider(container)
        , _bstReplyDataWriter(bstReplyDataWriter)
        , _poseDataWriter(poseDataWriter)
        , _systemInitializeWriter(systemInitializeWriter)
        , _telemetryPositionWriter(telemetryPositionWriter)
        , _telemetryOrientationWriter(telemetryOrientationWriter)
        , _sensorsWriter(sensorsWriter)
        , _calibrateSensorWriter(calibrateSensorWriter)
        , _commandWriter(commandWriter)
        , _pidWriter(pidWriter)
        , _systemWriter(systemWriter)
        , _telemetrySystemWriter(telemetrySystemWriter)
        , _telemetryPressureWriter(telemetryPressureWriter)
        , _telemetryControlWriter(telemetryControlWriter)
        , _telemetryGCSWriter(telemetryGCSWriter)
        , _payloadControlWriter(payloadControlWriter)
        , _bstRequestDataReader(bstRequestDataReader)
        , _bstWaypointCommandDataReader(bstWaypointCommandDataReader)
        , _systemInitializeReader(systemInitializeReader)
        , _telemetryPositionReader(telemetryPositionReader)
        , _telemetryOrientationReader(telemetryOrientationReader)
    {}

    /**
     * @brief start
     */
    void start() override {
        {
            kpsr::Publisher<kpsr::bst::BstRequestMessage> * publisher = _eventloopProvider.template getPublisher<kpsr::bst::BstRequestMessage>(EVENT_NAME_BST_REQUEST, 0, nullptr, nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_BST_REQUEST, _bstRequestDataReader, true, publisher);
        }

        {
            kpsr::Publisher<kpsr::bst::WaypointCommandMessage> * publisher = _eventloopProvider.template getPublisher<kpsr::bst::WaypointCommandMessage>(EVENT_NAME_BST_WP_CMD, 0, nullptr, nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_BST_WP_CMD, _bstWaypointCommandDataReader, true, publisher);
        }

        {
            kpsr::Publisher<SystemInitialize_t> * publisher = _eventloopProvider.template getPublisher<SystemInitialize_t>(EVENT_NAME_SYSTEM_INIT, 0, nullptr, nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_SYSTEM_INIT, _systemInitializeReader, true, publisher);
        }

        {
            kpsr::Publisher<TelemetryPosition_t> * publisher = _eventloopProvider.template getPublisher<TelemetryPosition_t>(EVENT_NAME_TELEMETRY_POS, 0, nullptr, nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_TELEMETRY_POS, _telemetryPositionReader, true, publisher);
        }

        {
            kpsr::Publisher<TelemetryOrientation_t> * publisher = _eventloopProvider.template getPublisher<TelemetryOrientation_t>(EVENT_NAME_TELEMETRY_ORI, 0, nullptr, nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_TELEMETRY_ORI, _telemetryOrientationReader, true, publisher);
        }
    }

    /**
     * @brief stop
     */
    void stop() override {
        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_BST_REQUEST, _bstRequestDataReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_BST_WP_CMD, _bstWaypointCommandDataReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_SYSTEM_INIT, _systemInitializeReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_TELEMETRY_POS, _telemetryPositionReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_TELEMETRY_ORI, _telemetryOrientationReader);

    }

    /**
     * @brief getBstRequestMessageSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::bst::BstRequestMessage> * getBstRequestMessageSubscriber() {
        return _eventloopProvider.template getSubscriber<kpsr::bst::BstRequestMessage>(EVENT_NAME_BST_REQUEST);
    }

    /**
     * @brief getBstWaypointCommandMessageSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::bst::WaypointCommandMessage> * getBstWaypointCommandMessageSubscriber() {
        return _eventloopProvider.template getSubscriber<kpsr::bst::WaypointCommandMessage>(EVENT_NAME_BST_WP_CMD);
    }

    /**
     * @brief getBst2KpsrReplyMessagePublisher
     * @return
     */
    kpsr::Publisher<kpsr::bst::BstReplyMessage> * getBst2KpsrReplyMessagePublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<kpsr::bst::BstReplyMessage, kpsr_dds_bst::BstReplyData>(EVENT_NAME_BST_REPLY, 0, nullptr, _bstReplyDataWriter);
    }

    /**
     * @brief getPoseEventDataPublisher
     * @return
     */
    kpsr::Publisher<kpsr::geometry::PoseEventData> * getPoseEventDataPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<kpsr::geometry::PoseEventData, kpsr_dds_geometry::PoseEventData>(EVENT_NAME_POSE_EVENT, 0, nullptr, _poseDataWriter);
    }

    /**
     * @brief getSystemInitializeSubscriber
     * @return
     */
    kpsr::Subscriber<SystemInitialize_t> * getSystemInitializeSubscriber() {
        return _eventloopProvider.template getSubscriber<SystemInitialize_t>(EVENT_NAME_SYSTEM_INIT);
    }

    /**
     * @brief getSystemInitializePublisher
     * @return
     */
    kpsr::Publisher<SystemInitialize_t> * getSystemInitializePublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<SystemInitialize_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_SYSTEM_INIT, 0, nullptr, _systemInitializeWriter);
    }

    /**
     * @brief getTelemetryPositionSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryPosition_t> * getTelemetryPositionSubscriber() {
        return _eventloopProvider.template getSubscriber<TelemetryPosition_t>(EVENT_NAME_TELEMETRY_POS);
    }

    /**
     * @brief getTelemetryPositionPublisher
     * @return
     */
    kpsr::Publisher<TelemetryPosition_t> * getTelemetryPositionPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<TelemetryPosition_t, kpsr_dds_bst::BstPacketData>("dds_bst_telemetry_position", 0, nullptr, _telemetryPositionWriter);
    }

    /**
     * @brief getTelemetryOrientationSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryOrientation_t> * getTelemetryOrientationSubscriber() {
        return _eventloopProvider.template getSubscriber<TelemetryOrientation_t>(EVENT_NAME_TELEMETRY_ORI);
    }

    /**
     * @brief getTelemetryOrientationPublisher
     * @return
     */
    kpsr::Publisher<TelemetryOrientation_t> * getTelemetryOrientationPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<TelemetryOrientation_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_TELEMETRY_ORI, 0, nullptr, _telemetryPositionWriter);
    }

    /**
     * @brief getSensorPublisher
     * @return
     */
    kpsr::Publisher<Sensors_t> * getSensorPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<Sensors_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_SENSOR, 0, nullptr, _sensorsWriter);
    }

    /**
     * @brief getCalibratePublisher
     * @return
     */
    kpsr::Publisher<CalibrateSensor_t> * getCalibratePublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<CalibrateSensor_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_CALIBRATE, 0, nullptr, _calibrateSensorWriter);
    }

    /**
     * @brief getControlCommandPublisher
     * @return
     */
    kpsr::Publisher<Command_t> * getControlCommandPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<Command_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_CONTROL_CMD, 0, nullptr, _commandWriter);
    }

    /**
     * @brief getControlPidPublisher
     * @return
     */
    kpsr::Publisher<PID_t> * getControlPidPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<PID_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_CONTROL_PID, 0, nullptr, _pidWriter);
    }

    /**
     * @brief getSystemPublisher
     * @return
     */
    kpsr::Publisher<uint8_t> * getSystemPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<uint8_t, kpsr_dds_serialization::OctetData>(EVENT_NAME_SYSTEM, 0, nullptr, _systemWriter);
    }

    /**
     * @brief getTelemetrySystemPublisher
     * @return
     */
    kpsr::Publisher<TelemetrySystem_t> * getTelemetrySystemPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<TelemetrySystem_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_TELEMETRY_SYS, 0, nullptr, _telemetrySystemWriter);
    }

    /**
     * @brief getTelemetryPressurePublisher
     * @return
     */
    kpsr::Publisher<TelemetryPressure_t> * getTelemetryPressurePublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<TelemetryPressure_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_TELEMETRY_PRES, 0, nullptr, _telemetryPressureWriter);
    }

    /**
     * @brief getTelemetryControlPublisher
     * @return
     */
    kpsr::Publisher<::bst::comms::TelemetryControl_t> * getTelemetryControlPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<::bst::comms::TelemetryControl_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_TELEMETRY_CTRL, 0, nullptr, _telemetryControlWriter);
    }

    /**
     * @brief getTelemetryGCSPublisher
     * @return
     */
    kpsr::Publisher<gcs::TelemetryGCS_t> * getTelemetryGCSPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<gcs::TelemetryGCS_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_TELEMETRY_GCS, 0, nullptr, _telemetryGCSWriter);
    }

    /**
     * @brief getPayloadControlPublisher
     * @return
     */
    kpsr::Publisher<PayloadControl_t> * getPayloadControlPublisher() {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<PayloadControl_t, kpsr_dds_bst::BstPacketData>(EVENT_NAME_PAYLOAD_CONTROL, 0, nullptr, _payloadControlWriter);
    }

private:
    kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & _eventloopProvider;

    kpsr::dds_mdlw::ToDDSMiddlewareProvider _toDDSMiddlewareProvider;
    kpsr::dds_mdlw::FromDDSMiddlewareProvider _fromDDSMiddlewareProvider;

    dds::pub::DataWriter<kpsr_dds_bst::BstReplyData> * _bstReplyDataWriter;
    dds::pub::DataWriter<kpsr_dds_geometry::PoseEventData> * _poseDataWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _systemInitializeWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _telemetryPositionWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _telemetryOrientationWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _sensorsWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _calibrateSensorWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _commandWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _pidWriter;
    dds::pub::DataWriter<kpsr_dds_serialization::OctetData> * _systemWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _telemetrySystemWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _telemetryPressureWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _telemetryControlWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _telemetryGCSWriter;
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> * _payloadControlWriter;
    dds::sub::DataReader<kpsr_dds_bst::BstRequestData> * _bstRequestDataReader;
    dds::sub::DataReader<kpsr_dds_bst::WaypointCommandData> * _bstWaypointCommandDataReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> * _systemInitializeReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> * _telemetryPositionReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> * _telemetryOrientationReader;
};
}
}
}

#endif // BST_SERVER_DDS_PROVIDER_H

