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

#ifndef REQUEST_REPLY_CORRELATION_H
#define REQUEST_REPLY_CORRELATION_H

#include <functional>

#include <klepsydra/bst_comms/bst_reply_message.h>
#include <klepsydra/bst_comms/bst_request_message.h>

#include <klepsydra/bst_comms/waypoint_command_message.h>

namespace kpsr {
namespace bst {
/**
 * @brief The RequestReplyCorrelation class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Request reply correlation facility.
 */
class RequestReplyCorrelation
{
public:
    /**
     * @brief RequestReplyCorrelation
     */
    RequestReplyCorrelation()
        : correlationFunction(std::bind(&RequestReplyCorrelation::match,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2))
    {}

    /**
     * @brief match
     * @param request
     * @param reply
     * @return
     */
    bool match(const BstRequestMessage &request, const BstReplyMessage &reply)
    {
        return ((request.type == reply.type) && (request.id == reply.id));
    }

    /**
     * @brief correlationFunction
     */
    std::function<bool(const BstRequestMessage &, const BstReplyMessage &)> correlationFunction;
};

/**
 * @brief The RequestReplyCorrelation class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Request reply correlation facility.
 */
class FlightPlantReplyCorrelation
{
public:
    /**
     * @brief RequestReplyCorrelation
     */
    FlightPlantReplyCorrelation()
        : correlationFunction(std::bind(&FlightPlantReplyCorrelation::match,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2))
    {}

    /**
     * @brief match
     * @param request
     * @param reply
     * @return
     */
    bool match(const WaypointCommandMessage &request, const BstReplyMessage &reply)
    {
        return (request.id == reply.id);
    }

    /**
     * @brief correlationFunction
     */
    std::function<bool(const WaypointCommandMessage &, const BstReplyMessage &)> correlationFunction;
};
} // namespace bst
} // namespace kpsr

#endif // REQUEST_REPLY_CORRELATION_H
