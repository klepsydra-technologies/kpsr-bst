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

#ifndef BST_TEST_CLIENT_H
#define BST_TEST_CLIENT_H

#include <atomic>
#include <termios.h>
#include <thread>

#include <klepsydra/bst_client_server/bst_client.h>

class BstTestClient
{
public:
    BstTestClient(kpsr::bst::BstClient &bstClient);

    void run();

private:
    void printTestHelp(void);
    void exitClient(void);
    void initializeClient(void);
    void updateClient(void);

    termios initial_settings, new_settings;

    kpsr::bst::BstClient &_bstClient;

    std::atomic_bool _isRunning;
    std::thread _clientThread;
};

#endif //BST_TEST_CLIENT_H
