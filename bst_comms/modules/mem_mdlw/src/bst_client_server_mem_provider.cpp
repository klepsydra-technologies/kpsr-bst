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
#include <klepsydra/bst_comms/bst_kpsr_event_names.h>

#include <klepsydra/mem_bst_comms/bst_client_server_mem_provider.h>

kpsr::bst::mem::BstClientServerMemProvider::BstClientServerMemProvider(Container * container)
    : _bstRequestMessageMiddlewareProvider(container, EVENT_NAME_BST_REQUEST, 8, 0, nullptr, nullptr, false)
    , _bstWaypointCommandMessageMiddlewareProvider(container, EVENT_NAME_BST_WP_CMD, 8, 0, nullptr, nullptr, false)
    , _bstReplyMessageMiddlewareProvider(container, EVENT_NAME_BST_REPLY, 8, 0, nullptr, nullptr, false)
    , _poseEventDataMiddlewareProvider(container, EVENT_NAME_POSE_EVENT, 8, 10, nullptr, nullptr, true)
    , _systemInitializeMiddlewareProvider(container, EVENT_NAME_SYSTEM_INIT, 8, 0, nullptr, nullptr, false)
    , _telemetryPositionMiddlewareProvider(container, EVENT_NAME_TELEMETRY_POS, 8, 0, nullptr, nullptr, true)
    , _telemetryOrientationMiddlewareProvider(container, EVENT_NAME_TELEMETRY_ORI, 8, 0, nullptr, nullptr, true)
    , _sensorMiddlewareProvider(container, EVENT_NAME_SENSOR, 8, 0, nullptr, nullptr, true)
    , _calibrateMiddlewareProvider(container, EVENT_NAME_CALIBRATE, 8, 0, nullptr, nullptr, true)
    , _controlCommandMiddlewareProvider(container, EVENT_NAME_CONTROL_CMD, 8, 0, nullptr, nullptr, true)
    , _controlPidMiddlewareProvider(container, EVENT_NAME_CONTROL_PID, 8, 0, nullptr, nullptr, true)
    , _systemMiddlewareProvider(container, EVENT_NAME_SYSTEM, 8, 0, nullptr, nullptr, false)
    , _telemetrySystemMiddlewareProvider(container, EVENT_NAME_TELEMETRY_SYS, 8, 0, nullptr, nullptr, false)
    , _telemetryPressureMiddlewareProvider(container, EVENT_NAME_TELEMETRY_PRES, 8, 0, nullptr, nullptr, true)
    , _telemetryControlMiddlewareProvider(container, EVENT_NAME_TELEMETRY_CTRL, 8, 0, nullptr, nullptr, true)
    , _telemetryGCSMiddlewareProvider(container, EVENT_NAME_TELEMETRY_GCS, 8, 0, nullptr, nullptr, true)
    , _payloadControlMiddlewareProvider(container, EVENT_NAME_PAYLOAD_CONTROL, 8, 0, nullptr, nullptr, false)
{}

void kpsr::bst::mem::BstClientServerMemProvider::start() {
    _bstRequestMessageMiddlewareProvider.start();
    _bstWaypointCommandMessageMiddlewareProvider.start();
    _bstReplyMessageMiddlewareProvider.start();
    _poseEventDataMiddlewareProvider.start();
    _systemInitializeMiddlewareProvider.start();
    _telemetryPositionMiddlewareProvider.start();
    _telemetryOrientationMiddlewareProvider.start();
    _sensorMiddlewareProvider.start();
    _calibrateMiddlewareProvider.start();
    _controlCommandMiddlewareProvider.start();
    _controlPidMiddlewareProvider.start();
    _systemMiddlewareProvider.start();
    _telemetrySystemMiddlewareProvider.start();
    _telemetryPressureMiddlewareProvider.start();
    _telemetryControlMiddlewareProvider.start();
    _telemetryGCSMiddlewareProvider.start();
    _payloadControlMiddlewareProvider.start();
}

void kpsr::bst::mem::BstClientServerMemProvider::stop() {
    _bstRequestMessageMiddlewareProvider.stop();
    _bstWaypointCommandMessageMiddlewareProvider.stop();
    _bstReplyMessageMiddlewareProvider.stop();
    _poseEventDataMiddlewareProvider.stop();
    _systemInitializeMiddlewareProvider.stop();
    _telemetryPositionMiddlewareProvider.stop();
    _telemetryOrientationMiddlewareProvider.stop();
    _sensorMiddlewareProvider.stop();
    _calibrateMiddlewareProvider.stop();
    _controlCommandMiddlewareProvider.stop();
    _controlPidMiddlewareProvider.stop();
    _systemMiddlewareProvider.stop();
    _telemetrySystemMiddlewareProvider.stop();
    _telemetryPressureMiddlewareProvider.stop();
    _telemetryControlMiddlewareProvider.stop();
    _telemetryGCSMiddlewareProvider.stop();
    _payloadControlMiddlewareProvider.stop();
}

kpsr::Subscriber<kpsr::bst::BstRequestMessage> * kpsr::bst::mem::BstClientServerMemProvider::getBstRequestMessageSubscriber(){
    return _bstRequestMessageMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<kpsr::bst::BstRequestMessage> * kpsr::bst::mem::BstClientServerMemProvider::getBstRequestMessagePublisher(){
    return _bstRequestMessageMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<kpsr::bst::WaypointCommandMessage> * kpsr::bst::mem::BstClientServerMemProvider::getBstWaypointCommandMessageSubscriber(){
    return _bstWaypointCommandMessageMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<kpsr::bst::WaypointCommandMessage> * kpsr::bst::mem::BstClientServerMemProvider::getBstWaypointCommandMessagePublisher(){
    return _bstWaypointCommandMessageMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<kpsr::bst::BstReplyMessage> * kpsr::bst::mem::BstClientServerMemProvider::getBst2KpsrReplyMessageSubscriber(){
    return _bstReplyMessageMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<kpsr::bst::BstReplyMessage> * kpsr::bst::mem::BstClientServerMemProvider::getBst2KpsrReplyMessagePublisher(){
    return _bstReplyMessageMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<kpsr::geometry::PoseEventData> * kpsr::bst::mem::BstClientServerMemProvider::getPoseEventDataSubscriber(){
    return _poseEventDataMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<kpsr::geometry::PoseEventData> * kpsr::bst::mem::BstClientServerMemProvider::getPoseEventDataPublisher(){
    return _poseEventDataMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<SystemInitialize_t> * kpsr::bst::mem::BstClientServerMemProvider::getSystemInitializeSubscriber(){
    return _systemInitializeMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<SystemInitialize_t> * kpsr::bst::mem::BstClientServerMemProvider::getSystemInitializePublisher(){
    return _systemInitializeMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<TelemetryPosition_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryPositionSubscriber(){
    return _telemetryPositionMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<TelemetryPosition_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryPositionPublisher(){
    return _telemetryPositionMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<TelemetryOrientation_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryOrientationSubscriber(){
    return _telemetryOrientationMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<TelemetryOrientation_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryOrientationPublisher(){
    return _telemetryOrientationMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<Sensors_t> * kpsr::bst::mem::BstClientServerMemProvider::getSensorSubscriber(){
    return _sensorMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<Sensors_t> * kpsr::bst::mem::BstClientServerMemProvider::getSensorPublisher(){
    return _sensorMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<CalibrateSensor_t> * kpsr::bst::mem::BstClientServerMemProvider::getCalibrateSubscriber(){
    return _calibrateMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<CalibrateSensor_t> * kpsr::bst::mem::BstClientServerMemProvider::getCalibratePublisher(){
    return _calibrateMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<Command_t> * kpsr::bst::mem::BstClientServerMemProvider::getControlCommandSubscriber(){
    return _controlCommandMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<Command_t> * kpsr::bst::mem::BstClientServerMemProvider::getControlCommandPublisher(){
    return _controlCommandMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<PID_t> * kpsr::bst::mem::BstClientServerMemProvider::getControlPidSubscriber(){
    return _controlPidMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<PID_t> * kpsr::bst::mem::BstClientServerMemProvider::getControlPidPublisher(){
    return _controlPidMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<uint8_t> * kpsr::bst::mem::BstClientServerMemProvider::getSystemSubscriber(){
    return _systemMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<uint8_t> * kpsr::bst::mem::BstClientServerMemProvider::getSystemPublisher(){
    return _systemMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<TelemetrySystem_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetrySystemSubscriber(){
    return _telemetrySystemMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<TelemetrySystem_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetrySystemPublisher(){
    return _telemetrySystemMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<TelemetryPressure_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryPressureSubscriber(){
    return _telemetryPressureMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<TelemetryPressure_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryPressurePublisher(){
    return _telemetryPressureMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<::bst::comms::TelemetryControl_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryControlSubscriber(){
    return _telemetryControlMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<::bst::comms::TelemetryControl_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryControlPublisher(){
    return _telemetryControlMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<gcs::TelemetryGCS_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryGCSSubscriber(){
    return _telemetryGCSMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<gcs::TelemetryGCS_t> * kpsr::bst::mem::BstClientServerMemProvider::getTelemetryGCSPublisher(){
    return _telemetryGCSMiddlewareProvider.getPublisher();
}

kpsr::Subscriber<PayloadControl_t> * kpsr::bst::mem::BstClientServerMemProvider::getPayloadControlSubscriber(){
    return _payloadControlMiddlewareProvider.getSubscriber();
}
kpsr::Publisher<PayloadControl_t> * kpsr::bst::mem::BstClientServerMemProvider::getPayloadControlPublisher(){
    return _payloadControlMiddlewareProvider.getPublisher();
}

kpsr::Scheduler * kpsr::bst::mem::BstClientServerMemProvider::getScheduler() {
    return &_multithreadScheduler;
}
