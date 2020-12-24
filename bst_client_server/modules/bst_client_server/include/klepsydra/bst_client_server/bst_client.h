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
#ifndef BST_CLIENT_H
#define BST_CLIENT_H

#include <klepsydra/core/cache_listener.h>

#include <klepsydra/bst_client_server/client_state_machine.h>

namespace kpsr {
namespace bst {
class BstClient : public Service {
public:
    BstClient(Container * container,
              Environment * environment,
              Subscriber<std::string> * clientStateMachineExtSubscriber,
              Publisher<std::string> * clientStateMachineExtPublisher,
              Subscriber<std::string> * clientStateMachineSubscriber,
              Publisher<std::string> * clientStateMachinePublisher,
              kpsr::mem::CacheListener<std::string> & clientStateMachineListener,
              BstClientMiddlewareProvider * bstClientMiddlewareProvider,
              kpsr::mem::CacheListener<TelemetrySystem_t> * telemetrySystemEventListener);

    void start() override;

    void stop() override;

    void execute() override;

    bool acquirePayloadControl();

    bool releasePayloadControl();

    bool takeoff();

    bool land();

    bool sendCommand(const BstRequestMessage & command);

    bool sendWaypoints(const WaypointCommandMessage & command);

    std::string getCurrentState();

private:
    Subscriber<std::string> * _clientStateMachineSubscriber;
    Publisher<std::string> * _clientStateMachineExtPublisher;

    kpsr::mem::CacheListener<std::string> & _clientStateMachineListener;

    BstClientMiddlewareProvider * _bstClientMiddlewareProvider;
    ClientStateMachine _clientStateMachine;
};
}
}
#endif // BST_CLIENT_H
