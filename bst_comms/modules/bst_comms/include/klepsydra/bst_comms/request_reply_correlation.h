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
#ifndef REQUEST_REPLY_CORRELATION_H
#define REQUEST_REPLY_CORRELATION_H

#include <functional>

#include <klepsydra/bst_comms/bst_request_message.h>
#include <klepsydra/bst_comms/bst_reply_message.h>

namespace kpsr
{
namespace bst
{
/**
 * @brief The RequestReplyCorrelation class
 *
 * @copyright Klepsydra Robotics 2017-2018.
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Request reply correlation facility.
 */
class RequestReplyCorrelation {
public:
    /**
     * @brief RequestReplyCorrelation
     */
    RequestReplyCorrelation()
        : correlationFunction(std::bind(&RequestReplyCorrelation::match, this, std::placeholders::_1, std::placeholders::_2))
    {}

    /**
     * @brief match
     * @param request
     * @param reply
     * @return
     */
    bool match(const BstRequestMessage & request, const BstReplyMessage & reply) {
        return ((request.type == reply.type) && (request.id == reply.id));
    }

    /**
     * @brief correlationFunction
     */
    std::function<bool(const BstRequestMessage &, const BstReplyMessage &)> correlationFunction;

};
}
}

#endif // REQUEST_REPLY_CORRELATION_H
