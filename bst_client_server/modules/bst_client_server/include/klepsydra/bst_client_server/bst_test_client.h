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
#ifndef BST_TEST_CLIENT_H
#define BST_TEST_CLIENT_H

#include <termios.h>
#include <thread>
#include <atomic>

#include <klepsydra/bst_client_server/bst_client.h>

class BstTestClient {
public:

    BstTestClient(kpsr::bst::BstClient & bstClient);

    void run();

private:

    void printTestHelp(void);
    void exitClient(void);
    void initializeClient(void);
    void updateClient(void);

    termios initial_settings, new_settings;

    kpsr::bst::BstClient & _bstClient;

    std::atomic_bool _isRunning;
    std::thread _clientThread;
};

#endif //BST_TEST_CLIENT_H
