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


SET(PROJ_MAIN_NAME "bst_comms")
PROJECT(${PROJ_MAIN_NAME})

if (NOT KPSR_CODEGEN_DISABLED)
    include(${KLEPSYDRA_CODE_GENERATOR})
    KpsrEventGenerator("${CMAKE_CURRENT_SOURCE_DIR}/kidl" "${CMAKE_CURRENT_SOURCE_DIR}/gen" "klepsydra/bst_comms" "True" "False" "False" "True")
endif()

######################################################################################
# ADD SUB FOLDERS
######################################################################################
ADD_SUBDIRECTORY(modules)
ADD_SUBDIRECTORY(tests)

add_core_export_target()
add_dds_export_target()
add_zmq_export_target()
add_include_dirs()

