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

#ifndef BST_SDK_BROADCASTER_H
#define BST_SDK_BROADCASTER_H

/* BST */
#include "bst_module_basic.h"
#include "bst_module_flight_plan.h"
#include "bst_protocol.h"
#include "comm_interface.h"
#include "comm_packets.h"
#include "comm_protocol.h"
#include "flight_plan.h"
#include "helper_functions.h"
#include "netuas_socket.h"

#include <inttypes.h>
#include <mutex>

#include <klepsydra/core/publisher.h>
#include <klepsydra/core/service.h>
#include <klepsydra/core/subscriber.h>

#include <klepsydra/bst_comms/bst_request_message.h>
#include <klepsydra/bst_comms/waypoint_command_message.h>

namespace kpsr {
namespace bst {
/**
 * @brief The CommInterfaceService class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst
 *
 * @details Serial communication service that handles the reading of reading and writing.
 */
class CommInterfaceService : public Service
{
public:
    /**
     * @brief CommInterfaceService
     * @param environment
     * @param bstRequestMessageSubscriber
     * @param bstWaypointCommandtMessageSubscriber
     * @param _systemInitializeSubscriber
     */
    CommInterfaceService(Environment *environment,
                         Subscriber<BstRequestMessage> *bstRequestMessageSubscriber,
                         Subscriber<WaypointCommandMessage> *bstWaypointCommandtMessageSubscriber,
                         Subscriber<SystemInitialize_t> *_systemInitializeSubscriber);

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
    void onBstRequestMessageReceived(const BstRequestMessage &eventData);

    /**
     * @brief onBstWaypointCommandMessageReceived
     * @param eventData
     */
    void onBstWaypointCommandMessageReceived(const WaypointCommandMessage &eventData);

    /**
     * @brief onSystemInitializeReceived
     * @param eventData
     */
    void onSystemInitializeReceived(const ::bst::comms::SystemInitialize_t &eventData);

private:
    void bstCommunicationsInit();
    void sendPayloadControlActive();

    CommunicationsProtocol *_commHandler;
    CommunicationsInterface *_commInterface;

    Subscriber<BstRequestMessage> *_bstRequestMessageSubscriber;
    Subscriber<WaypointCommandMessage> *_bstWaypointCommandtMessageSubscriber;
    Subscriber<SystemInitialize_t> *_systemInitializeSubscriber;

    BSTModuleBasic _basicModule;
    BSTModuleFlightPlan _flightPlanModule;

    Packet tx_packet;

    bool started;

    Waypoint_t _tempWaypoints[MAX_WAYPOINTS];

    FlightPlanMap_t _flightPlanMap;
    FlightPlan _flightPlan;

    mutable std::mutex _mutex;
};
} // namespace bst
} // namespace kpsr

#endif // BST_SDK_BROADCASTER_H
