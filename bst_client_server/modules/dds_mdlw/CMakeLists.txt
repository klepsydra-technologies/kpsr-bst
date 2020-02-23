#*************************************************************************
#
#                           Klepsydra Core Modules
#              Copyright (C) 2019-2020  Klepsydra Technologies GmbH
#                            All Rights Reserved.
#
#  This file is subject to the terms and conditions defined in
#  file 'LICENSE.md', which is part of this source code package.
#
#  NOTICE:  All information contained herein is, and remains the property of Klepsydra
#  Technologies GmbH and its suppliers, if any. The intellectual and technical concepts
#  contained herein are proprietary to Klepsydra Technologies GmbH and its suppliers and
#  may be covered by Swiss and Foreign Patents, patents in process, and are protected by
#  trade secret or copyright law. Dissemination of this information or reproduction of
#  this material is strictly forbidden unless prior written permission is obtained from
#  Klepsydra Technologies GmbH.
#
#*************************************************************************

SET(PROJ_NAME kpsr_dds_${PROJ_MAIN_NAME})
PROJECT(${PROJ_NAME})

include (MacroConnext)
find_package (Connext REQUIRED)
SET (DEFINITIONS ${DEFINITIONS} " -DDDS_IMPLEMENTATION_${DDS_IMPLEMENTATION}")

add_definitions (
  ${Connext_DEFINITIONS}
  ${DEFINITIONS}
)


# Include directories with headers
#---------------------------------------------------#
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/${PROJ_MAIN_NAME}/modules/${PROJ_MAIN_NAME}/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/modules/bst_comms/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/modules/mem_mdlw/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/gen/poco/include )
INCLUDE_DIRECTORIES( ${Connext_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${KLEPSYDRA_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( ${BST_INCLUDE_PATH} )
INCLUDE_DIRECTORIES( ${BST_INCLUDE_PATH}/netuas_lib )

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
LINK_DIRECTORIES( ${BST_LIBRARY_PATH})

ADD_EXECUTABLE(kpsr_dds_bst_client src/bst_dds_client.cpp  )
ADD_EXECUTABLE(kpsr_dds_bst_server src/bst_dds_server.cpp  )

# Link libraries with Project
#---------------------------------------------------#
TARGET_LINK_LIBRARIES(kpsr_dds_bst_client 
    kpsr_${PROJ_MAIN_NAME} 
    kpsr_bst_comms
    ${KLEPSYDRA_CORE_LIBRARIES}
    ${KLEPSYDRA_DDS_LIBRARIES}
    ${KPSR_ADMIN_CORE_LIBRARIES}
    ${KPSR_ADMIN_SOCKET_LIBRARIES}
    ${BST_LIBRARY_CORE} 
    ${BST_LIBRARY_NETUAS} 
    ${Connext_LIBRARIES} 
    yaml-cpp 
    pistache 
    atomic 
    rt
    ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(kpsr_dds_bst_server 
    kpsr_${PROJ_MAIN_NAME} 
    kpsr_bst_comms
    ${KLEPSYDRA_CORE_LIBRARIES}
    ${KLEPSYDRA_DDS_LIBRARIES}
    ${KPSR_ADMIN_CORE_LIBRARIES}
    ${KPSR_ADMIN_SOCKET_LIBRARIES}
    ${BST_LIBRARY_CORE} 
    ${BST_LIBRARY_NETUAS} 
    ${Connext_LIBRARIES} 
    yaml-cpp 
    pistache 
    atomic 
    rt
    ${CMAKE_THREAD_LIBS_INIT})


# Install
#---------------------------------------------------#
INSTALL(TARGETS kpsr_dds_bst_client
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
INSTALL(TARGETS kpsr_dds_bst_server
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

add_zmq_export_target(kpsr_dds_bst_client)
add_zmq_export_target(kpsr_dds_bst_server)
#PRINTBASICINFO(${PROJ_NAME})

