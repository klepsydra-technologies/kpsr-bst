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

#include <klepsydra/dds_serialization/primitive_type_dds_mapper.h>
#include <klepsydra/dds_serialization/enum_dds_mapper.h>

#include <klepsydra/core/service.h>
#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/core/event_emitter_middleware_provider.h>
#include <klepsydra/core/cache_listener.h>

#include <klepsydra/dds_core/dds_env.h>
#include <klepsydra/dds_core/from_dds_middleware_provider.h>
#include <klepsydra/dds_core/to_dds_middleware_provider.h>

#include <klepsydra/dds_bst_comms/bst_packet_data_dds_mapper.h>

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

TEST(DDSStgCoreTests, PubSubTest) {
    dds::domain::DomainParticipant dp(0);
    dds::pub::Publisher publisher(dp);
    dds::sub::Subscriber subscriber(dp);

    dds::topic::Topic<kpsr_dds_bst::BstPacketData> topic(dp, "bst_system_init");
    dds::pub::DataWriter<kpsr_dds_bst::BstPacketData> dw(publisher, topic);
    dds::sub::DataReader<kpsr_dds_bst::BstPacketData> dr(subscriber, topic);

    kpsr::dds_mdlw::ToDDSMiddlewareProvider provider(nullptr);

    //spdlog::info("Creating services.");
    SimpleWriteService writeService(nullptr, provider.getToMiddlewareChannel<bst::comms::SystemInitialize_t, kpsr_dds_bst::BstPacketData>("bst_system_init", 0, nullptr, &dw));

    kpsr::EventEmitterMiddlewareProvider<SystemInitialize_t> eventEmitter(nullptr, "bst_system_init", 8, nullptr, nullptr);
    kpsr::dds_mdlw::FromDDSMiddlewareProvider _fromDDSMiddlewareProvider;
    _fromDDSMiddlewareProvider.registerToTopic("bst_system_init", &dr, true, eventEmitter.getPublisher());

    kpsr::mem::MultiThreadCacheListener<SystemInitialize_t> cacheListener;

    eventEmitter.getSubscriber()->registerListener("test", cacheListener.cacheListenerFunction);

    writeService.startup();

    for (int i = 0; i < 100; i ++) {
        writeService.runOnce();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        ASSERT_EQ(writeService.event.vehicle_type, cacheListener.getLastReceivedEvent()->vehicle_type);
    }

    _fromDDSMiddlewareProvider.unregisterFromTopic("bst_system_init", &dr);
}
