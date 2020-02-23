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
#include <stdio.h>
#include <thread>
#include <unistd.h>
#include <math.h>

#include <sstream>
#include <fstream>

#include "gtest/gtest.h"

/* BST */
#include "bst_module_basic.h"
#include "comm_packets.h"
#include "gcs.h"
#include "multicopter.h"

#include <klepsydra/serialization/void_caster_mapper.h>

#include <klepsydra/core/service.h>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/core/event_emitter_middleware_provider.h>
#include <klepsydra/core/cache_listener.h>

#include <klepsydra/zmq_core/zhelpers.hpp>
#include <klepsydra/zmq_core/from_zmq_middleware_provider.h>
#include <klepsydra/zmq_core/to_zmq_middleware_provider.h>

class SimpleWriteService : public kpsr::Service
{
public:
    SimpleWriteService(kpsr::Environment * environment,
                       kpsr::Publisher<bst::comms::SystemInitialize_t> * publisher)
        : kpsr::Service(environment, "SimpleWriteService")
        , _publisher(publisher)
    {}

    void start() {}

    void stop() {}

    void execute() {

        event.vehicle_type = bst::comms::MULTI_COPTER;
        event.model = bst::comms::HW_UNKNOWN;
        event.sw_rev = 1;
        event.hw_rev = 2;
        event.svn_rev = 3;
        event.comms_rev = 4;
        event.serial_num = 5;
        event.vehicle_initialized = 6;
        event.num_inputs = 7;
        event.num_actuators = 8;
        std::memcpy(event.name, "ABC_DEF_GHI_JKL_", 16);
        event.unused[0] = 10;
        event.unused[1] = 11;
        event.unused[2] = 12;
        event.unused[3] = 13;
        event.unused[4] = 14;

        _publisher->publish(event);
    }

    bst::comms::SystemInitialize_t event;
private:
    kpsr::Publisher<bst::comms::SystemInitialize_t> * _publisher;
};

TEST(ZMQStgCoreTests, PubSubTest) {
    std::string serverUrl = "tcp://*:5556";
    std::string topic = "bst_system_init";

    //  Prepare our context and publisher
    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
    publisher.bind(serverUrl);
    publisher.bind("ipc://bst_system_init.ipc");

    kpsr::zmq_mdlw::ToZMQMiddlewareProvider toZMQMiddlewareProvider(nullptr, publisher);
    kpsr::Publisher<SystemInitialize_t> * toZMQPublisher = toZMQMiddlewareProvider.getVoidCasterToMiddlewareChannel<SystemInitialize_t>(topic, 0);

    std::string clientUrl = "tcp://localhost:5556";

    //  Socket to talk to server
    spdlog::info("Collecting updates from bst_system_init server...\n");
    zmq::socket_t subscriber (context, ZMQ_SUB);

    subscriber.connect(clientUrl);
    subscriber.setsockopt(ZMQ_SUBSCRIBE, topic.c_str(), topic.size());

    //  Process 100 updates
    kpsr::zmq_mdlw::FromZmqMiddlewareProvider fromZmqMiddlewareProvider;
    kpsr::zmq_mdlw::FromZmqChannel<std::vector<unsigned char>> * fromZMQProvider = fromZmqMiddlewareProvider.getVoidCasterFromMiddlewareChannel<SystemInitialize_t>(subscriber, 10);
    //spdlog::info("Creating services.");
    SimpleWriteService writeService(nullptr, toZMQPublisher);

    kpsr::EventEmitterMiddlewareProvider<SystemInitialize_t> eventEmitter(nullptr, "bst_system_init", 8, nullptr, nullptr);
    fromZMQProvider->registerToTopic(topic, eventEmitter.getPublisher());

    kpsr::mem::TestCacheListener<SystemInitialize_t> cacheListener(0);

    eventEmitter.getSubscriber()->registerListener("test", cacheListener.cacheListenerFunction);

    fromZMQProvider->start();
    writeService.startup();

    writeService.runOnce();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    for (int i = 0; i < 100; i ++) {
        writeService.runOnce();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (cacheListener.counter > 0) {
            ASSERT_EQ(writeService.event.vehicle_type, cacheListener.getLastReceivedEvent()->vehicle_type);
        }
    }
    writeService.shutdown();
    fromZMQProvider->stop();

    ASSERT_GE(cacheListener.counter, 1);
}
