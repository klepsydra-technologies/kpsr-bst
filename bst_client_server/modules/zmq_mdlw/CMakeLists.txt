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

set(PROJ_NAME kpsr_zmq_${PROJ_MAIN_NAME})
project(${PROJ_NAME})

# find cppzmq wrapper, installed by make of cppzmq
find_package(cppzmq REQUIRED)

# Source
# ---------------------------------------------------#
file(GLOB ${PROJ_NAME}_SRC "src/*.cpp")

file(GLOB_RECURSE ${PROJ_NAME}_HEADERS "include/*.h")

# Create Library
# ---------------------------------------------------#
add_executable(kpsr_zmq_bst_client src/bst_zmq_client.cpp)
add_executable(kpsr_zmq_bst_server src/bst_zmq_server.cpp)
add_executable(kpsr_zmq_bst_proxy src/bst_kpsr_zmq_proxy_server.cpp)

target_include_directories(kpsr_zmq_bst_client
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(kpsr_zmq_bst_server
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(kpsr_zmq_bst_proxy
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Link libraries with Project
# ---------------------------------------------------#
target_link_libraries(
    kpsr_zmq_bst_client
    PRIVATE kpsr_${PROJ_MAIN_NAME} kpsr_zmq_bst_comms
            ${KLEPSYDRA_ZMQ_LIBRARIES} ${KPSR_ROBOTICS_ZMQ_LIBRARIES})

target_link_libraries(
    kpsr_zmq_bst_server
    PRIVATE kpsr_${PROJ_MAIN_NAME} kpsr_zmq_bst_comms
            ${KLEPSYDRA_ZMQ_LIBRARIES} ${KPSR_ROBOTICS_ZMQ_LIBRARIES})

target_link_libraries(
    kpsr_zmq_bst_proxy
    PRIVATE czmq kpsr_${PROJ_MAIN_NAME} kpsr_zmq_bst_comms
            ${KLEPSYDRA_ZMQ_LIBRARIES} ${KPSR_ROBOTICS_ZMQ_LIBRARIES})

# Install
# ---------------------------------------------------#
install(
    TARGETS kpsr_zmq_bst_client
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT libs)
install(
    TARGETS kpsr_zmq_bst_server
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT libs)
install(
    TARGETS kpsr_zmq_bst_proxy
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT libs)

add_zmq_export_target(kpsr_zmq_bst_client)
add_zmq_export_target(kpsr_zmq_bst_server)
add_zmq_export_target(kpsr_zmq_bst_proxy)
# PRINTBASICINFO(${PROJ_NAME})
