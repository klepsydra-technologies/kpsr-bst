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
#ifndef BST2KPSR_MIDDLEWARE_PROVIDER_H
#define BST2KPSR_MIDDLEWARE_PROVIDER_H

#include <atomic>

#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>

#include <klepsydra/bst_comms/bst2_kpsr_internal_message.h>

namespace kpsr
{
namespace bst
{
namespace Bst2KpsrMiddlewareProvider {

void start(Container * container);
void stop();
kpsr::Subscriber<Bst2KpsrInternalMessage> * getBst2KpsrInternalMessageSubscriber();
kpsr::Publisher<Bst2KpsrInternalMessage> * getBst2KpsrInternalMessagePublisher();

}
}
}

#endif
