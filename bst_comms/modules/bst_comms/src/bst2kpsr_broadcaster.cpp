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
#include <iomanip>

#include <spdlog/spdlog.h>

#include <klepsydra/bst_comms/bst2kpsr_broadcaster.h>

kpsr::bst::Bst2KpsrBroadcaster::Bst2KpsrBroadcaster(Publisher<Sensors_t> * sensorPublisher,
                                                    Publisher<CalibrateSensor_t> * calibratePublisher,
                                                    Publisher<Command_t> * controlCommandPublisher,
                                                    Publisher<PID_t> * controlPidPublisher,
                                                    Publisher<uint8_t> * systemPublisher,
                                                    Publisher<SystemInitialize_t> * systemInitializePublisher,
                                                    Publisher<TelemetryPosition_t> * telemetryPositionPublisher,
                                                    Publisher<TelemetryOrientation_t> * telemetryOrientationPublisher,
                                                    Publisher<TelemetrySystem_t> * telemetrySystemPublisher,
                                                    Publisher<TelemetryPressure_t> * telemetryPressurePublisher,
                                                    Publisher<::bst::comms::TelemetryControl_t> * telemetryControlPublisher,
                                                    Publisher<gcs::TelemetryGCS_t> * telemetryGCSPublisher,
                                                    Publisher<PayloadControl_t> * payloadControlPublisher,
                                                    Publisher<BstReplyMessage> * bst2KpsrReplyMessagePublisher,
                                                    uint32_t serialNumber)
    : _sensorPublisher(sensorPublisher)
    , _calibratePublisher(calibratePublisher)
    , _controlCommandPublisher(controlCommandPublisher)
    , _controlPidPublisher(controlPidPublisher)
    , _systemPublisher(systemPublisher)
    , _systemInitializePublisher(systemInitializePublisher)
    , _telemetryPositionPublisher(telemetryPositionPublisher)
    , _telemetryOrientationPublisher(telemetryOrientationPublisher)
    , _telemetrySystemPublisher(telemetrySystemPublisher)
    , _telemetryPressurePublisher(telemetryPressurePublisher)
    , _telemetryControlPublisher(telemetryControlPublisher)
    , _telemetryGCSPublisher(telemetryGCSPublisher)
    , _payloadControlPublisher(payloadControlPublisher)
    , _bst2KpsrReplyMessagePublisher(bst2KpsrReplyMessagePublisher)
    , _payloadCurrentState(PAYLOAD_CTRL_OFF)
    , _serialNumber(serialNumber)
{}

void kpsr::bst::Bst2KpsrBroadcaster::receive(uint8_t type, std::vector<unsigned char> data, uint16_t size, const void * parameter) {
    switch(type) {
    /* SENSORS */
    case SENSORS_GPS:
    case SENSORS_ACCELEROMETER:
    case SENSORS_GYROSCOPE:
    case SENSORS_MAGNETOMETER:
    case SENSORS_IMU:
    case SENSORS_DYNAMIC_PRESSURE:
    case SENSORS_STATIC_PRESSURE:
    case SENSORS_AIR_TEMPERATURE:
    case SENSORS_AGL: {
        spdlog::debug("{}Sensor packet received", __PRETTY_FUNCTION__);
        ::bst::comms::Sensors_t sensors;
        memcpy(&sensors, data.data(), sizeof(::bst::comms::Sensors_t));
        _sensorPublisher->publish(sensors);
        break;
    }
    case SENSORS_CALIBRATE: {
        spdlog::debug("{}SENSORS_CALIBRATE received", __PRETTY_FUNCTION__);
        ::bst::comms::CalibrateSensor_t calibrateSensor;
        memcpy(&calibrateSensor, data.data(), sizeof(::bst::comms::CalibrateSensor_t));
        _calibratePublisher->publish(calibrateSensor);
        break;
    }
#ifdef VEHICLE_FIXEDWING
    case SENSORS_OFFSETS:
    case SENSORS_ORIENTATION:
        spdlog::debug("{}VEHICLE_FIXEDWING packet received", __PRETTY_FUNCTION__);
        break;
#endif

        /* STATE */
    case STATE_STATE:
    case STATE_ESTIMATOR_PARAM:
        spdlog::debug("{}State packet received", __PRETTY_FUNCTION__);
        break;

        /* CONTROL */
    case CONTROL_COMMAND: {
        spdlog::debug("{}CONTROL_COMMAND received", __PRETTY_FUNCTION__);
        ::bst::comms::Command_t command;
        memcpy(&command, data.data(), sizeof(::bst::comms::Command_t));
        _controlCommandPublisher->publish(command);
        break;
    }
    case CONTROL_PID: {
        spdlog::debug("{}CONTROL_PID received", __PRETTY_FUNCTION__);
        ::bst::comms::PID_t pid;
        memcpy(&pid, data.data(), sizeof(::bst::comms::PID_t));
        _controlPidPublisher->publish(pid);
        break;
    }
    case CONTROL_FLIGHT_PARAMS:
    case CONTROL_FILTER_PARAMS:
        spdlog::debug("{}Control packet received", __PRETTY_FUNCTION__);
        break;

        /* ACTUATORS */
    case ACTUATORS_VALUES:
    case ACTUATORS_CALIBRATION:
    case ACTUATORS_ROTOR_PARAMS:
    case ACTUATORS_MIXING_PARAMS:
        spdlog::debug("{}Actuators packet received", __PRETTY_FUNCTION__);
        break;

        /* INPUT */
    case INPUT_HANDSET_VALUES:
    case INPUT_HANDSET_SETUP:
    case INPUT_JOYSTICK_VALUES:
    case INPUT_JOYSTICK_SETUP:
        spdlog::debug("{}Input packet received", __PRETTY_FUNCTION__);
        break;

        /* SYSTEM */
    case SYSTEM_POWER_ON:
    case SYSTEM_HEALTH_AND_STATUS:
    case SYSTEM_HARDWARE_ERROR:
    case SYSTEM_REBOOT:
    case SYSTEM_INITIALIZE:
        spdlog::debug("{}System packet received", __PRETTY_FUNCTION__);
        break;

        /* TELEMETRY */
    case TELEMETRY_HEARTBEAT: {
        spdlog::debug("{}TELEMETRY_HEARTBEAT", __PRETTY_FUNCTION__);
        uint8_t bstPacketType = type;
        _systemPublisher->publish(bstPacketType);
        break;
    }

    case TELEMETRY_POSITION: {
        spdlog::debug("{}TELEMETRY_POSITION", __PRETTY_FUNCTION__);
        ::bst::comms::TelemetryPosition_t telemetryPosition;
        memcpy(&telemetryPosition, data.data(), sizeof(::bst::comms::TelemetryPosition_t));
        spdlog::debug("{}\tLatitude:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.latitude);
        spdlog::debug("{}\tLongitude:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.longitude);
        spdlog::debug("{}\tAltitude:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.altitude);
        spdlog::debug("{}\tposition.x:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.position.x);
        spdlog::debug("{}\tposition.y:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.position.y);
        spdlog::debug("{}\tposition.z:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.position.z);
        spdlog::debug("{}\tvelocity.x:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.velocity.x);
        spdlog::debug("{}\tvelocity.y:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.velocity.y);
        spdlog::debug("{}\tvelocity.z:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.velocity.z);
        spdlog::debug("{}\tacceleration.x:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.acceleration.x);
        spdlog::debug("{}\tacceleration.y:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.acceleration.y);
        spdlog::debug("{}\tacceleration.z:\t{:.20f}", __PRETTY_FUNCTION__, telemetryPosition.acceleration.z);
        _telemetryPositionPublisher->publish(telemetryPosition);
        break;
    }

    case TELEMETRY_ORIENTATION: {
        spdlog::debug("{}TELEMETRY_ORIENTATION", __PRETTY_FUNCTION__);
        ::bst::comms::TelemetryOrientation_t telemetryOrientation;
        memcpy(&telemetryOrientation, data.data(), sizeof(TelemetryOrientation_t));
        spdlog::debug("{}\tq[0]:\t{:.20f}", __PRETTY_FUNCTION__, telemetryOrientation.q[0]);
        spdlog::debug("{}\tq[1]:\t{:.20f}", __PRETTY_FUNCTION__, telemetryOrientation.q[1]);
        spdlog::debug("{}\tq[2]:\t{:.20f}", __PRETTY_FUNCTION__, telemetryOrientation.q[2]);
        spdlog::debug("{}\tq[3]:\t{:.20f}", __PRETTY_FUNCTION__, telemetryOrientation.q[3]);
        _telemetryOrientationPublisher->publish(telemetryOrientation);
        break;
    }

    case TELEMETRY_SYSTEM: {
        spdlog::debug("{}TELEMETRY_SYSTEM, size: {}", __PRETTY_FUNCTION__, size);
        ::bst::comms::TelemetrySystem_t telemetrySystem;
        memcpy(&telemetrySystem, data.data(), sizeof(TelemetrySystem_t));
        for (int i = 0; i < size; i ++) {
            spdlog::debug("{}. data[{}] = {}", __PRETTY_FUNCTION__, i, (int) data[i]);
        }
        spdlog::debug("{}\tflight_mode:\t{}", __PRETTY_FUNCTION__, telemetrySystem.flight_mode);
        spdlog::debug("{}\tautopilot_mode:\t{}", __PRETTY_FUNCTION__, telemetrySystem.autopilot_mode);
        spdlog::debug("{}\tbatt_percent:\t{}", __PRETTY_FUNCTION__, telemetrySystem.batt_percent);
        spdlog::debug("{}\tsatellites:\t{}", __PRETTY_FUNCTION__, telemetrySystem.satellites);
        spdlog::debug("{}\terror_code:\t{}", __PRETTY_FUNCTION__, telemetrySystem.error_code);
        _telemetrySystemPublisher->publish(telemetrySystem);
        break;
    }
    case TELEMETRY_PRESSURE: {
        spdlog::debug("{}TELEMETRY_PRESSURE", __PRETTY_FUNCTION__);
        ::bst::comms::TelemetryPressure_t telemetryPressure;
        memcpy(&telemetryPressure, data.data(), sizeof(TelemetryPressure_t));
        _telemetryPressurePublisher->publish(telemetryPressure);
        break;
    }
    case TELEMETRY_CONTROL: {
        spdlog::debug("{}TELEMETRY_PRESSURE", __PRETTY_FUNCTION__);
        ::bst::comms::TelemetryControl_t telemetryControl;
        memcpy(&telemetryControl, data.data(), sizeof(::bst::comms::TelemetryControl_t));
        _telemetryControlPublisher->publish(telemetryControl);
        break;
    }
    case TELEMETRY_GCS: {
        spdlog::debug("{}TELEMETRY_GCS", __PRETTY_FUNCTION__);
        ::bst::comms::gcs::TelemetryGCS_t telemetryGCS;
        memcpy(&telemetryGCS, data.data(), sizeof(::bst::comms::gcs::TelemetryGCS_t));
        _telemetryGCSPublisher->publish(telemetryGCS);
        break;
    }
    case TELEMETRY_GCS_LOCATION:
        spdlog::debug("{}Telemetry packet received", __PRETTY_FUNCTION__);
        break;

        /* ERRORS */
    default:
        spdlog::debug("{}UNKNOWN type: {}", __PRETTY_FUNCTION__, type);
        break;
    case INVALID_PACKET:
        spdlog::debug("{}INVALID_PACKET", __PRETTY_FUNCTION__);
        break;
    }
}

uint8_t kpsr::bst::Bst2KpsrBroadcaster::receiveCommand(uint8_t type, std::vector<unsigned char> data, uint16_t size, const void * parameter) {
    // validate this is a command
    if( size != sizeof(Command_t) ) {
        spdlog::debug("{}receiveCommand: invlid data size - size={}", __PRETTY_FUNCTION__, size);
        return false;
    }

    Command_t *command = (Command_t*)data.data();

    switch(command->id) {
    /* PAYLOAD */
    case CMD_PAYLOAD_CONTROL: {
        PayloadControl_t payloadControl = (PayloadControl_t)command->value;
        _payloadControlPublisher->publish(payloadControl);
        switch((uint8_t)command->value) {
        case PAYLOAD_CTRL_OFF:
            spdlog::debug("{}CMD:PAYLOAD_CTRL_OFF", __PRETTY_FUNCTION__);
            _payloadCurrentState = PAYLOAD_CTRL_OFF;
            return true;

        case PAYLOAD_CTRL_ACTIVE:
            spdlog::debug("{}CMD:PAYLOAD_CTRL_ACTIVE", __PRETTY_FUNCTION__);
            if( _payloadCurrentState != PAYLOAD_CTRL_READY )
                return false;

            _payloadCurrentState = PAYLOAD_CTRL_ACTIVE;
            return true;

        case PAYLOAD_CTRL_SHUTDOWN:
            spdlog::debug("{}CMD:PAYLOAD_CTRL_SHUTDOWN", __PRETTY_FUNCTION__);
            _payloadCurrentState = PAYLOAD_CTRL_SHUTDOWN;
            return true;

        default:
            spdlog::debug("{}receiveCommand: unexpected payload command", __PRETTY_FUNCTION__);
            break;
        }
        break;
    }
    }
    return false;
}

void kpsr::bst::Bst2KpsrBroadcaster::receiveReply(uint8_t type, std::vector<unsigned char> data, uint16_t size, bool ack, const void * parameter)
{
    spdlog::info("{}: type={}, data={}, ack: {}", __PRETTY_FUNCTION__, (int) type, (int) data[0], (ack ? "ACK" : "NACK"));

    kpsr::bst::BstReplyMessageBuilder builder;
    std::shared_ptr<BstReplyMessage> message = builder
            .withType(data[0])
            .withId(type)
            .withAck(ack).build();
    _bst2KpsrReplyMessagePublisher->publish(message);
}

bool kpsr::bst::Bst2KpsrBroadcaster::publish(uint8_t type, uint8_t param)
{
    spdlog::debug("{}publish: type={}", __PRETTY_FUNCTION__, type);

    // do some with status request
    switch(type) {
    case SYSTEM_INITIALIZE:
    {
        spdlog::debug("{}SYSTEM_INITIALIZE", __PRETTY_FUNCTION__);
        ::bst::comms::SystemInitialize_t systemInitialize;
        // initialize system_init packet
        systemInitialize.vehicle_type = PAYLOAD_NODE;
        systemInitialize.serial_num   = _serialNumber;
        _systemInitializePublisher->publish(systemInitialize);
        break;
    }

    default:
        return false;
    }

    return true;
}
