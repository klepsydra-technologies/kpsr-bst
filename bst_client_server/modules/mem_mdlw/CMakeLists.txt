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

SET(PROJ_NAME kpsr_mem_${PROJ_MAIN_NAME})
PROJECT(${PROJ_NAME})

# Include directories with headers
#---------------------------------------------------#
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/${PROJ_MAIN_NAME}/modules/${PROJ_MAIN_NAME}/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/modules/bst_comms/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/modules/mem_mdlw/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/gen/poco/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/trajectory_monitoring/modules/trajectory_monitoring/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/trajectory_monitoring/modules/mem_mdlw/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/trajectory_monitoring/gen/poco/include )
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
ADD_EXECUTABLE(${PROJ_NAME}_el src/bst_mem_el_client_server.cpp )
ADD_EXECUTABLE(${PROJ_NAME}_sf src/bst_mem_sf_client_server.cpp )

# Link libraries with Project
#---------------------------------------------------#
if(KPSR_WITH_ADMIN)
  list(APPEND KPSR_ALL_ADMIN_LIBRARIES "kpsr_trajectory_monitoring" "kpsr_mem_trajectory_monitoring" ${KPSR_ADMIN_CORE_LIBRARIES} ${KPSR_ADMIN_REST_LIBRARIES} "pistache")
endif()

TARGET_LINK_LIBRARIES(${PROJ_NAME}_el
    kpsr_${PROJ_MAIN_NAME} 
    kpsr_bst_comms
    kpsr_mem_bst_comms
    ${KLEPSYDRA_CORE_LIBRARIES}
    ${KPSR_ALL_ADMIN_LIBRARIES}
    ${BST_LIBRARY_CORE} 
    ${BST_LIBRARY_NETUAS} 
    yaml-cpp 
    atomic 
    rt
    ${CMAKE_THREAD_LIBS_INIT})

TARGET_LINK_LIBRARIES(${PROJ_NAME}_sf
    kpsr_${PROJ_MAIN_NAME} 
    kpsr_bst_comms
    kpsr_mem_bst_comms
    ${KLEPSYDRA_CORE_LIBRARIES}
    ${KPSR_ALL_ADMIN_LIBRARIES}
    ${BST_LIBRARY_CORE} 
    ${BST_LIBRARY_NETUAS} 
    yaml-cpp 
    atomic 
    rt
    ${CMAKE_THREAD_LIBS_INIT})

# Install
#---------------------------------------------------#
INSTALL(TARGETS ${PROJ_NAME}_el
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
INSTALL(TARGETS ${PROJ_NAME}_sf
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

add_core_export_target(${PROJ_NAME}_el)
add_core_export_target(${PROJ_NAME}_sf)
#PRINTBASICINFO(${PROJ_NAME})

