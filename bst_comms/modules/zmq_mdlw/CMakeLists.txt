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
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/${PROJ_MAIN_NAME}/gen/poco/include )
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/${PROJ_MAIN_NAME}/modules/${PROJ_MAIN_NAME}/include )
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
FILE(GLOB_RECURSE ${PROJ_NAME}_GEN_HEADERS
    "${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/codegen/gen/serialization/include/*/h"
)

# Create Library
#---------------------------------------------------#

ADD_LIBRARY(${PROJ_NAME} ${${PROJ_MAIN_NAME}_LIB_TYPE}  ${${PROJ_NAME}_SRC} )

# Link libraries with Project
#---------------------------------------------------#
TARGET_LINK_LIBRARIES(${PROJ_NAME} 
    ${KLEPSYDRA_CORE_LIBRARIES}
    kpsr_${PROJ_MAIN_NAME} 
    cppzmq 
    atomic 
    ${CMAKE_THREAD_LIBS_INIT})

# Install
#---------------------------------------------------#
INSTALL(TARGETS ${PROJ_NAME}
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT libs
)
INSTALL(DIRECTORY "include/klepsydra"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT dev
)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJ_MAIN_NAME}/gen/serialization/include/klepsydra
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT dev
)

add_zmq_export_target(${PROJ_NAME})
#PRINTBASICINFO(${PROJ_NAME})

