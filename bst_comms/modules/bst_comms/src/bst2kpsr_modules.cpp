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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <klepsydra/bst_comms/bst2kpsr_modules.h>
#include <klepsydra/bst_comms/bst2kpsr_middleware_provider.h>

void kpsr::bst::Bst2KpsrModules::BasicModule::receive(uint8_t type, void * data, uint16_t size, const void * parameter)
{
    spdlog::debug("{}receive: type={}", __PRETTY_FUNCTION__, type);

    unsigned char *charBuf = (unsigned char*)data;
    /* create a vector by copying out the contents of charBuf */
    std::vector<unsigned char> dataVector(charBuf, charBuf + size);

    kpsr::bst::Bst2KpsrInternalMessageBuilder builder;
    std::shared_ptr<Bst2KpsrInternalMessage> message = builder.withCommsModule(kpsr::bst::COMMS_MODULE::BASIC)
            .withCommsFunction(kpsr::bst::COMMS_FUNCTION::RECEIVE)
            .withType(type)
            .withData(dataVector)
            .withSize(size)
            .withReceivedParameter(parameter).build();

    kpsr::bst::Bst2KpsrMiddlewareProvider::getBst2KpsrInternalMessagePublisher()->publish(message);
    spdlog::debug("{}receive after publish.", __PRETTY_FUNCTION__);
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
    spdlog::debug("{}receiveCommand after publish.", __PRETTY_FUNCTION__);

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
    spdlog::debug("{}receiveReply after publish.", __PRETTY_FUNCTION__);
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
    spdlog::debug("{}publish after publish.", __PRETTY_FUNCTION__);

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

SystemInitialize_t system_initialize;

double start_time = 0.0;

#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef __cplusplus
}
#endif


