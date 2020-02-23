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
#ifndef BST2KPSR_BROADCASTER_H
#define BST2KPSR_BROADCASTER_H

#include <inttypes.h>

/* BST */
#include "bst_module_basic.h"
#include "bst_module_flight_plan.h"
#include "bst_protocol.h"
#include "helper_functions.h"
#include "multicopter.h"
#include "gcs.h"

#include <klepsydra/core/publisher.h>

#include <klepsydra/bst_comms/bst_reply_message.h>

namespace kpsr
{
namespace bst
{
/**
 * @brief The Bst2KpsrBroadcaster class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst
 *
 * @details This class is in charge of broadcasting all messages coming from BST into Klepsydra.
 */
class Bst2KpsrBroadcaster {
public:
    Bst2KpsrBroadcaster(Publisher<Sensors_t> * sensorPublisher,
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
                        uint32_t serialNumber);

    /**
     * @brief receive
     * @param type
     * @param data
     * @param size
     * @param parameter
     */
    void receive(uint8_t type, std::vector<unsigned char> data, uint16_t size, const void * parameter);

    /**
     * @brief receiveCommand
     * @param type
     * @param data
     * @param size
     * @param parameter
     * @return
     */
    uint8_t receiveCommand(uint8_t type, std::vector<unsigned char> data, uint16_t size, const void * parameter);

    /**
     * @brief receiveReply
     * @param type
     * @param data
     * @param size
     * @param ack
     * @param parameter
     */
    void receiveReply(uint8_t type, std::vector<unsigned char> data, uint16_t size, bool ack, const void * parameter);

    /**
     * @brief publish
     * @param type
     * @param param
     * @return
     */
    bool publish(uint8_t type, uint8_t param);

    /**
     * @brief _sensorPublisher
     */
    Publisher<Sensors_t> * _sensorPublisher;

    /**
     * @brief _calibratePublisher
     */
    Publisher<CalibrateSensor_t> * _calibratePublisher;

    /**
     * @brief _controlCommandPublisher
     */
    Publisher<Command_t> * _controlCommandPublisher;

    /**
     * @brief _controlPidPublisher
     */
    Publisher<PID_t> * _controlPidPublisher;

    /**
     * @brief _systemPublisher
     */
    Publisher<uint8_t> * _systemPublisher;

    /**
     * @brief _systemInitializePublisher
     */
    Publisher<SystemInitialize_t> * _systemInitializePublisher;

    /**
     * @brief _telemetryPositionPublisher
     */
    Publisher<TelemetryPosition_t> * _telemetryPositionPublisher;

    /**
     * @brief _telemetryOrientationPublisher
     */
    Publisher<TelemetryOrientation_t> * _telemetryOrientationPublisher;

    /**
     * @brief _telemetrySystemPublisher
     */
    Publisher<TelemetrySystem_t> * _telemetrySystemPublisher;

    /**
     * @brief _telemetryPressurePublisher
     */
    Publisher<TelemetryPressure_t> * _telemetryPressurePublisher;

    /**
     * @brief _telemetryControlPublisher
     */
    Publisher<::bst::comms::TelemetryControl_t> * _telemetryControlPublisher;

    /**
     * @brief _telemetryGCSPublisher
     */
    Publisher<gcs::TelemetryGCS_t> * _telemetryGCSPublisher;

    /**
     * @brief _payloadControlPublisher
     */
    Publisher<PayloadControl_t> * _payloadControlPublisher;

    /**
     * @brief _bst2KpsrReplyMessagePublisher
     */
    Publisher<BstReplyMessage> * _bst2KpsrReplyMessagePublisher;

    /**
     * @brief _payloadCurrentState
     */
    PayloadControl_t _payloadCurrentState;

    /**
     * @brief _serialNumber
     */
    uint32_t _serialNumber;

private:
};
}
}

#endif // BST2KPSR_BROADCASTER_H
