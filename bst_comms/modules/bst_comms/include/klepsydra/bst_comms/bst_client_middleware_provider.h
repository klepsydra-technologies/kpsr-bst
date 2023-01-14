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

#ifndef BST_CLIENT_MIDDLEWARE_PROVIDER_H
#define BST_CLIENT_MIDDLEWARE_PROVIDER_H

/* BST */
#include "bst_module_basic.h"
#include "comm_packets.h"
#include "gcs.h"
#include "multicopter.h"

#include <klepsydra/core/publisher.h>
#include <klepsydra/core/subscriber.h>
#include <klepsydra/core/scheduler.h>

#include <klepsydra/geometry/pose_event_data.h>

#include <klepsydra/bst_comms/bst_request_message.h>
#include <klepsydra/bst_comms/bst_reply_message.h>
#include <klepsydra/bst_comms/waypoint_command_message.h>

namespace kpsr
{
namespace bst
{
/**
 * @brief The BstClientMiddlewareProvider class
 *
 * @copyright 2023 Klepsydra Technologies AG
 *
 * @version 2.0.1
 *
 * @ingroup kpsr-bst-public
 *
 * @details Client interface to all BST provided messages subscribers and request publishers. The content of the
 * waypoint command message are:
@code
namespace kpsr {
namespace bst {
// Klepsydra generated event class.
class WaypointCommandMessage {
public:
   // Default constructor.
   WaypointCommandMessage() {}

   // Main constructor.
   WaypointCommandMessage(
      unsigned char id,
      std::vector<std::shared_ptr<WaypointMessage>> plan,
      FPMapMode_t mode)
      : id(id)
      , plan(plan)
      , mode(mode)
   {}

   // Clone constructor. Needed by klepsydra core APIs.
   WaypointCommandMessage(const WaypointCommandMessage & that)
      : id(that.id)
      , plan(that.plan)
      , mode(that.mode)
   {}

   // Clone method. Needed by klepsydra core APIs.
   void clone(const WaypointCommandMessage & that) {
      this->id = that.id;
      this->plan = that.plan;
      this->mode = that.mode;
   }

   // List of fields.
   unsigned char id;
   std::vector<std::shared_ptr<WaypointMessage>> plan;
   FPMapMode_t mode;
};
}
}
@endcode
 * and:
@code
namespace kpsr {
namespace bst {
// Klepsydra generated event class.
class WaypointMessage {
public:
   // Default constructor.
   WaypointMessage() {}

   // Main constructor.
   WaypointMessage(
      unsigned char id,
      unsigned char num,
      unsigned char next,
      float latitude,
      float longitude,
      float altitude,
      float radius,
      unsigned char action)
      : id(id)
      , num(num)
      , next(next)
      , latitude(latitude)
      , longitude(longitude)
      , altitude(altitude)
      , radius(radius)
      , action(action)
   {}

   // Clone constructor. Needed by klepsydra core APIs.
   WaypointMessage(const WaypointMessage & that)
      : id(that.id)
      , num(that.num)
      , next(that.next)
      , latitude(that.latitude)
      , longitude(that.longitude)
      , altitude(that.altitude)
      , radius(that.radius)
      , action(that.action)
   {}

   // Clone method. Needed by klepsydra core APIs.
   void clone(const WaypointMessage & that) {
      this->id = that.id;
      this->num = that.num;
      this->next = that.next;
      this->latitude = that.latitude;
      this->longitude = that.longitude;
      this->altitude = that.altitude;
      this->radius = that.radius;
      this->action = that.action;
   }

   // List of fields.
   unsigned char id;
   unsigned char num;
   unsigned char next;
   float latitude;
   float longitude;
   float altitude;
   float radius;
   unsigned char action;
};
}
}
@endcode
 */
class BstClientMiddlewareProvider {
public:

    /**
     * @brief start underlying middleware queues
     */
    virtual void start() = 0;

    /**
     * @brief stop underlying middleware queues
     */
    virtual void stop() = 0;

    /**
     * @brief BST Command request publisher used for sending commands to BST Autopilot.
     * @return
     */
    virtual Publisher<BstRequestMessage> * getBstRequestMessagePublisher() = 0;

    /**
     * @brief Klepsydra reply queue with the result of the request command.
     * @return
     */
    virtual Subscriber<BstReplyMessage> * getBst2KpsrReplyMessageSubscriber() = 0;

    /**
     * @brief BST Command request publisher for sending waypoints commmands
     * @return
     */
    virtual Publisher<WaypointCommandMessage> * getBstWaypointCommandMessagePublisher() = 0;

    /**
     * @brief Klepsydra geometry telemetry message subscriber.
     * @return
     */
    virtual Subscriber<geometry::PoseEventData> * getPoseEventDataSubscriber() = 0;

    /**
     * @brief BST SystemInitialize_t subscriber
     * @return
     */
    virtual Subscriber<SystemInitialize_t> * getSystemInitializeSubscriber() = 0;

    /**
     * @brief BST TelemetryPosition_t subscriber
     * @return
     */
    virtual Subscriber<TelemetryPosition_t> * getTelemetryPositionSubscriber() = 0;

    /**
     * @brief BST TelemetryOrientation_t subscriber
     * @return
     */
    virtual Subscriber<TelemetryOrientation_t> * getTelemetryOrientationSubscriber() = 0;

    /**
     * @brief getSensorSubscriber
     * @return
     */
    virtual Subscriber<Sensors_t> * getSensorSubscriber() = 0;

    /**
     * @brief BST CalibrateSensor_t subscriber
     * @return
     */
    virtual Subscriber<CalibrateSensor_t> * getCalibrateSubscriber() = 0;

    /**
     * @brief BST Command_t subscriber
     * @return
     */
    virtual Subscriber<Command_t> * getControlCommandSubscriber() = 0;

    /**
     * @brief BST PID_t subscriber
     * @return
     */
    virtual Subscriber<PID_t> * getControlPidSubscriber() = 0;

    /**
     * @brief BST uint8_t subscriber
     * @return
     */
    virtual Subscriber<uint8_t> * getSystemSubscriber() = 0;

    /**
     * @brief BST TelemetrySystem_t subscriber
     * @return
     */
    virtual Subscriber<TelemetrySystem_t> * getTelemetrySystemSubscriber() = 0;

    /**
     * @brief BST TelemetryPressure_t subscriber
     * @return
     */
    virtual Subscriber<TelemetryPressure_t> * getTelemetryPressureSubscriber() = 0;

    /**
     * @brief BST ::bst::comms::TelemetryControl_t subscriber
     * @return
     */
    virtual Subscriber<::bst::comms::TelemetryControl_t> * getTelemetryControlSubscriber() = 0;

    /**
     * @brief BST gcs::TelemetryGCS_t subscriber
     * @return
     */
    virtual Subscriber<gcs::TelemetryGCS_t> * getTelemetryGCSSubscriber() = 0;

    /**
     * @brief BST PayloadControl_t subscriber
     * @return
     */
    virtual Subscriber<PayloadControl_t> * getPayloadControlSubscriber() = 0;

    /**
     * @brief getScheduler
     * @return
     */
    virtual Scheduler * getScheduler() = 0;
};
}
}

#endif // BST_CLIENT_MIDDLEWARE_PROVIDER_H

