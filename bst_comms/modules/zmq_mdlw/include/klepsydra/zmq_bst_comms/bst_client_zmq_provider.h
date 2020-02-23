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
#ifndef BST_CLIENT_ZMQ_PROVIDER_H
#define BST_CLIENT_ZMQ_PROVIDER_H

#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/geometry/pose_event_data.h>

#include <klepsydra/bst_comms/bst_client_middleware_provider.h>

#include <klepsydra/zmq_bst_comms/bst_zmq_topic_names.h>

namespace kpsr
{
namespace bst
{
namespace zmq_mdlw
{
template <std::size_t BufferSize>
/**
 * @brief The BstClientZMQProvider class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-zmq-bst
 *
 * @details Client ZMQ implementation to all BST provided messages subscribers and request publishers.
 * Code example:
@code
    std::string bstClientPublishUrl = "tcp://*:8120";
    std::string bstServerSubscribeUrl = "tcp://localhost:8110";

    //  Prepare our context and publisher
    zmq::context_t context (1);

    zmq::socket_t bstClientPublisher(context, ZMQ_PUB);
    bstClientPublisher.bind(bstClientPublishUrl);
    bstClientPublisher.bind("ipc://kpsr.bst.client.ipc");

    int numberServerSubscribers = 15;
    std::vector<std::shared_ptr<zmq::socket_t>> bstServerSubscribers(numberServerSubscribers);
    for (int i = 0; i < numberServerSubscribers; i ++) {
        bstServerSubscribers[i] = std::shared_ptr<zmq::socket_t>(new zmq::socket_t(context, ZMQ_SUB));
        bstServerSubscribers[i]->connect(bstServerSubscribeUrl);
        bstServerSubscribers[i]->setsockopt(ZMQ_SUBSCRIBE, "", 0);
    }

    //  Process 100 updates
    kpsr::zmq_mdlw::FromZmqMiddlewareProvider fromZmqMiddlewareProvider;
    kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZmqMiddlewareProvider(nullptr, bstClientPublisher);

    std::string configListenUrl = "tcp://localhost:8210";
    std::string configWriteUrl = "tcp://localhost:8200";
    std::string configTopic = "configuration";
    std::string configKey = "test_configuration";

    //  Socket to talk to server
    spdlog::info("Setup configuration publisher\n");
    zmq::socket_t envPublisher (context, ZMQ_PUB);
    envPublisher.connect(configWriteUrl);

    spdlog::info("Setup configuration subcriber\n");
    zmq::socket_t envSubscriber (context, ZMQ_SUB);
    envSubscriber.connect(configListenUrl);
    envSubscriber.setsockopt(ZMQ_SUBSCRIBE, configTopic.c_str(), configTopic.size());

    kpsr::zmq_mdlw::ZMQEnv environment(fileName, configKey, configTopic, envPublisher, envSubscriber);

    kpsr::high_performance::EventLoopMiddlewareProvider<256> eventloopProvider(container);
    bool running = true;
    kpsr::bst::zmq::BstClientZMQProvider<256> bstClientZmqProvider(eventloopProvider,
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

    eventloopProvider.start();
    bstClientZmqProvider.start();
@endcode
 */
class BstClientZMQProvider : public BstClientMiddlewareProvider {
public:

    /**
     * @brief BstClientZMQProvider
     * @param eventloopProvider
     * @param fromZmqMiddlewareProvider
     * @param toZmqMiddlewareProvider
     * @param bstReplySubscriber
     * @param poseDataSubscriber
     * @param systemInitializeSubscriber
     * @param telemetryPositionSubscriber
     * @param telemetryOrientationSubscriber
     * @param sensorsSubscriber
     * @param calibrateSensorSubscriber
     * @param commandSubscriber
     * @param pidSubscriber
     * @param systemSubscriber
     * @param telemetrySystemSubscriber
     * @param telemetryPressureSubscriber
     * @param telemetryControlSubscriber
     * @param telemetryGCSSubscriber
     * @param payloadControlSubscriber
     * @param pollPeriod
     */
    BstClientZMQProvider(kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & eventloopProvider,
                         kpsr::zmq_mdlw::FromZmqMiddlewareProvider & fromZmqMiddlewareProvider,
                         kpsr::zmq_mdlw::ToZMQMiddlewareProvider & toZmqMiddlewareProvider,
                         zmq::socket_t & bstReplySubscriber,
                         zmq::socket_t & poseDataSubscriber,
                         zmq::socket_t & systemInitializeSubscriber,
                         zmq::socket_t & telemetryPositionSubscriber,
                         zmq::socket_t & telemetryOrientationSubscriber,
                         zmq::socket_t & sensorsSubscriber,
                         zmq::socket_t & calibrateSensorSubscriber,
                         zmq::socket_t & commandSubscriber,
                         zmq::socket_t & pidSubscriber,
                         zmq::socket_t & systemSubscriber,
                         zmq::socket_t & telemetrySystemSubscriber,
                         zmq::socket_t & telemetryPressureSubscriber,
                         zmq::socket_t & telemetryControlSubscriber,
                         zmq::socket_t & telemetryGCSSubscriber,
                         zmq::socket_t & payloadControlSubscriber,
                         long pollPeriod)
        : _eventloopProvider(eventloopProvider)
        , _fromZmqMiddlewareProvider(fromZmqMiddlewareProvider)
        , _toZmqMiddlewareProvider(toZmqMiddlewareProvider)
        , _bstReplyFromZmqChannel(fromZmqMiddlewareProvider.getJsonFromMiddlewareChannel<kpsr::bst::BstReplyMessage>(bstReplySubscriber, pollPeriod))
        , _poseDataFromZmqChannel(fromZmqMiddlewareProvider.getJsonFromMiddlewareChannel<kpsr::geometry::PoseEventData>(poseDataSubscriber, pollPeriod))
        , _systemInitializeFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<SystemInitialize_t>(systemInitializeSubscriber, pollPeriod))
        , _telemetryPositionFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<TelemetryPosition_t>(telemetryPositionSubscriber, pollPeriod))
        , _telemetryOrientationFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<TelemetryOrientation_t>(telemetryOrientationSubscriber, pollPeriod))
        , _sensorsFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<Sensors_t>(sensorsSubscriber, pollPeriod))
        , _calibrateSensorFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<CalibrateSensor_t>(calibrateSensorSubscriber, pollPeriod))
        , _commandFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<Command_t>(commandSubscriber, pollPeriod))
        , _pidFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<PID_t>(pidSubscriber, pollPeriod))
        , _systemFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<uint8_t>(systemSubscriber, pollPeriod))
        , _telemetrySystemFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<TelemetrySystem_t>(telemetrySystemSubscriber, pollPeriod))
        , _telemetryPressureFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<TelemetryPressure_t>(telemetryPressureSubscriber, pollPeriod))
        , _telemetryControlFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<::bst::comms::TelemetryControl_t>(telemetryControlSubscriber, pollPeriod))
        , _telemetryGCSFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<gcs::TelemetryGCS_t>(telemetryGCSSubscriber, pollPeriod))
        , _payloadControlFromZmqChannel(fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<PayloadControl_t>(payloadControlSubscriber, pollPeriod))
    {}

    /**
     * @brief start
     */
    void start() override {
        _bstReplyFromZmqChannel->start();
        _poseDataFromZmqChannel->start();
        _systemInitializeFromZmqChannel->start();
        _telemetryPositionFromZmqChannel->start();
        _telemetryOrientationFromZmqChannel->start();
        _sensorsFromZmqChannel->start();
        _calibrateSensorFromZmqChannel->start();
        _commandFromZmqChannel->start();
        _pidFromZmqChannel->start();
        _systemFromZmqChannel->start();
        _telemetrySystemFromZmqChannel->start();
        _telemetryPressureFromZmqChannel->start();
        _telemetryControlFromZmqChannel->start();
        _telemetryGCSFromZmqChannel->start();
        _payloadControlFromZmqChannel->start();
    }

    /**
     * @brief stop
     */
    void stop() override {
        _bstReplyFromZmqChannel->stop();
        _poseDataFromZmqChannel->stop();
        _systemInitializeFromZmqChannel->stop();
        _telemetryPositionFromZmqChannel->stop();
        _telemetryOrientationFromZmqChannel->stop();
        _sensorsFromZmqChannel->stop();
        _calibrateSensorFromZmqChannel->stop();
        _commandFromZmqChannel->stop();
        _pidFromZmqChannel->stop();
        _systemFromZmqChannel->stop();
        _telemetrySystemFromZmqChannel->stop();
        _telemetryPressureFromZmqChannel->stop();
        _telemetryControlFromZmqChannel->stop();
        _telemetryGCSFromZmqChannel->stop();
        _payloadControlFromZmqChannel->stop();
    }

    /**
     * @brief getBstRequestMessagePublisher
     * @return
     */
    kpsr::Publisher<kpsr::bst::BstRequestMessage> * getBstRequestMessagePublisher() {
        return _toZmqMiddlewareProvider.getJsonToMiddlewareChannel<kpsr::bst::BstRequestMessage>(ZMQ_TOPIC_NAME_BST_REQUEST, 0);
    }

    /**
     * @brief getBst2KpsrReplyMessageSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::bst::BstReplyMessage> * getBst2KpsrReplyMessageSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_BST_REPLY;
        kpsr::Publisher<kpsr::bst::BstReplyMessage> * publisher = _eventloopProvider.template getPublisher<kpsr::bst::BstReplyMessage>(eventName, 0, nullptr, nullptr);
        _bstReplyFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<kpsr::bst::BstReplyMessage>(eventName);
    }

    /**
     * @brief getBstWaypointCommandMessagePublisher
     * @return
     */
    kpsr::Publisher<WaypointCommandMessage> * getBstWaypointCommandMessagePublisher() {
        return _toZmqMiddlewareProvider.getJsonToMiddlewareChannel<WaypointCommandMessage>(ZMQ_TOPIC_NAME_BST_WP_CMD, 0);
    }

    /**
     * @brief getPoseEventDataSubscriber
     * @return
     */
    kpsr::Subscriber<kpsr::geometry::PoseEventData> * getPoseEventDataSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_POSE_EVENT;
        kpsr::Publisher<kpsr::geometry::PoseEventData> * publisher = _eventloopProvider.template getPublisher<kpsr::geometry::PoseEventData>(eventName, 0, nullptr, nullptr);
        _poseDataFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<kpsr::geometry::PoseEventData>(eventName);
    }

    /**
     * @brief getSystemInitializeSubscriber
     * @return
     */
    kpsr::Subscriber<SystemInitialize_t> * getSystemInitializeSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_SYSTEM_INIT;
        kpsr::Publisher<SystemInitialize_t> * publisher = _eventloopProvider.template getPublisher<SystemInitialize_t>(eventName, 0, nullptr, nullptr);
        _systemInitializeFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<SystemInitialize_t>(eventName);
    }

    /**
     * @brief getTelemetryPositionSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryPosition_t> * getTelemetryPositionSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_TELEMETRY_POS;
        kpsr::Publisher<TelemetryPosition_t> * publisher = _eventloopProvider.template getPublisher<TelemetryPosition_t>(eventName, 0, nullptr, nullptr);
        _telemetryPositionFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<TelemetryPosition_t>(eventName);
    }

    /**
     * @brief getTelemetryOrientationSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryOrientation_t> * getTelemetryOrientationSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_TELEMETRY_ORI;
        kpsr::Publisher<TelemetryOrientation_t> * publisher = _eventloopProvider.template getPublisher<TelemetryOrientation_t>(eventName, 0, nullptr, nullptr);
        _telemetryOrientationFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<TelemetryOrientation_t>(eventName);
    }

    /**
     * @brief getSensorSubscriber
     * @return
     */
    kpsr::Subscriber<Sensors_t> * getSensorSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_SENSOR;
        kpsr::Publisher<Sensors_t> * publisher = _eventloopProvider.template getPublisher<Sensors_t>(eventName, 0, nullptr, nullptr);
        _sensorsFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<Sensors_t>(eventName);
    }

    /**
     * @brief getCalibrateSubscriber
     * @return
     */
    kpsr::Subscriber<CalibrateSensor_t> * getCalibrateSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_CALIBRATE;
        kpsr::Publisher<CalibrateSensor_t> * publisher = _eventloopProvider.template getPublisher<CalibrateSensor_t>(eventName, 0, nullptr, nullptr);
        _calibrateSensorFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<CalibrateSensor_t>(eventName);
    }

    /**
     * @brief getControlCommandSubscriber
     * @return
     */
    kpsr::Subscriber<Command_t> * getControlCommandSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_CONTROL_CMD;
        kpsr::Publisher<Command_t> * publisher = _eventloopProvider.template getPublisher<Command_t>(eventName, 0, nullptr, nullptr);
        _commandFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<Command_t>(eventName);
    }

    /**
     * @brief getControlPidSubscriber
     * @return
     */
    kpsr::Subscriber<PID_t> * getControlPidSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_CONTROL_PID;
        kpsr::Publisher<PID_t> * publisher = _eventloopProvider.template getPublisher<PID_t>(eventName, 0, nullptr, nullptr);
        _pidFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<PID_t>(eventName);
    }

    /**
     * @brief getSystemSubscriber
     * @return
     */
    kpsr::Subscriber<uint8_t> * getSystemSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_SYSTEM;
        kpsr::Publisher<uint8_t> * publisher = _eventloopProvider.template getPublisher<uint8_t>(eventName, 0, nullptr, nullptr);
        _systemFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<uint8_t>(eventName);
    }

    /**
     * @brief getTelemetrySystemSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetrySystem_t> * getTelemetrySystemSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_TELEMETRY_SYS;
        kpsr::Publisher<TelemetrySystem_t> * publisher = _eventloopProvider.template getPublisher<TelemetrySystem_t>(eventName, 0, nullptr, nullptr);
        _telemetrySystemFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<TelemetrySystem_t>(eventName);
    }

    /**
     * @brief getTelemetryPressureSubscriber
     * @return
     */
    kpsr::Subscriber<TelemetryPressure_t> * getTelemetryPressureSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_TELEMETRY_PRES;
        kpsr::Publisher<TelemetryPressure_t> * publisher = _eventloopProvider.template getPublisher<TelemetryPressure_t>(eventName, 0, nullptr, nullptr);
        _telemetryPressureFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<TelemetryPressure_t>(eventName);
    }

    /**
     * @brief getTelemetryControlSubscriber
     * @return
     */
    kpsr::Subscriber<::bst::comms::TelemetryControl_t> * getTelemetryControlSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_TELEMETRY_CTRL;
        kpsr::Publisher<::bst::comms::TelemetryControl_t> * publisher = _eventloopProvider.template getPublisher<::bst::comms::TelemetryControl_t>(eventName, 0, nullptr, nullptr);
        _telemetryControlFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<::bst::comms::TelemetryControl_t>(eventName);
    }

    /**
     * @brief getTelemetryGCSSubscriber
     * @return
     */
    kpsr::Subscriber<gcs::TelemetryGCS_t> * getTelemetryGCSSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_TELEMETRY_GCS;
        kpsr::Publisher<gcs::TelemetryGCS_t> * publisher = _eventloopProvider.template getPublisher<gcs::TelemetryGCS_t>(eventName, 0, nullptr, nullptr);
        _telemetryGCSFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<gcs::TelemetryGCS_t>(eventName);
    }

    /**
     * @brief getPayloadControlSubscriber
     * @return
     */
    kpsr::Subscriber<PayloadControl_t> * getPayloadControlSubscriber() {
        std::string eventName = ZMQ_TOPIC_NAME_PAYLOAD_CONTROL;
        kpsr::Publisher<PayloadControl_t> * publisher = _eventloopProvider.template getPublisher<PayloadControl_t>(eventName, 0, nullptr, nullptr);
        _payloadControlFromZmqChannel->registerToTopic(eventName, publisher);
        return _eventloopProvider.template getSubscriber<PayloadControl_t>(eventName);
    }

    /**
     * @brief getScheduler
     * @return
     */
    Scheduler * getScheduler() override {
        return _eventloopProvider.getScheduler();
    }

private:
    kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & _eventloopProvider;
    kpsr::zmq_mdlw::FromZmqMiddlewareProvider & _fromZmqMiddlewareProvider;
    kpsr::zmq_mdlw::ToZMQMiddlewareProvider & _toZmqMiddlewareProvider;

    kpsr::zmq_mdlw::FromZmqChannel<std::string> * _bstReplyFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::string> * _poseDataFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _systemInitializeFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _telemetryPositionFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _telemetryOrientationFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _sensorsFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _calibrateSensorFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _commandFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _pidFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _systemFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _telemetrySystemFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _telemetryPressureFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _telemetryControlFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _telemetryGCSFromZmqChannel;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * _payloadControlFromZmqChannel;
};
}
}
}

#endif // BST_CLIENT_ZMQ_PROVIDER_H
