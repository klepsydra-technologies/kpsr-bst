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

/* BST */
#include "helper_functions.h"
#include "comm_packets.h"
#include "comm_interface.h"
#include "comm_protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <spdlog/spdlog.h>

#include <klepsydra/bst_comms/bst2kpsr_modules.h>
#include <klepsydra/bst_comms/bst2kpsr_middleware_provider.h>

void kpsr::bst::Bst2KpsrModules::BasicModule::receive(uint8_t type, void * data, uint16_t size, const void * parameter)
{
    spdlog::debug("{}receive: type={} size={}", __PRETTY_FUNCTION__, type, size);

    unsigned char *charBuf = (unsigned char*)data;
    /* create a vector by copying out the contents of charBuf */
    std::vector<unsigned char> dataVector(charBuf, charBuf + size);
    /*
    for (int i = 0; i < size; i ++) {
        spdlog::debug("{}. data[{}] = {} / {}", __PRETTY_FUNCTION__, i, (int) charBuf[i], (int) dataVector[i]);
    }*/

    kpsr::bst::Bst2KpsrInternalMessageBuilder builder;
    std::shared_ptr<Bst2KpsrInternalMessage> message = builder.withCommsModule(kpsr::bst::COMMS_MODULE::BASIC)
            .withCommsFunction(kpsr::bst::COMMS_FUNCTION::RECEIVE)
            .withType(type)
            .withData(dataVector)
            .withSize(size)
            .withReceivedParameter(parameter).build();

    kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()->publish(message);
}

uint8_t kpsr::bst::Bst2KpsrModules::BasicModule::receiveCommand(uint8_t type, void * data, uint16_t size, const void * parameter)
{
    spdlog::debug("{}receiveCommand: type={}", __PRETTY_FUNCTION__, type);

    unsigned char *charBuf = (unsigned char*)data;
    /* create a vector by copying out the contents of charBuf */
    std::vector<unsigned char> dataVector(charBuf, charBuf + size);

    kpsr::bst::Bst2KpsrInternalMessageBuilder builder;
    std::shared_ptr<Bst2KpsrInternalMessage> message = builder.withCommsModule(kpsr::bst::COMMS_MODULE::BASIC)
            .withCommsFunction(kpsr::bst::COMMS_FUNCTION::RECEIVE_COMMAND)
            .withType(type)
            .withData(dataVector)
            .withSize(size)
            .withReceivedParameter(parameter).build();

    kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()->publish(message);

    //FIXME: return real value
    return true;
}

void kpsr::bst::Bst2KpsrModules::BasicModule::receiveReply(uint8_t type, void * data, uint16_t size, bool ack, const void * parameter)
{
    spdlog::debug("{}receiveReply: type={}, data={}, ack: {}", __PRETTY_FUNCTION__, (int) type, (int) (* (uint8_t *) data), (ack ? "ACK" : "NACK"));

    unsigned char *charBuf = (unsigned char*)data;
    /* create a vector by copying out the contents of charBuf */
    std::vector<unsigned char> dataVector(charBuf, charBuf + size);

    kpsr::bst::Bst2KpsrInternalMessageBuilder builder;
    std::shared_ptr<Bst2KpsrInternalMessage> message = builder.withCommsModule(kpsr::bst::COMMS_MODULE::BASIC)
            .withCommsFunction(kpsr::bst::COMMS_FUNCTION::RECEIVE_REPLY)
            .withType(type)
            .withData(dataVector)
            .withSize(size)
            .withAck(ack)
            .withReceivedParameter(parameter).build();

    kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()->publish(message);
}

bool kpsr::bst::Bst2KpsrModules::BasicModule::publish(uint8_t type, uint8_t param)
{
    spdlog::debug("{}publish: type={}", __PRETTY_FUNCTION__, type);
    kpsr::bst::Bst2KpsrInternalMessageBuilder builder;
    std::shared_ptr<Bst2KpsrInternalMessage> message = builder.withCommsModule(kpsr::bst::COMMS_MODULE::BASIC)
            .withCommsFunction(kpsr::bst::COMMS_FUNCTION::PUBLISH)
            .withType(type)
            .withPublishParam(param).build();

    kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()->publish(message);

    //FIXME: return real value
    return false;
}

void kpsr::bst::Bst2KpsrModules::FlightPlan::receive(uint8_t type, void * data, uint16_t size, const void * parameter)
{
    spdlog::debug("{}receive: type={}", __PRETTY_FUNCTION__, type);

    unsigned char *charBuf = (unsigned char*)data;
    /* create a vector by copying out the contents of charBuf */
    std::vector<unsigned char> dataVector(charBuf, charBuf + size);

    kpsr::bst::Bst2KpsrInternalMessageBuilder builder;
    std::shared_ptr<Bst2KpsrInternalMessage> message = builder.withCommsModule(kpsr::bst::COMMS_MODULE::FLIGHT_PLAN)
            .withCommsFunction(kpsr::bst::COMMS_FUNCTION::RECEIVE)
            .withType(type)
            .withData(dataVector)
            .withSize(size)
            .withReceivedParameter(parameter).build();

    kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()->publish(message);
}

uint8_t kpsr::bst::Bst2KpsrModules::FlightPlan::receiveCommand(uint8_t type, void * data, uint16_t size, const void * parameter)
{
    spdlog::debug("{}receiveReply: type={}, data={}", __PRETTY_FUNCTION__, type, * (uint8_t *) data);

    unsigned char *charBuf = (unsigned char*)data;
    /* create a vector by copying out the contents of charBuf */
    std::vector<unsigned char> dataVector(charBuf, charBuf + size);

    kpsr::bst::Bst2KpsrInternalMessageBuilder builder;
    std::shared_ptr<Bst2KpsrInternalMessage> message = builder.withCommsModule(kpsr::bst::COMMS_MODULE::FLIGHT_PLAN)
            .withCommsFunction(kpsr::bst::COMMS_FUNCTION::RECEIVE_COMMAND)
            .withType(type)
            .withData(dataVector)
            .withSize(size)
            .withReceivedParameter(parameter).build();

    kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()->publish(message);
    //FIXME: return real value
    return true;
}

void kpsr::bst::Bst2KpsrModules::FlightPlan::receiveReply(uint8_t type, void * data, uint16_t size, bool ack, const void * parameter)
{
    spdlog::debug("{}: type={}, data={}, ack: {}", __PRETTY_FUNCTION__, (int) type, (int) (* (uint8_t *) data), (ack ? "ACK" : "NACK"));

    unsigned char *charBuf = (unsigned char*)data;
    /* create a vector by copying out the contents of charBuf */
    std::vector<unsigned char> dataVector(charBuf, charBuf + size);

    kpsr::bst::Bst2KpsrInternalMessageBuilder builder;
    std::shared_ptr<Bst2KpsrInternalMessage> message = builder.withCommsModule(kpsr::bst::COMMS_MODULE::FLIGHT_PLAN)
            .withCommsFunction(kpsr::bst::COMMS_FUNCTION::RECEIVE_REPLY)
            .withType(type)
            .withData(dataVector)
            .withSize(size)
            .withAck(ack)
            .withReceivedParameter(parameter).build();

    kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()->publish(message);
}

bool kpsr::bst::Bst2KpsrModules::FlightPlan::publish(uint8_t type, uint8_t param)
{
    spdlog::debug("{}publish: type={}", __PRETTY_FUNCTION__, type);
    kpsr::bst::Bst2KpsrInternalMessageBuilder builder;
    std::shared_ptr<Bst2KpsrInternalMessage> message = builder.withCommsModule(kpsr::bst::COMMS_MODULE::FLIGHT_PLAN)
            .withCommsFunction(kpsr::bst::COMMS_FUNCTION::PUBLISH)
            .withType(type)
            .withPublishParam(param).build();

    kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()->publish(message);

    //FIXME: return real value
    return false;
}


