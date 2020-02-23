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

#include <klepsydra/mem_trajectory_monitoring/sf_rest_traj_mon_container_provider.h>

kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider::BasicRestTrajectoryContainerProvider(kpsr::restapi::RestEndpoint & restEndpoint,
                                                                                                              kpsr::trajectory::TrajectoryDataProvider * trajectoryDataProvider,
                                                                                                              Environment * env, std::string applicationName)
    : _restEndpoint(restEndpoint)
    , _trajectoryCommsProvider()
    , _restTrajMonInterface(_trajectoryCommsProvider.trajectoryCallbackHandlerProvider.getCallbackHandler())
    , _container(env, applicationName, trajectoryDataProvider,
                 _trajectoryCommsProvider.trajectoryRequestSubscriber,
                 _trajectoryCommsProvider.trajectoryResponsePublisher)
{
    _restEndpoint.attachApplication(&_restTrajMonInterface);
}

kpsr::trajectory::TrajectoryContainer & kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider::getContainer() {
    return _container;
}

void kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider::start() {
    _container.start();
    _trajectoryCommsProvider.start();
}

void kpsr::trajectory::restapi::BasicRestTrajectoryContainerProvider::stop() {
    _trajectoryCommsProvider.stop();
    _container.stop();
}
