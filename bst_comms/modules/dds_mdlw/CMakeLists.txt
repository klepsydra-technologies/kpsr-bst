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


SET(PROJ_NAME kpsr_dds_${PROJ_MAIN_NAME})
PROJECT(${PROJ_NAME})

include (MacroConnext)

file(REMOVE_RECURSE ${CMAKE_CURRENT_SOURCE_DIR}/gen)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/gen)

file(GLOB idls  "./idl/*.idl" "${CMAKE_SOURCE_DIR}/${PROJ_MAIN_NAME}/gen/dds/idl/*.idl")
FOREACH(idl ${idls})
        Connext_IDLGEN (${idl} ${CMAKE_CURRENT_SOURCE_DIR})
        GET_FILENAME_COMPONENT(nfile ${idl} NAME)
        Connext_GENDATAMODEL(${nfile} ${CMAKE_CURRENT_SOURCE_DIR})
ENDFOREACH(idl)

# Source
#---------------------------------------------------#
FILE(GLOB ${PROJ_NAME}_SRC
    "src/*.cpp"
)

FILE(GLOB_RECURSE ${PROJ_NAME}_HEADERS
    "include/*.h"
)
FILE(GLOB ${PROJ_NAME}_GEN_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/gen/*.h*"
)

FILE(GLOB_RECURSE ${PROJ_NAME}_KPSR_GEN_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/gen/dds/include/*.h"
)

# Create Library
#---------------------------------------------------#
ADD_LIBRARY (kpsr_dds_${PROJ_MAIN_NAME}_datamodel SHARED ${Connext_DATAMODEL})

target_include_directories(
    kpsr_dds_${PROJ_MAIN_NAME}_datamodel
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
           $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/${PROJ_MAIN_NAME}/gen/dds/include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/gen>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/klepsydra/gen_dds_${PROJ_MAIN_NAME}>)

# Link libraries with Project
#---------------------------------------------------#
# TARGET_LINK_LIBRARIES(${PROJ_NAME})
target_link_libraries(kpsr_dds_${PROJ_MAIN_NAME}_datamodel
                      PUBLIC RTIConnextDDS::cpp2_api kpsr_${PROJ_MAIN_NAME} ${KLEPSYDRA_DDS_LIBRARIES} )
# Install
#---------------------------------------------------#
INSTALL(TARGETS kpsr_dds_${PROJ_MAIN_NAME}_datamodel
    EXPORT KlepsydraBstTargets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT libs
)
INSTALL(DIRECTORY "include/klepsydra"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT dev
)
INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/gen/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/klepsydra/gen_dds_${PROJ_MAIN_NAME}"
    FILES_MATCHING PATTERN "*.h*"
)

add_dds_export_target(kpsr_dds_${PROJ_MAIN_NAME}_datamodel)

add_include_dirs("${CMAKE_INSTALL_FULL_INCLUDEDIR}/klepsydra/gen_dds_${PROJ_MAIN_NAME}")

#PRINTBASICINFO(${PROJ_NAME})

