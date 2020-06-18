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
#include <klepsydra/bst_client_server/bst_server.h>

kpsr::bst::BstServer::BstServer(Container * container,
                                Environment * environment,
                                BstServerMiddlewareProvider * _serverMiddlewareProvider)
    : kpsr::Service(environment, "BST_SERVER")
    , _serverMiddlewareProvider(_serverMiddlewareProvider)
{
    _commInterfaceService = new kpsr::bst::CommInterfaceService(
                _environment,
                _serverMiddlewareProvider->getBstRequestMessageSubscriber(),
                _serverMiddlewareProvider->getBstWaypointCommandMessageSubscriber(),
                _serverMiddlewareProvider->getSystemInitializeSubscriber());

    uint32_t serialNumber;
    int confSerialNumber;
    environment->getPropertyInt("bst_serial_number", confSerialNumber);
    serialNumber = confSerialNumber;
    _bst2KpsrBroadcaster = new Bst2KpsrBroadcaster(_serverMiddlewareProvider->getSensorPublisher(),
                                                   _serverMiddlewareProvider->getCalibratePublisher(),
                                                   _serverMiddlewareProvider->getControlCommandPublisher(),
                                                   _serverMiddlewareProvider->getControlPidPublisher(),
                                                   _serverMiddlewareProvider->getSystemPublisher(),
                                                   _serverMiddlewareProvider->getSystemInitializePublisher(),
                                                   _serverMiddlewareProvider->getTelemetryPositionPublisher(),
                                                   _serverMiddlewareProvider->getTelemetryOrientationPublisher(),
                                                   _serverMiddlewareProvider->getTelemetrySystemPublisher(),
                                                   _serverMiddlewareProvider->getTelemetryPressurePublisher(),
                                                   _serverMiddlewareProvider->getTelemetryControlPublisher(),
                                                   _serverMiddlewareProvider->getTelemetryGCSPublisher(),
                                                   _serverMiddlewareProvider->getPayloadControlPublisher(),
                                                   _serverMiddlewareProvider->getBst2KpsrReplyMessagePublisher(),
                                                   serialNumber);

    _bst2KpsrAdaptorService = new kpsr::bst::Bst2KpsrAdaptorService(
                _environment,
                container,
                *_bst2KpsrBroadcaster);

    _telemetryPoseService = new kpsr::bst::TelemetryPoseService(
                _environment,
                _serverMiddlewareProvider->getTelemetryPositionSubscriber(),
                _serverMiddlewareProvider->getTelemetryOrientationSubscriber(),
                _serverMiddlewareProvider->getPoseEventDataPublisher());

    if (container != nullptr) {
        container->attach(_commInterfaceService);
        container->attach(_bst2KpsrAdaptorService);
        container->attach(_telemetryPoseService);
    }
}

void kpsr::bst::BstServer::start() {
    _commInterfaceService->startup();
    _bst2KpsrAdaptorService->startup();
    _telemetryPoseService->startup();

    int period;
    _environment->getPropertyInt("bst_server_period_microsecs", period);
    scheduler.startScheduledService(period, true, this);
}

void kpsr::bst::BstServer::execute() {
    _commInterfaceService->runOnce();
    _bst2KpsrAdaptorService->runOnce();
    _telemetryPoseService->runOnce();
}

void kpsr::bst::BstServer::stop() {
    scheduler.stopScheduledTask("BST_SERVER");
    scheduler.stopScheduledService(this);

    _commInterfaceService->shutdown();
    _bst2KpsrAdaptorService->shutdown();
    _telemetryPoseService->shutdown();
}
