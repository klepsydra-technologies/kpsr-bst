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
    COMPONENT libs
)
INSTALL(TARGETS ${PROJ_NAME}_sf
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT libs
)

add_core_export_target(${PROJ_NAME}_el)
add_core_export_target(${PROJ_NAME}_sf)
#PRINTBASICINFO(${PROJ_NAME})

