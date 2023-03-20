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

#ifndef BST_SERVER_ZMQ_PROVIDER_H
#define BST_SERVER_ZMQ_PROVIDER_H

#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/geometry/pose_event_data.h>

#include <klepsydra/bst_comms/bst_server_middleware_provider.h>

#include <klepsydra/zmq_bst_comms/bst_zmq_topic_names.h>

namespace kpsr {
namespace bst {
namespace zmq_mdlw {
template<std::size_t BufferSize>
/**
 * @brief The BstServerZMQProvider class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-zmq-bst
 *
 * @details Server ZMQ implementation to all BST provided messages publishers and request subscribers.
 * Code example:
@code
    int numberServerSubscribers = 3;
    std::vector<std::shared_ptr<zmq::socket_t>> bstServerSubscribers(numberServerSubscribers);
    for (int i = 0; i < numberServerSubscribers; i ++) {
        bstServerSubscribers[i] = std::shared_ptr<zmq::socket_t>(new zmq::socket_t(context, ZMQ_SUB));
        bstServerSubscribers[i]->connect(bstServerSubscribeUrl);
        bstServerSubscribers[i]->setsockopt(ZMQ_SUBSCRIBE, "", 0);
    }

    int numberClientSubscribers = 2;
    std::vector<std::shared_ptr<zmq::socket_t>> bstClientSubscribers(numberClientSubscribers);
    for (int i = 0; i < numberClientSubscribers; i ++) {
        bstClientSubscribers[i] = std::shared_ptr<zmq::socket_t>(new zmq::socket_t(context, ZMQ_SUB));
        bstClientSubscribers[i]->connect(bstClientSubscribeUrl);
        bstClientSubscribers[i]->setsockopt(ZMQ_SUBSCRIBE, "", 0);
    }

    kpsr::zmq_mdlw::FromZmqMiddlewareProvider fromZmqMiddlewareProvider;
    kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZmqMiddlewareProvider(nullptr, bstServerPublisher);

    std::string configListenUrl = "tcp://localhost:8210";
    std::string configWriteUrl = "tcp://localhost:8200";
    std::string configTopic = "configuration";
    std::string configKey = "test_configuration";

    spdlog::info("Setup configuration publisher");
    zmq::socket_t envPublisher (context, ZMQ_PUB);
    envPublisher.connect(configWriteUrl);

    spdlog::info("Setup configuration subcriber");
    zmq::socket_t envSubscriber (context, ZMQ_SUB);
    envSubscriber.connect(configListenUrl);
    envSubscriber.setsockopt(ZMQ_SUBSCRIBE, configTopic.c_str(), configTopic.size());

    kpsr::zmq_mdlw::ZMQEnv * environment = new kpsr::zmq_mdlw::ZMQEnv(fileName, configKey, configTopic, envPublisher, envSubscriber);

    kpsr::high_performance::EventLoopMiddlewareProvider<256> eventloopProvider(container);

    bool running = true;
    kpsr::bst::zmq::BstServerZMQProvider<256> bstServerZmqProvider(eventloopProvider,
                                                                      fromZmqMiddlewareProvider,
                                                                      toZmqMiddlewareProvider,
                                                                      * bstClientSubscribers[0].get(),
                                                                      * bstClientSubscribers[1].get(),
                                                                      * bstServerSubscribers[0].get(),
                                                                      * bstServerSubscribers[1].get(),
                                                                      * bstServerSubscribers[2].get(),
                                                                      100);

    kpsr::bst::BstServiceWizard serviceWizard(container, environment, &bstServerZmqProvider);
    BstServer bstServer(&serviceWizard, running);

    eventloopProvider.start();
    bstServer.start();
@endcode
 */
class BstServerZMQProvider : public BstServerMiddlewareProvider
{
public:
    /**
     * @brief BstServerZMQProvider
     * @param eventloopProvider
     * @param fromZmqMiddlewareProvider
     * @param toZmqMiddlewareProvider
     * @param bstRequestSubscriber
     * @param bstWaypointCommandSubscriber
     * @param systemInitializeSubscriber
     * @param telemetryPositionSubscriber
     * @param telemetryOrientationSubscriber
     * @param pollPeriod
     */
    BstServerZMQProvider(
        kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> &eventloopProvider,
        kpsr::zmq_mdlw::FromZmqMiddlewareProvider &fromZmqMiddlewareProvider,
        kpsr::zmq_mdlw::ToZMQMiddlewareProvider &toZmqMiddlewareProvider,
        zmq::socket_t &bstRequestSubscriber,
        zmq::socket_t &bstWaypointCommandSubscriber,
        zmq::socket_t &systemInitializeSubscriber,
        zmq::socket_t &telemetryPositionSubscriber,
        zmq::socket_t &telemetryOrientationSubscriber,
        long pollPeriod)
        : _eventloopProvider(eventloopProvider)
        , _fromZmqMiddlewareProvider(fromZmqMiddlewareProvider)
        , _toZmqMiddlewareProvider(toZmqMiddlewareProvider)
        , _bstRequestFromZmqChannel(
              fromZmqMiddlewareProvider
                  .getJsonFromMiddlewareChannel<kpsr::bst::BstRequestMessage>(bstRequestSubscriber,
                                                                              pollPeriod))
        , _bstWaypointCommandFromZmqChannel(
              fromZmqMiddlewareProvider.getJsonFromMiddlewareChannel<kpsr::geometry::PoseEventData>(
                  bstWaypointCommandSubscriber, pollPeriod))
        , _systemInitializeFromZmqChannel(
              fromZmqMiddlewareProvider
                  .getVoidCasterFromMiddlewareChannel<SystemInitialize_t>(systemInitializeSubscriber,
                                                                          pollPeriod))
        , _telemetryPositionFromZmqChannel(
              fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<TelemetryPosition_t>(
                  telemetryPositionSubscriber, pollPeriod))
        , _telemetryOrientationFromZmqChannel(
              fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<TelemetryOrientation_t>(
                  telemetryOrientationSubscriber, pollPeriod))
    {}

    /**
     * @brief start
     */
    void start() override
    {
        _bstRequestFromZmqChannel->start();
        _bstWaypointCommandFromZmqChannel->start();
        _systemInitializeFromZmqChannel->start();
        _telemetryPositionFromZmqChannel->start();
        _telemetryOrientationFromZmqChannel->start();
    }

    /**
     * @brief stop
     */
    void stop() override
    {
        _bstRequestFromZmqChannel->stop();
        _bstWaypointCommandFromZmqChannel->stop();
        _systemInitializeFromZmqChannel->stop();
        _telemetryPositionFromZmqChannel->stop();
        _telemetryOrientationFromZmqChannel->stop();
    }

    /**
     * @brief getBstRequestMessageSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::bst::BstRequestMessage> *getBstRequestMessageSubscriber()
    {
        std::string eventName = ZMQ_TOPIC_NAME_BST_REQUEST;
        kpsr::Publisher<kpsr::bst::BstRequestMessage> *publisher =
            _eventloopProvider.template getPublisher<kpsr::bst::BstRequestMessage>(eventName,
                                                                                   0,
                                                                                   nullptr,
                                                                                   nullptr);
        _bstRequestFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<kpsr::bst::BstRequestMessage>(eventName);
    }

    /**
     * @brief getBstWaypointCommandMessageSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::bst::WaypointCommandMessage> *getBstWaypointCommandMessageSubscriber()
    {
        std::string eventName = ZMQ_TOPIC_NAME_BST_WP_CMD;
        kpsr::Publisher<kpsr::bst::WaypointCommandMessage> *publisher =
            _eventloopProvider.template getPublisher<kpsr::bst::WaypointCommandMessage>(eventName,
                                                                                        0,
                                                                                        nullptr,
                                                                                        nullptr);
        _bstWaypointCommandFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<kpsr::bst::WaypointCommandMessage>(
            eventName);
    }

    /**
     * @brief getBst2KpsrReplyMessagePublisher
     * @return
     */
    kpsr::Publisher<kpsr::bst::BstReplyMessage> *getBst2KpsrReplyMessagePublisher()
    {
        return _toZmqMiddlewareProvider
            .getJsonToMiddlewareChannel<kpsr::bst::BstReplyMessage>(ZMQ_TOPIC_NAME_BST_REPLY, 0);
    }

    /**
     * @brief getPoseEventDataPublisher
     * @return
     */
    kpsr::Publisher<kpsr::geometry::PoseEventData> *getPoseEventDataPublisher()
    {
        return _toZmqMiddlewareProvider
            .getJsonToMiddlewareChannel<kpsr::geometry::PoseEventData>(ZMQ_TOPIC_NAME_POSE_EVENT, 0);
    }

    /**
     * @brief getSystemInitializeSubscriber
     * @return
     */
    kpsr::Subscriber<SystemInitialize_t> *getSystemInitializeSubscriber()
    {
        std::string eventName = ZMQ_TOPIC_NAME_SYSTEM_INIT;
        kpsr::Publisher<SystemInitialize_t> *publisher =
            _eventloopProvider.template getPublisher<SystemInitialize_t>(eventName,
                                                                         0,
                                                                         nullptr,
                                                                         nullptr);
        _systemInitializeFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<SystemInitialize_t>(eventName);
    }

    /**
     * @brief getSystemInitializePublisher
     * @return
     */
    kpsr::Publisher<SystemInitialize_t> *getSystemInitializePublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<SystemInitialize_t>(ZMQ_TOPIC_NAME_SYSTEM_INIT, 0);
    }

    /**
     * @brief getTelemetryPositionSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryPosition_t> *getTelemetryPositionSubscriber()
    {
        std::string eventName = ZMQ_TOPIC_NAME_TELEMETRY_POS;
        kpsr::Publisher<TelemetryPosition_t> *publisher =
            _eventloopProvider.template getPublisher<TelemetryPosition_t>(eventName,
                                                                          0,
                                                                          nullptr,
                                                                          nullptr);
        _telemetryPositionFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<TelemetryPosition_t>(eventName);
    }

    /**
     * @brief getTelemetryPositionPublisher
     * @return
     */
    kpsr::Publisher<TelemetryPosition_t> *getTelemetryPositionPublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<TelemetryPosition_t>(ZMQ_TOPIC_NAME_TELEMETRY_POS, 0);
    }

    /**
     * @brief getTelemetryOrientationSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryOrientation_t> *getTelemetryOrientationSubscriber()
    {
        std::string eventName = ZMQ_TOPIC_NAME_TELEMETRY_ORI;
        kpsr::Publisher<TelemetryOrientation_t> *publisher =
            _eventloopProvider.template getPublisher<TelemetryOrientation_t>(eventName,
                                                                             0,
                                                                             nullptr,
                                                                             nullptr);
        _telemetryOrientationFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<TelemetryOrientation_t>(eventName);
    }

    /**
     * @brief getTelemetryOrientationPublisher
     * @return
     */
    kpsr::Publisher<TelemetryOrientation_t> *getTelemetryOrientationPublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<TelemetryOrientation_t>(ZMQ_TOPIC_NAME_TELEMETRY_ORI,
                                                                      0);
    }

    /**
     * @brief getSensorPublisher
     * @return
     */
    kpsr::Publisher<Sensors_t> *getSensorPublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<Sensors_t>(ZMQ_TOPIC_NAME_SENSOR, 0);
    }

    /**
     * @brief getCalibratePublisher
     * @return
     */
    kpsr::Publisher<CalibrateSensor_t> *getCalibratePublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<CalibrateSensor_t>(ZMQ_TOPIC_NAME_CALIBRATE, 0);
    }

    /**
     * @brief getControlCommandPublisher
     * @return
     */
    kpsr::Publisher<Command_t> *getControlCommandPublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<Command_t>(ZMQ_TOPIC_NAME_CONTROL_CMD, 0);
    }

    /**
     * @brief getControlPidPublisher
     * @return
     */
    kpsr::Publisher<PID_t> *getControlPidPublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<PID_t>(ZMQ_TOPIC_NAME_CONTROL_PID, 0);
    }

    /**
     * @brief getSystemPublisher
     * @return
     */
    kpsr::Publisher<uint8_t> *getSystemPublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<uint8_t>(ZMQ_TOPIC_NAME_SYSTEM, 0);
    }

    /**
     * @brief getTelemetrySystemPublisher
     * @return
     */
    kpsr::Publisher<TelemetrySystem_t> *getTelemetrySystemPublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<TelemetrySystem_t>(ZMQ_TOPIC_NAME_TELEMETRY_SYS, 0);
    }

    /**
     * @brief getTelemetryPressurePublisher
     * @return
     */
    kpsr::Publisher<TelemetryPressure_t> *getTelemetryPressurePublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<TelemetryPressure_t>(ZMQ_TOPIC_NAME_TELEMETRY_PRES, 0);
    }

    /**
     * @brief getTelemetryControlPublisher
     * @return
     */
    kpsr::Publisher<::bst::comms::TelemetryControl_t> *getTelemetryControlPublisher()
    {
        return _toZmqMiddlewareProvider.getVoidCasterToMiddlewareChannel<
            ::bst::comms::TelemetryControl_t>(ZMQ_TOPIC_NAME_TELEMETRY_CTRL, 0);
    }

    /**
     * @brief getTelemetryGCSPublisher
     * @return
     */
    kpsr::Publisher<gcs::TelemetryGCS_t> *getTelemetryGCSPublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<gcs::TelemetryGCS_t>(ZMQ_TOPIC_NAME_TELEMETRY_GCS, 0);
    }

    /**
     * @brief getPayloadControlPublisher
     * @return
     */
    kpsr::Publisher<PayloadControl_t> *getPayloadControlPublisher()
    {
        return _toZmqMiddlewareProvider
            .getVoidCasterToMiddlewareChannel<PayloadControl_t>(ZMQ_TOPIC_NAME_PAYLOAD_CONTROL, 0);
    }

private:
    kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> &_eventloopProvider;
    kpsr::zmq_mdlw::FromZmqMiddlewareProvider &_fromZmqMiddlewareProvider;
    kpsr::zmq_mdlw::ToZMQMiddlewareProvider &_toZmqMiddlewareProvider;

    kpsr::zmq_mdlw::FromZmqChannel<std::string> *_bstRequestFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::string> *_bstWaypointCommandFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> *_systemInitializeFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> *_telemetryPositionFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> *_telemetryOrientationFromZmqChannel;
};
} // namespace zmq_mdlw
} // namespace bst
} // namespace kpsr

#endif // BST_SERVER_ZMQ_PROVIDER_H
