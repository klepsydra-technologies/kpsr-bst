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
#ifndef BST_SDK_BROADCASTER_H
#define BST_SDK_BROADCASTER_H

/* BST */
#include "netuas_socket.h"
#include "bst_module_basic.h"
#include "bst_protocol.h"
#include "helper_functions.h"
#include "comm_packets.h"
#include "comm_interface.h"
#include "comm_protocol.h"
#include "bst_module_flight_plan.h"
#include "flight_plan.h"

#include <inttypes.h>
#include <mutex>

#include <klepsydra/core/service.h>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>

#include <klepsydra/bst_comms/bst_request_message.h>
#include <klepsydra/bst_comms/waypoint_command_message.h>

namespace kpsr
{
namespace bst
{
/**
 * @brief The CommInterfaceService class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst
 *
 * @details Serial communication service that handles the reading of reading and writing.
 */
class CommInterfaceService : public Service {
public:
    /**
     * @brief CommInterfaceService
     * @param environment
     * @param bstRequestMessageSubscriber
     * @param bstWaypointCommandtMessageSubscriber
     * @param _systemInitializeSubscriber
     */
    CommInterfaceService(Environment *environment,
                         Subscriber<BstRequestMessage> * bstRequestMessageSubscriber,
                         Subscriber<WaypointCommandMessage> * bstWaypointCommandtMessageSubscriber,
                         Subscriber<SystemInitialize_t> * _systemInitializeSubscriber);

    /**
     * @brief start
     */
    void start();

    /**
     * @brief execute
     */
    void execute();

    /**
     * @brief stop
     */
    void stop();

    /**
     * @brief onBstRequestMessageReceived
     * @param eventData
     */
    void onBstRequestMessageReceived(const BstRequestMessage  & eventData);

    /**
     * @brief onBstWaypointCommandMessageReceived
     * @param eventData
     */
    void onBstWaypointCommandMessageReceived(const WaypointCommandMessage  & eventData);

    /**
     * @brief onSystemInitializeReceived
     * @param eventData
     */
    void onSystemInitializeReceived(const ::bst::comms::SystemInitialize_t & eventData);

private:

    void bstCommunicationsInit ();
    void sendPayloadControlActive();

    CommunicationsProtocol * _commHandler;
    CommunicationsInterface * _commInterface;

    Subscriber<BstRequestMessage> * _bstRequestMessageSubscriber;
    Subscriber<WaypointCommandMessage> * _bstWaypointCommandtMessageSubscriber;
    Subscriber<SystemInitialize_t> * _systemInitializeSubscriber;

    BSTModuleBasic _basicModule;
    BSTModuleFlightPlan _flightPlanModule;

    Packet tx_packet;

    bool started;

    Waypoint_t _tempWaypoints[MAX_WAYPOINTS];

    FlightPlanMap_t _flightPlanMap;
    FlightPlan _flightPlan;

    mutable std::mutex _mutex;
};
}
}

#endif // BST_SDK_BROADCASTER_H
