// Copyright 2023 Klepsydra Technologies AG
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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