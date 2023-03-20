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

#ifndef BST_CLIENT_DDS_PROVIDER_H
#define BST_CLIENT_DDS_PROVIDER_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/dds_serialization/primitive_type_dds_mapper.h>

#include <klepsydra/dds_core/from_dds_middleware_provider.h>
#include <klepsydra/dds_core/to_dds_middleware_provider.h>

#include <klepsydra/dds_geometry/pose_event_dds_mapper.h>

#include <klepsydra/bst_comms/bst_client_middleware_provider.h>

#include <klepsydra/dds_bst_comms/bst_packet_data_dds_mapper.h>

#include <klepsydra/bst_comms/bst_kpsr_event_names.h>
#include <klepsydra/bst_comms/dds/bst_reply_message_dds_mapper.h>
#include <klepsydra/bst_comms/dds/bst_request_message_dds_mapper.h>
#include <klepsydra/bst_comms/dds/waypoint_command_message_dds_mapper.h>

namespace kpsr {
namespace bst {
namespace dds_mdlw {
template<std::size_t BufferSize>
/**
 * @brief The BstClientDDSProvider class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-dds-bst
 *
 * @details Client DDS implementation to all BST provided messages subscribers and request publishers. Example code:
@code
    // DDS pub and sub creation
    dds::domain::DomainParticipant dp(0);
    dds::pub::Publisher publisher(dp);
    dds::sub::Subscriber subscriber(dp);

    dds::topic::Topic<kpsr_dds_bst::BstRequestData> bstRequestTopic(dp, "dds_bst_request");
    dds::pub::DataWriter<kpsr_dds_bst::BstRequestData> bstRequestDataWriter(publisher, bstRequestTopic);

    dds::topic::Topic<kpsr_dds_bst::WaypointCommandData> bstWaypointCommandTopic(dp, EVENT_NAME_BST_WP_CMD);
    dds::pub::DataWriter<kpsr_dds_bst::WaypointCommandData> bstWaypointCommandDataWriter(publisher, bstWaypointCommandTopic);

    dds::topic::Topic<kpsr_dds_bst::BstReplyData> bstReplyTopic(dp, EVENT_NAME_BST_REPLY);
    dds::sub::DataReader<kpsr_dds_bst::BstReplyData> bstReplyDataReader(subscriber, bstReplyTopic);

    ...
    ...

    // local event loop instance.
    kpsr::high_performance::EventLoopMiddlewareProvider<256> eventloopProvider(nullptr);

    // BST Client instance.
    kpsr::bst::dds::BstClientDDSProvider<256> bstClientDdsProvider(
                nullptr,
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

    eventloopProvider.start();
    bstClientDdsProvider.start();
@endcode
 */
class BstClientDDSProvider : public BstClientMiddlewareProvider
{
public:
    /**
     * @brief BstClientDDSProvider
     * @param container
     * @param eventloopProvider
     * @param bstRequestDataWriter
     * @param bstReplyDataReader
     * @param bstWaypointCommandDataWriter
     * @param poseDataReader
     * @param systemInitializeReader
     * @param telemetryPositionReader
     * @param telemetryOrientationReader
     * @param sensorsReader
     * @param calibrateSensorReader
     * @param commandReader
     * @param pidReader
     * @param systemReader
     * @param telemetrySystemReader
     * @param telemetryPressureReader
     * @param telemetryControlReader
     * @param telemetryGCSReader
     * @param payloadControlReader
     */
    BstClientDDSProvider(
        Container *container,
        kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> &eventloopProvider,
        dds::pub::DataWriter<kpsr_dds_bst::BstRequestData> *bstRequestDataWriter,
        dds::sub::DataReader<kpsr_dds_bst::BstReplyData> *bstReplyDataReader,
        dds::pub::DataWriter<kpsr_dds_bst::WaypointCommandData> *bstWaypointCommandDataWriter,
        dds::sub::DataReader<kpsr_dds_geometry::PoseEventData> *poseDataReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *systemInitializeReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *telemetryPositionReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *telemetryOrientationReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *sensorsReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *calibrateSensorReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *commandReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *pidReader,
        dds::sub::DataReader<kpsr_dds_serialization::OctetData> *systemReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *telemetrySystemReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *telemetryPressureReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *telemetryControlReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *telemetryGCSReader,
        dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *payloadControlReader)
        : _eventloopProvider(eventloopProvider)
        , _toDDSMiddlewareProvider(container)
        , _bstRequestDataWriter(bstRequestDataWriter)
        , _bstReplyDataReader(bstReplyDataReader)
        , _bstWaypointCommandDataWriter(bstWaypointCommandDataWriter)
        , _poseDataReader(poseDataReader)
        , _systemInitializeReader(systemInitializeReader)
        , _telemetryPositionReader(telemetryPositionReader)
        , _telemetryOrientationReader(telemetryOrientationReader)
        , _sensorsReader(sensorsReader)
        , _calibrateSensorReader(calibrateSensorReader)
        , _commandReader(commandReader)
        , _pidReader(pidReader)
        , _systemReader(systemReader)
        , _telemetrySystemReader(telemetrySystemReader)
        , _telemetryPressureReader(telemetryPressureReader)
        , _telemetryControlReader(telemetryControlReader)
        , _telemetryGCSReader(telemetryGCSReader)
        , _payloadControlReader(payloadControlReader)
    {}

    /**
     * @brief start
     */
    void start() override
    {
        {
            kpsr::Publisher<kpsr::bst::BstReplyMessage> *publisher =
                _eventloopProvider
                    .template getPublisher<kpsr::bst::BstReplyMessage>(EVENT_NAME_BST_REPLY,
                                                                       0,
                                                                       nullptr,
                                                                       nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_BST_REPLY,
                                                       _bstReplyDataReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<kpsr::geometry::PoseEventData> *publisher =
                _eventloopProvider
                    .template getPublisher<kpsr::geometry::PoseEventData>(EVENT_NAME_POSE_EVENT,
                                                                          0,
                                                                          nullptr,
                                                                          nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_POSE_EVENT,
                                                       _poseDataReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<SystemInitialize_t> *publisher =
                _eventloopProvider.template getPublisher<SystemInitialize_t>(EVENT_NAME_SYSTEM_INIT,
                                                                             0,
                                                                             nullptr,
                                                                             nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_SYSTEM_INIT,
                                                       _systemInitializeReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<TelemetryPosition_t> *publisher =
                _eventloopProvider
                    .template getPublisher<TelemetryPosition_t>(EVENT_NAME_TELEMETRY_POS,
                                                                0,
                                                                nullptr,
                                                                nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_TELEMETRY_POS,
                                                       _telemetryPositionReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<TelemetryOrientation_t> *publisher =
                _eventloopProvider
                    .template getPublisher<TelemetryOrientation_t>(EVENT_NAME_TELEMETRY_ORI,
                                                                   0,
                                                                   nullptr,
                                                                   nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_TELEMETRY_ORI,
                                                       _telemetryOrientationReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<Sensors_t> *publisher =
                _eventloopProvider.template getPublisher<Sensors_t>(EVENT_NAME_SENSOR,
                                                                    0,
                                                                    nullptr,
                                                                    nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_SENSOR,
                                                       _sensorsReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<CalibrateSensor_t> *publisher =
                _eventloopProvider.template getPublisher<CalibrateSensor_t>(EVENT_NAME_CALIBRATE,
                                                                            0,
                                                                            nullptr,
                                                                            nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_CALIBRATE,
                                                       _calibrateSensorReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<Command_t> *publisher =
                _eventloopProvider.template getPublisher<Command_t>(EVENT_NAME_CONTROL_CMD,
                                                                    0,
                                                                    nullptr,
                                                                    nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_CONTROL_CMD,
                                                       _commandReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<PID_t> *publisher =
                _eventloopProvider.template getPublisher<PID_t>(EVENT_NAME_CONTROL_PID,
                                                                0,
                                                                nullptr,
                                                                nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_CONTROL_PID,
                                                       _pidReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<uint8_t> *publisher =
                _eventloopProvider.template getPublisher<uint8_t>(EVENT_NAME_SYSTEM,
                                                                  0,
                                                                  nullptr,
                                                                  nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_SYSTEM,
                                                       _systemReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<TelemetrySystem_t> *publisher =
                _eventloopProvider.template getPublisher<TelemetrySystem_t>(EVENT_NAME_TELEMETRY_SYS,
                                                                            0,
                                                                            nullptr,
                                                                            nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_TELEMETRY_SYS,
                                                       _telemetrySystemReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<TelemetryPressure_t> *publisher =
                _eventloopProvider
                    .template getPublisher<TelemetryPressure_t>(EVENT_NAME_TELEMETRY_PRES,
                                                                0,
                                                                nullptr,
                                                                nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_TELEMETRY_PRES,
                                                       _telemetryPressureReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<::bst::comms::TelemetryControl_t> *publisher =
                _eventloopProvider.template getPublisher<::bst::comms::TelemetryControl_t>(
                    EVENT_NAME_TELEMETRY_CTRL, 0, nullptr, nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_TELEMETRY_CTRL,
                                                       _telemetryControlReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<gcs::TelemetryGCS_t> *publisher =
                _eventloopProvider
                    .template getPublisher<gcs::TelemetryGCS_t>(EVENT_NAME_TELEMETRY_GCS,
                                                                0,
                                                                nullptr,
                                                                nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_TELEMETRY_GCS,
                                                       _telemetryGCSReader,
                                                       true,
                                                       publisher);
        }

        {
            kpsr::Publisher<PayloadControl_t> *publisher = _eventloopProvider.template getPublisher<
                PayloadControl_t>(EVENT_NAME_PAYLOAD_CONTROL, 0, nullptr, nullptr);
            _fromDDSMiddlewareProvider.registerToTopic(EVENT_NAME_PAYLOAD_CONTROL,
                                                       _payloadControlReader,
                                                       true,
                                                       publisher);
        }
    }

    /**
     * @brief stop
     */
    void stop() override
    {
        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_BST_REPLY, _bstReplyDataReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_POSE_EVENT, _poseDataReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_SYSTEM_INIT,
                                                       _systemInitializeReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_TELEMETRY_POS,
                                                       _telemetryPositionReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_TELEMETRY_ORI,
                                                       _telemetryOrientationReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_SENSOR, _sensorsReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_CALIBRATE, _calibrateSensorReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_CONTROL_CMD, _commandReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_CONTROL_PID, _pidReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_SYSTEM, _systemReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_TELEMETRY_SYS,
                                                       _telemetrySystemReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_TELEMETRY_PRES,
                                                       _telemetryPressureReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_TELEMETRY_CTRL,
                                                       _telemetryControlReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_TELEMETRY_GCS,
                                                       _telemetryGCSReader);

        _fromDDSMiddlewareProvider.unregisterFromTopic(EVENT_NAME_PAYLOAD_CONTROL,
                                                       _payloadControlReader);
    }

    /**
     * @brief getBstRequestMessagePublisher
     * @return
     */
    kpsr::Publisher<kpsr::bst::BstRequestMessage> *getBstRequestMessagePublisher()
    {
        return _toDDSMiddlewareProvider
            .getToMiddlewareChannel<kpsr::bst::BstRequestMessage, kpsr_dds_bst::BstRequestData>(
                "dds_bst_request", 0, nullptr, _bstRequestDataWriter);
    }

    /**
     * @brief getBst2KpsrReplyMessageSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::bst::BstReplyMessage> *getBst2KpsrReplyMessageSubscriber()
    {
        return _eventloopProvider.template getSubscriber<kpsr::bst::BstReplyMessage>(
            EVENT_NAME_BST_REPLY);
    }

    /**
     * @brief getBstWaypointCommandMessagePublisher
     * @return
     */
    kpsr::Publisher<WaypointCommandMessage> *getBstWaypointCommandMessagePublisher()
    {
        return _toDDSMiddlewareProvider.getToMiddlewareChannel<kpsr::bst::WaypointCommandMessage,
                                                               kpsr_dds_bst::WaypointCommandData>(
            EVENT_NAME_BST_WP_CMD, 0, nullptr, _bstWaypointCommandDataWriter);
    }

    /**
     * @brief getPoseEventDataSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::geometry::PoseEventData> *getPoseEventDataSubscriber()
    {
        return _eventloopProvider.template getSubscriber<kpsr::geometry::PoseEventData>(
            EVENT_NAME_POSE_EVENT);
    }

    /**
     * @brief getSystemInitializeSubscriber
     * @return
     */
    kpsr::Subscriber<SystemInitialize_t> *getSystemInitializeSubscriber()
    {
        return _eventloopProvider.template getSubscriber<SystemInitialize_t>(EVENT_NAME_SYSTEM_INIT);
    }

    /**
     * @brief getTelemetryPositionSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryPosition_t> *getTelemetryPositionSubscriber()
    {
        return _eventloopProvider.template getSubscriber<TelemetryPosition_t>(
            EVENT_NAME_TELEMETRY_POS);
    }

    /**
     * @brief getTelemetryOrientationSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryOrientation_t> *getTelemetryOrientationSubscriber()
    {
        return _eventloopProvider.template getSubscriber<TelemetryOrientation_t>(
            EVENT_NAME_TELEMETRY_ORI);
    }

    /**
     * @brief getSensorSubscriber
     * @return
     */
    kpsr::Subscriber<Sensors_t> *getSensorSubscriber()
    {
        return _eventloopProvider.template getSubscriber<Sensors_t>(EVENT_NAME_SENSOR);
    }

    /**
     * @brief getCalibrateSubscriber
     * @return
     */
    kpsr::Subscriber<CalibrateSensor_t> *getCalibrateSubscriber()
    {
        return _eventloopProvider.template getSubscriber<CalibrateSensor_t>(EVENT_NAME_CALIBRATE);
    }

    /**
     * @brief getControlCommandSubscriber
     * @return
     */
    kpsr::Subscriber<Command_t> *getControlCommandSubscriber()
    {
        return _eventloopProvider.template getSubscriber<Command_t>(EVENT_NAME_CONTROL_CMD);
    }

    /**
     * @brief getControlPidSubscriber
     * @return
     */
    kpsr::Subscriber<PID_t> *getControlPidSubscriber()
    {
        return _eventloopProvider.template getSubscriber<PID_t>(EVENT_NAME_CONTROL_PID);
    }

    /**
     * @brief getSystemSubscriber
     * @return
     */
    kpsr::Subscriber<uint8_t> *getSystemSubscriber()
    {
        return _eventloopProvider.template getSubscriber<uint8_t>(EVENT_NAME_SYSTEM);
    }

    /**
     * @brief getTelemetrySystemSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetrySystem_t> *getTelemetrySystemSubscriber()
    {
        return _eventloopProvider.template getSubscriber<TelemetrySystem_t>(
            EVENT_NAME_TELEMETRY_SYS);
    }

    /**
     * @brief getTelemetryPressureSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryPressure_t> *getTelemetryPressureSubscriber()
    {
        return _eventloopProvider.template getSubscriber<TelemetryPressure_t>(
            EVENT_NAME_TELEMETRY_PRES);
    }

    /**
     * @brief getTelemetryControlSubscriber
     * @return
     */
    kpsr::Subscriber<::bst::comms::TelemetryControl_t> *getTelemetryControlSubscriber()
    {
        return _eventloopProvider.template getSubscriber<::bst::comms::TelemetryControl_t>(
            EVENT_NAME_TELEMETRY_CTRL);
    }

    /**
     * @brief getTelemetryGCSSubscriber
     * @return
     */
    kpsr::Subscriber<gcs::TelemetryGCS_t> *getTelemetryGCSSubscriber()
    {
        return _eventloopProvider.template getSubscriber<gcs::TelemetryGCS_t>(
            EVENT_NAME_TELEMETRY_GCS);
    }

    /**
     * @brief getPayloadControlSubscriber
     * @return
     */
    kpsr::Subscriber<PayloadControl_t> *getPayloadControlSubscriber()
    {
        return _eventloopProvider.template getSubscriber<PayloadControl_t>(
            EVENT_NAME_PAYLOAD_CONTROL);
    }

    /**
     * @brief getScheduler
     * @return
     */
    Scheduler *getScheduler() override { return _eventloopProvider.getScheduler(); }

private:
    kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> &_eventloopProvider;

    kpsr::dds_mdlw::ToDDSMiddlewareProvider _toDDSMiddlewareProvider;
    kpsr::dds_mdlw::FromDDSMiddlewareProvider _fromDDSMiddlewareProvider;

    dds::pub::DataWriter<kpsr_dds_bst::BstRequestData> *_bstRequestDataWriter;
    dds::sub::DataReader<kpsr_dds_bst::BstReplyData> *_bstReplyDataReader;
    dds::pub::DataWriter<kpsr_dds_bst::WaypointCommandData> *_bstWaypointCommandDataWriter;
    dds::sub::DataReader<kpsr_dds_geometry::PoseEventData> *_poseDataReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_systemInitializeReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_telemetryPositionReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_telemetryOrientationReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_sensorsReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_calibrateSensorReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_commandReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_pidReader;
    dds::sub::DataReader<kpsr_dds_serialization::OctetData> *_systemReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_telemetrySystemReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_telemetryPressureReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_telemetryControlReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_telemetryGCSReader;
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> *_payloadControlReader;
};
} // namespace dds_mdlw
} // namespace bst
} // namespace kpsr

#endif // BST_CLIENT_DDS_PROVIDER_H
