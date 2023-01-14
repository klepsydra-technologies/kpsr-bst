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
