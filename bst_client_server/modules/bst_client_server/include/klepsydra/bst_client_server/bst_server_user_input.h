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
#ifndef BST_SERVER_USER_INPUT_H
#define BST_SERVER_USER_INPUT_H

#include <termios.h>
#include <thread>
#include <atomic>

class BstServerUserInput {
public:

    BstServerUserInput();

    void run();

private:

    void printTestHelp(void);
    void exitServer(void);
    void initializeServer(void);
    void updateServer(void);

    termios initial_settings, new_settings;

    std::atomic_bool _isRunning;
    std::thread _serverThread;
};

#endif //BST_SERVER_USER_INPUT_H
