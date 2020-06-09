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
#include <thread>
#include <inttypes.h>

#include <spdlog/spdlog.h>

#include <klepsydra/bst_comms/comm_interface_service.h>

/* BST */
#include "multicopter.h"
#include "netuas_serial.h"

#include <klepsydra/bst_comms/bst2kpsr_modules.h>

kpsr::bst::CommInterfaceService::CommInterfaceService(Environment *_environment,
                                                      Subscriber<BstRequestMessage> * bstRequestMessageSubscriber,
                                                      Subscriber<WaypointCommandMessage> * bstWaypointCommandtMessageSubscriber,
                                                      Subscriber<SystemInitialize_t> * systemInitializeSubscriber)
    : Service(_environment, "sdk_bst_interface_service")
    , _bstRequestMessageSubscriber(bstRequestMessageSubscriber)
    , _bstWaypointCommandtMessageSubscriber(bstWaypointCommandtMessageSubscriber)
    , _systemInitializeSubscriber(systemInitializeSubscriber)
{}

void kpsr::bst::CommInterfaceService::onBstWaypointCommandMessageReceived(const WaypointCommandMessage & eventData) {
    std::lock_guard<std::mutex> lock (_mutex);
    spdlog::debug("{}", __PRETTY_FUNCTION__);
    uint8_t num_points = 0;
    _flightPlan.reset();

    for (size_t i = 0; i < eventData.plan.size(); i ++) {
        _tempWaypoints[i].num = eventData.plan[i].num;
        _tempWaypoints[i].next = eventData.plan[i].next;
        _tempWaypoints[i].latitude = eventData.plan[i].latitude;
        _tempWaypoints[i].longitude = eventData.plan[i].longitude;
        _tempWaypoints[i].altitude = eventData.plan[i].altitude;
        _tempWaypoints[i].radius = eventData.plan[i].radius;
    }

    _flightPlan.add(_tempWaypoints, eventData.plan.size());

    // Get all waypoints
    num_points = _flightPlan.getAll(_tempWaypoints);
    memcpy(&_flightPlanMap,_flightPlan.getMap(),sizeof(FlightPlanMap_t));
    _flightPlanMap.mode = eventData.mode;

    spdlog::debug("{} sendCommand", __PRETTY_FUNCTION__);
    _commHandler->sendCommand(FLIGHT_PLAN, (uint8_t *)_tempWaypoints, num_points, &_flightPlanMap);
}

void kpsr::bst::CommInterfaceService::onBstRequestMessageReceived(const BstRequestMessage  & eventData) {
    std::lock_guard<std::mutex> lock (_mutex);
    spdlog::debug("{}", __PRETTY_FUNCTION__);
    switch (eventData.type) {
    case TELEMETRY_HEARTBEAT:
        spdlog::debug("{} TelemetryHeartbeat", __PRETTY_FUNCTION__);
        _commHandler->send(TELEMETRY_HEARTBEAT, NULL, 0, NULL);
        break;
    case ::bst::comms::multicopter::CMD_X_VEL:
    case ::bst::comms::multicopter::CMD_Y_VEL:
    case CMD_VRATE: {
        spdlog::debug("{} velocity command", __PRETTY_FUNCTION__);
        Command_t payLoadActiveCommand;
        payLoadActiveCommand.id = CMD_PAYLOAD_CONTROL;
        payLoadActiveCommand.value = PAYLOAD_CTRL_ACTIVE;
        _commHandler->sendCommand(CONTROL_COMMAND, (uint8_t *)&payLoadActiveCommand, sizeof(Command_t), NULL);

        Command_t command;
        command.id = eventData.type;
        command.value = eventData.value;
        _commHandler->sendCommand(eventData.id, (uint8_t *)&command, sizeof(Command_t), NULL);
        break;
    }
    case SENSORS_GYROSCOPE: {
        spdlog::debug("{} calibrate sensor command", __PRETTY_FUNCTION__);
        CalibrateSensor_t calibrateSensor;
        calibrateSensor.sensor = GYROSCOPE;
        calibrateSensor.state = static_cast<CalibrationState_t>(eventData.value);
        _commHandler->sendCommand(eventData.id, (uint8_t *)&calibrateSensor, sizeof(CalibrateSensor_t), NULL);
        break;
    }
    default: {
        spdlog::debug("{} default", __PRETTY_FUNCTION__);
        Command_t command;
        command.id = eventData.type;
        command.value = eventData.value;
        _commHandler->sendCommand(eventData.id, (uint8_t *)&command, sizeof(Command_t), NULL);
        break;
    }
    }
}

void kpsr::bst::CommInterfaceService::onSystemInitializeReceived(const ::bst::comms::SystemInitialize_t & eventData) {
    std::lock_guard<std::mutex> lock (_mutex);
    spdlog::debug("{}", __PRETTY_FUNCTION__);
    _commHandler->send(SYSTEM_INITIALIZE, (uint8_t *)&eventData, sizeof(SystemInitialize_t), NULL);
}

void kpsr::bst::CommInterfaceService::bstCommunicationsInit () {
    spdlog::debug("{}", __PRETTY_FUNCTION__);
    std::string bstSerialPort;
    std::string bstBaudRate;
    std::string bstHost;
    std::string bstPort;
    bool serialComm;

    _environment->getPropertyBool("bst_serial_enable", serialComm);
    spdlog::info("{}. serialComm:{}", __PRETTY_FUNCTION__, (serialComm ? "true" : "false"));

    if (serialComm) {
        _environment->getPropertyString("bst_serial_port", bstSerialPort);
        _environment->getPropertyString("bst_baud_rate", bstBaudRate);
        spdlog::info("{}. bstSerialPort:{}", __PRETTY_FUNCTION__, bstSerialPort);
        spdlog::info("{}. bstBaudRate:{}", __PRETTY_FUNCTION__, bstBaudRate);
    }
    else {
        _environment->getPropertyString("bst_host", bstHost);
        _environment->getPropertyString("bst_port", bstPort);
        spdlog::info("{}. bstHost:{}", __PRETTY_FUNCTION__, bstHost);
        spdlog::info("{}. bstPort:{}", __PRETTY_FUNCTION__, bstPort);
    }

    // get handler
    _commHandler = new BSTProtocol();

    // set interface
    if(serialComm) {
        _commHandler->setInterface(new NetuasSerial);
        _commInterface = _commHandler->getInterface();
        _commInterface->initialize(bstSerialPort.c_str(), bstBaudRate.c_str(), "");
    } else {
        _commHandler->setInterface(new NetuasSocket);
        _commInterface = _commHandler->getInterface();
        _commInterface->initialize(bstHost.c_str(), bstPort.c_str(), "TCP:CLIENT");
    }

    _basicModule.registerReceive(kpsr::bst::Bst2KpsrModules::BasicModule::receive);
    _basicModule.registerReceiveCommand(kpsr::bst::Bst2KpsrModules::BasicModule::receiveCommand);
    _basicModule.registerReceiveReply(kpsr::bst::Bst2KpsrModules::BasicModule::receiveReply);
    _basicModule.registerPublish(kpsr::bst::Bst2KpsrModules::BasicModule::publish);

    _flightPlanModule.registerReceive(kpsr::bst::Bst2KpsrModules::FlightPlan::receive);
    _flightPlanModule.registerReceiveCommand(kpsr::bst::Bst2KpsrModules::FlightPlan::receiveCommand);
    _flightPlanModule.registerReceiveReply(kpsr::bst::Bst2KpsrModules::FlightPlan::receiveReply);
    _flightPlanModule.registerPublish(kpsr::bst::Bst2KpsrModules::FlightPlan::publish);

    ((BSTProtocol *)_commHandler)->registerModule(&_basicModule);
    ((BSTProtocol *)_commHandler)->registerModule(&_flightPlanModule);
}

void kpsr::bst::CommInterfaceService::start() {
    std::lock_guard<std::mutex> lock (_mutex);
    spdlog::debug("{}", __PRETTY_FUNCTION__);

    this->bstCommunicationsInit();

    std::function<void(const BstRequestMessage &)> bstRequestMessageListener = std::bind(&kpsr::bst::CommInterfaceService::onBstRequestMessageReceived, this, std::placeholders::_1);
    _bstRequestMessageSubscriber->registerListener("sdk_bst_interface_service_listener", bstRequestMessageListener);

    std::function<void(const WaypointCommandMessage &)> bstWaypointCommandMessageListener = std::bind(&kpsr::bst::CommInterfaceService::onBstWaypointCommandMessageReceived, this, std::placeholders::_1);
    _bstWaypointCommandtMessageSubscriber->registerListener("sdk_bst_interface_service_listener", bstWaypointCommandMessageListener);

    std::function<void(const ::bst::comms::SystemInitialize_t &)> systemInitializeListener = std::bind(&kpsr::bst::CommInterfaceService::onSystemInitializeReceived, this, std::placeholders::_1);
    _systemInitializeSubscriber->registerListener("sdk_bst_interface_service_listener", systemInitializeListener);

    _commHandler->getInterface()->open();
}

void kpsr::bst::CommInterfaceService::stop() {
    std::lock_guard<std::mutex> lock (_mutex);
    spdlog::debug("{}", __PRETTY_FUNCTION__);

    _bstRequestMessageSubscriber->removeListener("sdk_bst_interface_service_listener");
    _bstWaypointCommandtMessageSubscriber->removeListener("sdk_bst_interface_service_listener");
    _systemInitializeSubscriber->removeListener("sdk_bst_interface_service_listener");

    _commHandler->getInterface()->close();
}

void kpsr::bst::CommInterfaceService::execute() {
    std::lock_guard<std::mutex> lock (_mutex);
    if(_commInterface->isConnected()) {
        _commHandler->update();
    }
}

void kpsr::bst::CommInterfaceService::sendPayloadControlActive() {
    spdlog::debug("{}", __PRETTY_FUNCTION__);
    Command_t command;
    command.id = CMD_PAYLOAD_CONTROL;
    command.value = PAYLOAD_CTRL_ACTIVE;

    _commHandler->sendCommand(CONTROL_COMMAND, (uint8_t *)&command, sizeof(Command_t), NULL);
}
