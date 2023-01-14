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


SET(PROJ_NAME kpsr_zmq_${PROJ_MAIN_NAME})
PROJECT(${PROJ_NAME})

#find cppzmq wrapper, installed by make of cppzmq
find_package(cppzmq REQUIRED)

# Include directories with headers
#---------------------------------------------------#
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/${PROJ_MAIN_NAME}/modules/${PROJ_MAIN_NAME}/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/modules/bst_comms/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/modules/zmq_mdlw/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/gen/poco/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/gen/serialization/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/trajectory_monitoring/modules/trajectory_monitoring/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/trajectory_monitoring/modules/mem_mdlw/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/trajectory_monitoring/gen/poco/include )
INCLUDE_DIRECTORIES( ${cppzmq_INCLUDE_DIR})
INCLUDE_DIRECTORIES( ${KLEPSYDRA_INCLUDE_DIRS} )

# Source
#---------------------------------------------------#
FILE(GLOB ${PROJ_NAME}_SRC
    "src/*.cpp"
)

FILE(GLOB_RECURSE ${PROJ_NAME}_HEADERS
    "include/*.h"
)

# Create Library
#---------------------------------------------------#
ADD_EXECUTABLE(kpsr_zmq_bst_client src/bst_zmq_client.cpp  )
ADD_EXECUTABLE(kpsr_zmq_bst_server src/bst_zmq_server.cpp  )
ADD_EXECUTABLE(kpsr_zmq_bst_proxy src/bst_kpsr_zmq_proxy_server.cpp )

# Link libraries with Project
#---------------------------------------------------#
if(KPSR_WITH_ADMIN)
  list(APPEND KPSR_ALL_ADMIN_LIBRARIES ${KPSR_ADMIN_CORE_LIBRARIES} ${KPSR_ADMIN_REST_LIBRARIES} "pistache")
endif()

TARGET_LINK_LIBRARIES(kpsr_zmq_bst_client 
    kpsr_${PROJ_MAIN_NAME} 
    kpsr_bst_comms
    kpsr_zmq_bst_comms
    ${KLEPSYDRA_CORE_LIBRARIES}
    ${KLEPSYDRA_ZMQ_LIBRARIES}
    ${KPSR_ALL_ADMIN_LIBRARIES}
    ${BST_LIBRARY_CORE} 
    ${BST_LIBRARY_NETUAS} 
    cppzmq 
    yaml-cpp 
    atomic 
    rt
    ${CMAKE_THREAD_LIBS_INIT})

TARGET_LINK_LIBRARIES(kpsr_zmq_bst_server
    kpsr_${PROJ_MAIN_NAME} 
    kpsr_bst_comms
    kpsr_zmq_bst_comms
    ${KLEPSYDRA_CORE_LIBRARIES}
    ${KLEPSYDRA_ZMQ_LIBRARIES}
    ${KPSR_ALL_ADMIN_LIBRARIES}
    ${BST_LIBRARY_CORE} 
    ${BST_LIBRARY_NETUAS} 
    cppzmq 
    yaml-cpp 
    atomic 
    rt
    ${CMAKE_THREAD_LIBS_INIT})

TARGET_LINK_LIBRARIES(kpsr_zmq_bst_proxy 
    kpsr_${PROJ_MAIN_NAME} 
    ${KLEPSYDRA_CORE_LIBRARIES}
    cppzmq 
    yaml-cpp 
    czmq
    ${CMAKE_THREAD_LIBS_INIT})

# Install
#---------------------------------------------------#
INSTALL(TARGETS kpsr_zmq_bst_client
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    libs
)
INSTALL(TARGETS kpsr_zmq_bst_server
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT libs
)
INSTALL(TARGETS kpsr_zmq_bst_proxy
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT libs
)

add_zmq_export_target(kpsr_zmq_bst_client)
add_zmq_export_target(kpsr_zmq_bst_server)
add_zmq_export_target(kpsr_zmq_bst_proxy)
#PRINTBASICINFO(${PROJ_NAME})

