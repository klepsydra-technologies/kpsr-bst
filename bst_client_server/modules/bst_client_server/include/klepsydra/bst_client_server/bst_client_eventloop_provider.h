/*
 * Copyright 2023 Klepsydra Technologies AG
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BST_CLIENT_EVENTLOOP_PROVIDER_H
#define BST_CLIENT_EVENTLOOP_PROVIDER_H

#include <klepsydra/high_performance/event_loop_middleware_provider.h>

#include <klepsydra/bst_comms/bst_client_middleware_provider.h>

#include <klepsydra/bst_client_server/bst_client.h>

namespace kpsr {
namespace bst {
template<std::size_t BufferSize>
class BstClientEventloopProvider
{
public:
    BstClientEventloopProvider(
        Container *container,
        Environment *environment,
        kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> &eventloopMiddlewareProvider,
        BstClientMiddlewareProvider *bstClientMiddlewareProvider)
        : _environment(environment)
        , _eventloopMiddlewareProvider(eventloopMiddlewareProvider)
        , _bstClientMiddlewareProvider(bstClientMiddlewareProvider)
        , _stateMachineListener()
        , _telemetrySystemEventListener()
        , _bstClient(container,
                     _environment,
                     _eventloopMiddlewareProvider.template getSubscriber<std::string>(
                         "bst_client_ext_state_machine"),
                     _eventloopMiddlewareProvider.template getPublisher<std::string>(
                         "bst_client_ext_state_machine", 0, nullptr, nullptr),
                     _eventloopMiddlewareProvider.template getSubscriber<std::string>(
                         "bst_client_state_machine"),
                     _eventloopMiddlewareProvider.template getPublisher<std::string>(
                         "bst_client_state_machine", 0, nullptr, nullptr),
                     _stateMachineListener,
                     _bstClientMiddlewareProvider,
                     &_telemetrySystemEventListener)
    {}

    void start()
    {
        _bstClient.startup();
        int period;
        _environment->getPropertyInt("bst_client_run_period_microsecs", period);
        if (period > 0) {
            _bstClientMiddlewareProvider->getScheduler()->startScheduledService(period,
                                                                                true,
                                                                                &_bstClient);
        }
    }

    void stop()
    {
        _bstClientMiddlewareProvider->getScheduler()->stopScheduledTask("BST_CLIENT_EXEC");
        _bstClientMiddlewareProvider->getScheduler()->stopScheduledService(&_bstClient);
        _bstClient.shutdown();
    }

    BstClient &getBstClient() { return _bstClient; }

private:
    Environment *_environment;
    kpsr::high_performance::EventLoopMiddlewareProvider<BufferSize> &_eventloopMiddlewareProvider;
    BstClientMiddlewareProvider *_bstClientMiddlewareProvider;
    kpsr::mem::CacheListener<std::string> _stateMachineListener;
    kpsr::mem::MultiThreadCacheListener<TelemetrySystem_t> _telemetrySystemEventListener;
    BstClient _bstClient;
};

} // namespace bst

} // namespace kpsr

#endif // BST_CLIENT_EVENTLOOP_PROVIDER_H
