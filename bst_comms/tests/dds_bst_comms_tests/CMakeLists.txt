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

SET(PROJ_NAME kpsr_dds_${PROJ_MAIN_NAME}_tests)
PROJECT(${PROJ_NAME})

include (MacroConnext)
# Source
#---------------------------------------------------#
FILE(GLOB ${PROJ_NAME}_SRC
    "src/*.cpp"
)

# Link libraries with Project
#---------------------------------------------------#
ADD_EXECUTABLE(${PROJ_NAME} ${${PROJ_NAME}_SRC} ) 
target_include_directories(${PROJ_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

#---------------------------------------------------#
TARGET_LINK_LIBRARIES(${PROJ_NAME} kpsr_core yaml-cpp gtest_main
    kpsr_dds_serialization_datamodel kpsr_dds_${PROJ_MAIN_NAME}_datamodel)

add_test(NAME ${PROJ_NAME} COMMAND ${PROJ_NAME})
