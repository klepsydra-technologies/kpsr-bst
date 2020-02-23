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
#ifndef BST_CLIENT_EVENTLOOP_PROVIDER_H
#define BST_CLIENT_EVENTLOOP_PROVIDER_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/bst_comms/bst_client_middleware_provider.h>

#include <klepsydra/bst_client_server/bst_client.h>

namespace kpsr {
namespace bst {
template <std::size_t BufferSize>
class BstClientEventloopProvider
{
public:
    BstClientEventloopProvider(Container * container, Environment * environment,
                               kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & eventloopMiddlewareProvider,
                               BstClientMiddlewareProvider * bstClientMiddlewareProvider)
        : _environment(environment)
        , _eventloopMiddlewareProvider(eventloopMiddlewareProvider)
        , _bstClientMiddlewareProvider(bstClientMiddlewareProvider)
        , _stateMachineListener()
        , _telemetrySystemEventListener()
        , _bstClient(container, _environment,
                     _eventloopMiddlewareProvider.template getSubscriber<std::string>("bst_client_ext_state_machine"),
                     _eventloopMiddlewareProvider.template getPublisher<std::string>("bst_client_ext_state_machine", 0, nullptr, nullptr),
                     _eventloopMiddlewareProvider.template getSubscriber<std::string>("bst_client_state_machine"),
                     _eventloopMiddlewareProvider.template getPublisher<std::string>("bst_client_state_machine", 0, nullptr, nullptr),
                     _stateMachineListener,
                     _bstClientMiddlewareProvider,
                     &_telemetrySystemEventListener)
    {}

    void start() {
        _bstClient.startup();
        int period;
        _environment->getPropertyInt("bst_client_run_period_microsecs", period);
        if (period > 0) {
            _bstClientMiddlewareProvider->getScheduler()->startScheduledService(period, true, &_bstClient);
        }
    }

    void stop() {
        _bstClientMiddlewareProvider->getScheduler()->stopScheduledTask("BST_CLIENT_EXEC");
        _bstClient.shutdown();
    }

    BstClient & getBstClient() {
        return _bstClient;
    }

private:
    Environment * _environment;
    kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> & _eventloopMiddlewareProvider;
    BstClientMiddlewareProvider * _bstClientMiddlewareProvider;
    kpsr::mem::CacheListener<std::string> _stateMachineListener;
    kpsr::mem::MultiThreadCacheListener<TelemetrySystem_t> _telemetrySystemEventListener;
    BstClient _bstClient;
};

}

}

#endif // BST_CLIENT_EVENTLOOP_PROVIDER_H
