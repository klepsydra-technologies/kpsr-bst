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

#include <klepsydra/mem_trajectory_monitoring/sf_traj_mon_comms_provider.h>

kpsr::trajectory::mem::BasicTrajectoryCommsProvider::BasicTrajectoryCommsProvider()
    : _trajectoryRequestProvider(nullptr, "trajectory_request", 10, 0, nullptr, nullptr, false)
    , _trajectoryResponseProvider(nullptr, "trajectory_response", 10, 0, nullptr, nullptr, false)
    , trajectoryRequestSubscriber(_trajectoryRequestProvider.getSubscriber())
    , trajectoryRequestPublisher(_trajectoryRequestProvider.getPublisher())
    , trajectoryResponseSubscriber(_trajectoryResponseProvider.getSubscriber())
    , trajectoryResponsePublisher(_trajectoryResponseProvider.getPublisher())
    , trajectoryCallbackHandlerProvider(trajectoryRequestPublisher, trajectoryResponseSubscriber, true)
{}

void kpsr::trajectory::mem::BasicTrajectoryCommsProvider::start() {
    _trajectoryRequestProvider.start();
    _trajectoryResponseProvider.start();
}

void kpsr::trajectory::mem::BasicTrajectoryCommsProvider::stop() {
    _trajectoryRequestProvider.stop();
    _trajectoryResponseProvider.stop();
}
