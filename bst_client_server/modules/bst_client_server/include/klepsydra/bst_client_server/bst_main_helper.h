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

#ifndef BST_MAIN_HELPER_H
#define BST_MAIN_HELPER_H

#include <string>

#include <klepsydra/core/environment.h>

namespace kpsr
{
namespace bst
{
/**
 * @brief The BstMainHelper class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Argument parser helper to retrieve configuration. Example:
@code
    bool fileNameProvided;
    std::string fileName = "";

    fileNameProvided = kpsr::bst::BstMainHelper::getConfFileFromParams(argc, argv, fileName);
@endcode
 */
class BstMainHelper {
public:
    /**
     * @brief getConfFileFromParams
     * @param argc
     * @param argv
     * @param fileName
     * @return
     */
    static std::string getConfFileFromParams(int argc, char *argv[]);
    static std::string currentDateTime();
private:
    void static printHelp();
};
}
}

#endif // BST_MAIN_HELPER_H

