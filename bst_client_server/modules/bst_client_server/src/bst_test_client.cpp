/*****************************************************************************
*
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <functional>

/* BST */
#include "bst_module_basic.h"
#include "bst_module_flight_plan.h"
#include "bst_protocol.h"
#include "helper_functions.h"
#include "multicopter.h"
#include "gcs.h"


#include <klepsydra/core/time_utils.h>

#include <klepsydra/bst_comms/waypoint_message.h>
#include <klepsydra/bst_comms/waypoint_command_message.h>

#include <klepsydra/bst_client_server/bst_test_client.h>

BstTestClient::BstTestClient(kpsr::bst::BstClient & bstClient)
    : _bstClient(bstClient)
    , _isRunning(false)
{}

void BstTestClient::run() {
    _clientThread = std::thread([this]() {
        initializeClient();
        printTestHelp();

        _isRunning = true;
        while(_isRunning) {
            updateClient();
        }
        exitClient();
    });
    _clientThread.join();
}

void BstTestClient::printTestHelp() {
    printf("Keys:\n");
    printf("  h   : Send payload heartbeat\n");
    printf("  r   : Send payload ready comannd\n");
    printf("  o   : Send payload off comannd\n");
    printf("  s   : Send payload shutdown comannd\n");
    printf("\n");
    printf("  t   : Send launch / land comannd\n");
    printf("\n");
    printf("  i   : Send vel_x=1\n");
    printf("  k   : Send vel_x=-1\n");
    printf("  j   : Send vel_y=-1\n");
    printf("  l   : Send vel_y=1\n");
    printf("\n");
    printf("  u   : Send vrate=1\n");
    printf("  d   : Send vrate=-1\n");
    printf("\n");
    printf("  f   : Send simple flight plan consisting of waypoint 80\n");
    printf("  w   : Command aircraft to go to waypoint 80\n");
    printf("\n");
    printf("  p   : print this help\n");
}

void BstTestClient::initializeClient() {

    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to initial_settings*/
    tcgetattr( STDIN_FILENO, &initial_settings);
    /*now the settings will be copied*/
    new_settings = initial_settings;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    new_settings.c_lflag &= ~(ICANON);
    new_settings.c_lflag &= ~ECHO;

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &new_settings);
}

void BstTestClient::updateClient() {
    char input = getchar();

    if(input > 0) {
        switch(input) {
        case 'r':
            if (_bstClient.acquirePayloadControl()) {
                spdlog::info("Sending payload ready command mode ...");
            }
            else {
                spdlog::info("Cannot send the payload ready command. Client is in {}", _bstClient.getCurrentState());
            }
            break;
        case 'z': {
            kpsr::bst::BstRequestMessage message;
            message.id = SENSORS_CALIBRATE;
            message.type = SENSORS_GYROSCOPE;
            message.value = SENT;
            spdlog::info("Sending calibrate sensor command");
            _bstClient.sendCommand(message);
            break;
        }
        case 'o':
            if (_bstClient.releasePayloadControl()) {
                spdlog::info("Sending payload off command ...");
            }
            else {
                spdlog::info("Cannot send the payload off command. Client is in {}", _bstClient.getCurrentState());
            }
            break;

        case 't':
            spdlog::info("Launch / Land command ...");
            if (_bstClient.getCurrentState() == "bstClientStateMachine:payloadControlReady") {
                spdlog::info("Sending take off command...");
                _bstClient.takeoff();
            } else if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending land command...");
                _bstClient.land();
            }
            else {
                spdlog::info("Cannot send the Launch/land command. Client is in {}", _bstClient.getCurrentState());
            }

            break;

        case 'i': {
            kpsr::bst::BstRequestMessage message;
            message.id = CONTROL_COMMAND;
            message.type = bst::comms::multicopter::CMD_X_VEL;
            message.value = 1.0;
            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending velocity command");
                _bstClient.sendCommand(message);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }
            break;
        }
        case 'k': {
            kpsr::bst::BstRequestMessage message;
            message.id = CONTROL_COMMAND;
            message.type = bst::comms::multicopter::CMD_X_VEL;
            message.value = -1.0;
            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending velocity command");
                _bstClient.sendCommand(message);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }
            break;
        }
        case 'j': {
            kpsr::bst::BstRequestMessage message;
            message.id = CONTROL_COMMAND;
            message.type = bst::comms::multicopter::CMD_Y_VEL;
            message.value = -1.0;
            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending velocity command");
                _bstClient.sendCommand(message);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }
            break;
        }
        case 'l': {
            kpsr::bst::BstRequestMessage message;
            message.id = CONTROL_COMMAND;
            message.type = bst::comms::multicopter::CMD_Y_VEL;
            message.value = 1.0;
            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending velocity command");
                _bstClient.sendCommand(message);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }
            break;
        }
        case 'u': {
            kpsr::bst::BstRequestMessage message;
            message.id = CONTROL_COMMAND;
            message.type = CMD_VRATE;
            message.value = 1.0;
            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending velocity command");
                _bstClient.sendCommand(message);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }
            break;
        }
        case 'd': {
            kpsr::bst::BstRequestMessage message;
            message.id = CONTROL_COMMAND;
            message.type = CMD_VRATE;
            message.value = -1.0;
            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending velocity command");
                _bstClient.sendCommand(message);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }
            break;
        }
            // Send flight plan consisting of one orbit waypoint
        case 'f': {
            // Add a waypoint to the plan
            kpsr::bst::WaypointCommandMessage waypointComand;
            waypointComand.plan.resize(1);

            waypointComand.plan[0].num = 80;
            waypointComand.plan[0].next = 80;
            waypointComand.plan[0].latitude = 40.021801;  // [deg]
            waypointComand.plan[0].longitude = -105.21730804;  // [deg]
            waypointComand.plan[0].altitude = 1700.020752;  // [m]
            waypointComand.plan[0].radius = 0.0;  // [m]

            waypointComand.mode = ADD;
            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending a single waypoint");
                _bstClient.sendWaypoints(waypointComand);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }

            break;
        }
        case 'F': {
            spdlog::info("Sending a four waypoint flight plan");

            kpsr::bst::WaypointCommandMessage waypointComand;
            waypointComand.plan.resize(4);

            waypointComand.plan[0].num = 80;
            waypointComand.plan[0].next = 81;
            waypointComand.plan[0].latitude = 40.021904;  // [deg]
            waypointComand.plan[0].longitude = -105.217194;  // [deg]
            waypointComand.plan[0].altitude = 1620.0;  // [m]
            waypointComand.plan[0].radius = 0.0;  // [m]

            waypointComand.plan[1].num = 81;
            waypointComand.plan[1].next = 82;
            waypointComand.plan[1].latitude = 40.021904;  // [deg]
            waypointComand.plan[1].longitude = -105.216980;  // [deg]
            waypointComand.plan[1].altitude = 1620.0;  // [m]
            waypointComand.plan[1].radius = 0.0;  // [m]

            waypointComand.plan[2].num = 82;
            waypointComand.plan[2].next = 83;
            waypointComand.plan[2].latitude = 40.021759;  // [deg]
            waypointComand.plan[2].longitude = -105.216980;  // [deg]
            waypointComand.plan[2].altitude = 1620.0;  // [m]
            waypointComand.plan[2].radius = 0.0;  // [m]

            waypointComand.plan[3].num = 83;
            waypointComand.plan[3].next = 80;
            waypointComand.plan[3].latitude = 40.021759;  // [deg]
            waypointComand.plan[3].longitude = -105.217194;  // [deg]
            waypointComand.plan[3].altitude = 1620.0;  // [m]
            waypointComand.plan[3].radius = 0.0;  // [m]

            waypointComand.mode = ADD;

            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending a single waypoint");
                _bstClient.sendWaypoints(waypointComand);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }

            break;
        }
        case 'D': {
            spdlog::info("Delete waypoints 80,81,82,83");

            kpsr::bst::WaypointCommandMessage waypointComand;
            waypointComand.plan.resize(4);

            waypointComand.plan[0].num = 80;
            waypointComand.plan[1].num = 81;
            waypointComand.plan[2].num = 82;
            waypointComand.plan[3].num = 83;
            waypointComand.mode = DELETE;

            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending a single waypoint");
                _bstClient.sendWaypoints(waypointComand);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }

            break;
        }
            // Send vehicle to waypoint 80
        case 'w': {
            spdlog::info("Sending vehicle to waypoint 80");
            kpsr::bst::BstRequestMessage message;
            message.id = CONTROL_COMMAND;
            message.type = CMD_WAYPOINT;
            message.value = 80;
            if (_bstClient.getCurrentState() == "bstClientStateMachine:flying") {
                spdlog::info("Sending velocity command");
                _bstClient.sendCommand(message);
            }
            else {
                spdlog::info("Cannot send the velocity command. Client is in {}", _bstClient.getCurrentState());
            }
            break;
        }
        case 'p':
            printTestHelp();
            break;

        case 3: // <CTRL-C>
        case 'q':
            spdlog::info("Keyboard caught exit signal ...");
            _isRunning = false;
            break;

        default:
            break;
        }
        input = 0;
    } else {
        clearerr(stdin);
    }
}

void BstTestClient::exitClient() {
    tcsetattr(STDIN_FILENO, TCSANOW, &initial_settings);
}
