# Copyright 2023 Klepsydra Technologies AG
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(PROJ_NAME kpsr_dds_${PROJ_MAIN_NAME})
project(${PROJ_NAME})

include(MacroConnext)

# Source
# ---------------------------------------------------#
file(GLOB ${PROJ_NAME}_SRC "src/*.cpp")

file(GLOB_RECURSE ${PROJ_NAME}_HEADERS "include/*.h")

# Create Library
# ---------------------------------------------------#
add_executable(kpsr_dds_bst_client src/bst_dds_client.cpp)
add_executable(kpsr_dds_bst_server src/bst_dds_server.cpp)

target_link_libraries(kpsr_dds_bst_client kpsr_${PROJ_MAIN_NAME} kpsr_bst_comms
                      kpsr_dds_bst_comms_datamodel kpsr_dds_geometry_datamodel)

target_link_libraries(kpsr_dds_bst_server kpsr_${PROJ_MAIN_NAME} kpsr_bst_comms
                      kpsr_dds_bst_comms_datamodel kpsr_dds_geometry_datamodel)

# Install
# ---------------------------------------------------#
install(
    TARGETS kpsr_dds_bst_client
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT libs)
install(
    TARGETS kpsr_dds_bst_server
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT libs)

add_zmq_export_target(kpsr_dds_bst_client)
add_zmq_export_target(kpsr_dds_bst_server)
# PRINTBASICINFO(${PROJ_NAME})
