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

#include <klepsydra/bst_client_server/client_state_machine_conf.h>

kpsr::bst::ClientStateMachineConfiguration::ClientStateMachineConfiguration()
{
    stateMachineConf.id = "bstClientStateMachine";
    stateMachineConf.states.resize(20);

    stateMachineConf.states[0].id = "idle";
    stateMachineConf.states[0].transitions.resize(1);
    stateMachineConf.states[0].transitions[0].event = "telemetrySystemRx";
    stateMachineConf.states[0].transitions[0].destinationId = "ready";

    stateMachineConf.states[1].id = "ready";
    stateMachineConf.states[1].transitions.resize(1);
    stateMachineConf.states[1].transitions[0].event = "payloadControlReqRx";
    stateMachineConf.states[1].transitions[0].destinationId = "payloadControlReadyReq";

    stateMachineConf.states[2].id = "payloadControlReadyReq";
    stateMachineConf.states[2].transitions.resize(2);
    stateMachineConf.states[2].transitions[0].event = "payloadControlReadyAckRx";
    stateMachineConf.states[2].transitions[0].destinationId = "payloadControlReqAccepted";
    stateMachineConf.states[2].transitions[1].event = "payloadControlReadyNackRx";
    stateMachineConf.states[2].transitions[1].destinationId = "error";

    stateMachineConf.states[3].id = "payloadControlReqAccepted";
    stateMachineConf.states[3].transitions.resize(2);
    stateMachineConf.states[3].transitions[0].event = "validFlightModeRx";
    stateMachineConf.states[3].transitions[0].destinationId = "payloadControlReady";
    stateMachineConf.states[3].transitions[1].event = "notValidFlightModeRx";
    stateMachineConf.states[3].transitions[1].destinationId = "error";

    stateMachineConf.states[4].id = "payloadControlReady";
    stateMachineConf.states[4].transitions.resize(2);
    stateMachineConf.states[4].transitions[0].event = "takeoffReqRx";
    stateMachineConf.states[4].transitions[0].destinationId = "preFlightReq";
    stateMachineConf.states[4].transitions[1].event = "payloadControlOffReqRx";
    stateMachineConf.states[4].transitions[1].destinationId = "payloadControlOffReq";

    stateMachineConf.states[5].id = "preFlightReq";
    stateMachineConf.states[5].transitions.resize(3);
    stateMachineConf.states[5].transitions[0].event = "preFlightReqAckRx";
    stateMachineConf.states[5].transitions[0].destinationId = "preFlightReqAccepted";
    stateMachineConf.states[5].transitions[1].event = "preFlightReqNackRx";
    stateMachineConf.states[5].transitions[1].destinationId = "error";
    stateMachineConf.states[5].transitions[2].event = "alreadyPreFlight";
    stateMachineConf.states[5].transitions[2].destinationId = "launchModeReq";

    stateMachineConf.states[6].id = "preFlightReqAccepted";
    stateMachineConf.states[6].transitions.resize(2);
    stateMachineConf.states[6].transitions[0].event = "preFlightModeRx";
    stateMachineConf.states[6].transitions[0].destinationId = "launchModeReq";
    stateMachineConf.states[6].transitions[1].event = "notPreFlightModeRx";
    stateMachineConf.states[6].transitions[1].destinationId = "error";

    stateMachineConf.states[7].id = "launchModeReq";
    stateMachineConf.states[7].transitions.resize(3);
    stateMachineConf.states[7].transitions[0].event = "launchModeReqAckRx";
    stateMachineConf.states[7].transitions[0].destinationId = "launchModeReqAccepted";
    stateMachineConf.states[7].transitions[1].event = "launchModeReqNackRx";
    stateMachineConf.states[7].transitions[1].destinationId = "error";
    stateMachineConf.states[7].transitions[2].event = "alreadyLaunchMode";
    stateMachineConf.states[7].transitions[2].destinationId = "enableEngineReq";

    stateMachineConf.states[8].id = "launchModeReqAccepted";
    stateMachineConf.states[8].transitions.resize(2);
    stateMachineConf.states[8].transitions[0].event = "launchModeRx";
    stateMachineConf.states[8].transitions[0].destinationId = "enableEngineReq";
    stateMachineConf.states[6].transitions[1].event = "notLaunchModeRx";
    stateMachineConf.states[6].transitions[1].destinationId = "error";

    stateMachineConf.states[9].id = "enableEngineReq";
    stateMachineConf.states[9].transitions.resize(2);
    stateMachineConf.states[9].transitions[0].event = "enableEngineReqAckRx";
    stateMachineConf.states[9].transitions[0].destinationId = "launchReq";
    stateMachineConf.states[9].transitions[1].event = "enableEngineReqNackRx";
    stateMachineConf.states[9].transitions[1].destinationId = "error";

    stateMachineConf.states[10].id = "launchReq";
    stateMachineConf.states[10].transitions.resize(2);
    stateMachineConf.states[10].transitions[0].event = "launchReqAckRx";
    stateMachineConf.states[10].transitions[0].destinationId = "launchReqAccepted";
    stateMachineConf.states[10].transitions[1].event = "launchReqNackRx";
    stateMachineConf.states[10].transitions[1].destinationId = "launchReqAccepted";

    stateMachineConf.states[11].id = "launchReqAccepted";
    stateMachineConf.states[11].transitions.resize(2);
    stateMachineConf.states[11].transitions[0].event = "flyingModeRx";
    stateMachineConf.states[11].transitions[0].destinationId = "flying";
    stateMachineConf.states[11].transitions[1].event = "notFlyingModeRx";
    stateMachineConf.states[11].transitions[1].destinationId = "error";

    stateMachineConf.states[12].id = "flying";
    stateMachineConf.states[12].transitions.resize(3);
    stateMachineConf.states[12].transitions[0].event = "landReqRx";
    stateMachineConf.states[12].transitions[0].destinationId = "landReq";
    stateMachineConf.states[12].transitions[1].event = "controlCommandRx";
    stateMachineConf.states[12].transitions[1].destinationId = "controlCommandReq";
    stateMachineConf.states[12].transitions[2].event = "flightPlanRx";
    stateMachineConf.states[12].transitions[2].destinationId = "flightPlanReq";

    stateMachineConf.states[13].id = "controlCommandReq";
    stateMachineConf.states[13].transitions.resize(2);
    stateMachineConf.states[13].transitions[0].event = "controlCommandReqAckRx";
    stateMachineConf.states[13].transitions[0].destinationId = "flying";
    stateMachineConf.states[13].transitions[1].event = "controlCommandReqNackRx";
    stateMachineConf.states[13].transitions[1].destinationId = "error";

    stateMachineConf.states[14].id = "flightPlanReq";
    stateMachineConf.states[14].transitions.resize(2);
    stateMachineConf.states[14].transitions[0].event = "flightPlanReqAckRx";
    stateMachineConf.states[14].transitions[0].destinationId = "flying";
    stateMachineConf.states[14].transitions[1].event = "flightPlanReqNackRx";
    stateMachineConf.states[14].transitions[1].destinationId = "error";

    stateMachineConf.states[15].id = "landReq";
    stateMachineConf.states[15].transitions.resize(2);
    stateMachineConf.states[15].transitions[0].event = "landReqAckRx";
    stateMachineConf.states[15].transitions[0].destinationId = "landReqAccepted";
    stateMachineConf.states[15].transitions[1].event = "landReqNackRx";
    stateMachineConf.states[15].transitions[1].destinationId = "landReqAccepted";

    stateMachineConf.states[16].id = "landReqAccepted";
    stateMachineConf.states[16].transitions.resize(2);
    stateMachineConf.states[16].transitions[0].event = "landingModeRx";
    stateMachineConf.states[16].transitions[0].destinationId = "landed";
    stateMachineConf.states[16].transitions[1].event = "notLandingModeRx";
    stateMachineConf.states[16].transitions[1].destinationId = "error";

    stateMachineConf.states[17].id = "landed";
    stateMachineConf.states[17].transitions.resize(1);
    stateMachineConf.states[17].transitions[0].event = "payloadControlOffReqRx";
    stateMachineConf.states[17].transitions[0].destinationId = "payloadControlOffReq";

    stateMachineConf.states[18].id = "payloadControlOffReq";
    stateMachineConf.states[18].transitions.resize(2);
    stateMachineConf.states[18].transitions[0].event = "payloadControlOffAckRx";
    stateMachineConf.states[18].transitions[0].destinationId = "ready";
    stateMachineConf.states[18].transitions[1].event = "payloadControlOffNackRx";
    stateMachineConf.states[18].transitions[1].destinationId = "error";

    stateMachineConf.states[19].id = "error";
    stateMachineConf.states[19].transitions.resize(0);
}
