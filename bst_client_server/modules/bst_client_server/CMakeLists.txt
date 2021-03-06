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

SET(PROJ_NAME kpsr_${PROJ_MAIN_NAME})
PROJECT(${PROJ_NAME})

# Include directories with headers
#---------------------------------------------------#
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/modules/bst_comms/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/bst_comms/gen/poco/include )
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
ADD_LIBRARY(${PROJ_NAME} ${${PROJ_MAIN_NAME}_LIB_TYPE} ${${PROJ_NAME}_SRC} ${${PROJ_NAME}_HEADERS})

# Link libraries with Project
#---------------------------------------------------#
TARGET_LINK_LIBRARIES(${PROJ_NAME} 
    kpsr_bst_comms
    ${KLEPSYDRA_CORE_LIBRARIES}
    ${BST_LIBRARY_CORE} 
    ${BST_LIBRARY_NETUAS} 
    atomic 
    rt
    ${CMAKE_THREAD_LIBS_INIT})

# Install
#---------------------------------------------------#
INSTALL(TARGETS ${PROJ_NAME}
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
INSTALL(DIRECTORY "include/klepsydra"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

add_core_export_target(${PROJ_NAME})
#PRINTBASICINFO(${PROJ_NAME})

