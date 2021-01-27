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
#include <getopt.h>

#include <klepsydra/bst_client_server/bst_main_helper.h>

std::string kpsr::bst::BstMainHelper::getConfFileFromParams(int argc, char *argv[]) {
    char c;
    while ((c = getopt(argc, argv, "f:")) != -1) {
        switch(c) {
        case 'f':
            return std::string(optarg);
            break;
        default:
            break;
        }
    }

    printHelp();
    return "";
}

void kpsr::bst::BstMainHelper::printHelp() {
    printf("Usage: bst_server [OPTIONS]\n");
    printf("    -f <configuration file> : default null\n");
    exit(0);
}

std::string kpsr::bst::BstMainHelper::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);
    return buf;
}