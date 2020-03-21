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

#include <spdlog/spdlog.h>

#include <klepsydra/core/time_utils.h>

#include <klepsydra/bst_client_server/bst_server_user_input.h>

BstServerUserInput::BstServerUserInput()
    : _isRunning(false)
{}

void BstServerUserInput::run() {
    _serverThread = std::thread([this]() {
        initializeServer();
        printTestHelp();

        _isRunning = true;
        while(_isRunning) {
            updateServer();
        }
        exitServer();
    });
    _serverThread.join();
}

void BstServerUserInput::printTestHelp() {
    printf("Keys:\n");
    printf("  q   : Quit\n");
    printf("  p   : Print help\n");
}

void BstServerUserInput::initializeServer() {

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

void BstServerUserInput::updateServer() {
    char input = getchar();

    if(input > 0) {
        switch(input) {
        case 'p':
            printTestHelp();
            break;
        case 3: // <CTRL-C>
            spdlog::info("Keyboard caught exit signal ...");
            _isRunning = false;
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

void BstServerUserInput::exitServer() {
    tcsetattr(STDIN_FILENO, TCSANOW, &initial_settings);
}
