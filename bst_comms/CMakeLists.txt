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

SET(PROJ_MAIN_NAME "bst_comms")
PROJECT(${PROJ_MAIN_NAME})

if (NOT KPSR_CODEGEN_DISABLED)
    include(${KLEPSYDRA_CODE_GENERATOR})
    KpsrEventGenerator("${CMAKE_CURRENT_SOURCE_DIR}/kidl" "${CMAKE_CURRENT_SOURCE_DIR}/gen" "klepsydra/bst_comms" "True" "False" "True" "True")
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

