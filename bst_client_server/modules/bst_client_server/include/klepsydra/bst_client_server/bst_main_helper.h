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
 * @copyright Klepsydra Robotics 2017-2018.
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

