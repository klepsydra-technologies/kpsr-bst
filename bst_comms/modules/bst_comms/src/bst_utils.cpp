/*****************************************************************************
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
/* BST */
#include "helper_functions.h"
#include "comm_packets.h"
#include "comm_interface.h"
#include "comm_protocol.h"

SystemInitialize_t system_initialize;

double start_time = 0.0;

float getElapsedTime() {
#ifdef __APPLE__
	uint64_t now = mach_absolute_time();
	float conversion  = 0.0;
	mach_timebase_info_data_t info;
	kern_return_t err = mach_timebase_info( &info );
	if( err == 0  )
		conversion = 1e-9 * (float) info.numer / (float) info.denom;
	float current_time = conversion * (float) now;
#else
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	double current_time = (double)now.tv_sec + (double)now.tv_nsec / SEC_TO_NSEC;
#endif
	return current_time - start_time;
}


