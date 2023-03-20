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

#include <math.h>

/* BST */
#include "comm_interface.h"
#include "comm_packets.h"
#include "comm_protocol.h"
#include "helper_functions.h"

SystemInitialize_t system_initialize;

float start_time = 0.0;

extern "C" {
float getElapsedTime()
{
#ifdef __APPLE__
    uint64_t now = mach_absolute_time();
    float conversion = 0.0;
    mach_timebase_info_data_t info;
    kern_return_t err = mach_timebase_info(&info);
    if (err == 0)
        conversion = 1e-9 * (float) info.numer / (float) info.denom;
    float current_time = conversion * (float) now;
#else
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    float current_time = (float) now.tv_sec + (float) now.tv_nsec / SEC_TO_NSEC;
#endif
    return current_time - start_time;
}
}
