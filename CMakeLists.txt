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

# Main header: Project name & Configuration
# ------------------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.12)

set(PROJ_MAIN_NAME "bst")
project("kpsr-bst")

set(KPSR_BST_MAJOR_VERSION 4)
set(KPSR_BST_MINOR_VERSION 0)
set(KPSR_BST_PATCH_VERSION 0)
set(KPSR_BST_VERSION
    ${KPSR_BST_MAJOR_VERSION}.${KPSR_BST_MINOR_VERSION}.${KPSR_BST_PATCH_VERSION}
)

# Configuration types
set(CMAKE_CONFIGURATION_TYPES
    "Debug;Release"
    CACHE STRING "Configs" FORCE)
if(DEFINED CMAKE_BUILD_TYPE AND CMAKE_VERSION VERSION_GREATER "2.8")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
                                                 ${CMAKE_CONFIGURATION_TYPES})
endif()

# Install binaries
if(DEFINED THIRDPARTIES_PATH)

else()
    set(THIRDPARTIES_PATH ${CMAKE_SOURCE_DIR}/thirdparties)
endif()
message(STATUS "Thirdparties install path: " ${THIRDPARTIES_PATH})

if(DEFINED BST_SDK_HOME)
    set(bst_home ${BST_SDK_HOME})
else()
    set(bst_home "${CMAKE_SOURCE_DIR}/../../bst_sdk/v2.1.0a/sdk/v2.1.0")
    message(
        "BST_SDK_HOME not provided. Using the value ${bst_home} to look for BST libs and headers"
    )
endif()

if(DEFINED GTEST_PATH)

else()
    set(GTEST_PATH ${THIRDPARTIES_PATH}/googletest) # default google test path
endif()
message(STATUS "Google test path: " ${GTEST_PATH})
if(DEFINED KPSR_BUILD_PATH)

else()
    set(KPSR_BUILD_PATH ${CMAKE_SOURCE_DIR}/kpsr-build)
endif()
message(STATUS "kpsr-build path: " ${KPSR_BUILD_PATH})

# Configure variables
# ------------------------------------------------------------------------------------------------------

# Paths
set(CMAKE_MODULE_PATH ${KPSR_BUILD_PATH}/cmake)
set(LIBRARY_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH "Library output" FORCE)
set(EXECUTABLE_OUTPUT_PATH
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH "Executable output" FORCE)

set(BST_INCLUDE_PATH ${bst_home}/include)
set(BST_SOURCE_PATH ${bst_home}/src)
set(BST_LIBRARY_PATH ${bst_home}/lib)
set(BST_LIBRARY_CORE_NAME bstmr_lib)
set(BST_LIBRARY_NETUAS_NAME netuas_lib)

find_library(
    BST_LIBRARY_CORE
    NAMES ${BST_LIBRARY_CORE_NAME}
    PATHS ${BST_LIBRARY_PATH})

if(NOT BST_LIBRARY_CORE)
    message(
        FATAL_ERROR
            "Could not find ${BST_LIBRARY_CORE_NAME} in location ${BST_LIBRARY_PATH}"
    )
endif()

find_library(
    BST_LIBRARY_NETUAS
    NAMES ${BST_LIBRARY_NETUAS_NAME}
    PATHS ${BST_LIBRARY_PATH})

if(NOT BST_LIBRARY_NETUAS)
    message(
        FATAL_ERROR
            "Could not find ${BST_LIBRARY_NETUAS_NAME} in location ${BST_LIBRARY_PATH}"
    )
endif()

set(BST_INCLUDE_DIRS)
list(
    APPEND
    BST_INCLUDE_DIRS
    ${BST_INCLUDE_PATH}
    ${BST_INCLUDE_PATH}/bst_core
    ${BST_INCLUDE_PATH}/bst_can
    ${BST_INCLUDE_PATH}/bst_protocol
    ${BST_INCLUDE_PATH}/bst_protocol/modules
    ${BST_INCLUDE_PATH}/bst_protocol/messages
    ${BST_LIBRARY_PATH})

add_library(bst::core STATIC IMPORTED)
set_target_properties(
    bst::core PROPERTIES IMPORTED_LOCATION "${BST_LIBRARY_CORE}"
                         INTERFACE_INCLUDE_DIRECTORIES "${BST_INCLUDE_DIRS}")

add_library(bst::netuas STATIC IMPORTED)
set_target_properties(
    bst::netuas PROPERTIES IMPORTED_LOCATION "${BST_LIBRARY_NETUAS}"
                           INTERFACE_INCLUDE_DIRECTORIES "${BST_INCLUDE_DIRS}")

target_link_libraries(bst::netuas INTERFACE bst::core rt)

add_library(bst::bst INTERFACE IMPORTED)
set_property(TARGET bst::bst PROPERTY INTERFACE_LINK_LIBRARIES bst::netuas)

# Must use GNUInstallDirs to install libraries into correct locations on all
# platforms.
include(GNUInstallDirs)

# Build type
set(BUILD_SHARED_LIBS
    ON
    CACHE BOOL "Build shared libraries" FORCE)

include(CheckCXXCompilerFlag)
include(CheckIncludeFileCXX)

check_cxx_compiler_flag(-std=gnu++11 HAS_CXX11)
if(HAS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif()

if(NOT APPLE)
    set(CMAKE_INSTALL_RPATH $ORIGIN)
    check_cxx_compiler_flag(-Wl,--no-undefined HAS_NO_UNDEFINED)
    if(HAS_NO_UNDEFINED)
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined")
        set(CMAKE_SHARED_LINKER_FLAGS
            "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
        set(CMAKE_MODULE_LINKER_FLAGS
            "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    endif()
endif()

check_cxx_compiler_flag(-pedantic HAS_PEDANTIC)
if(HAS_PEDANTIC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
endif()

check_cxx_compiler_flag(-Wall HAS_WALL)
if(HAS_WALL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

check_include_file_cxx(sys/epoll.h HAS_EPOLL)
if(HAS_EPOLL)
    add_definitions(-DHAS_EPOLL)
else()
    add_definitions(-DUSE_GET_WRITE_QUEUE)
endif()

add_definitions(-std=c++11)
enable_testing()

add_definitions(-faligned-new)
add_definitions(-fno-strict-overflow)
add_definitions(-Wno-deprecated-declarations)
add_definitions(-Wno-comment)

if(KPSR_WITH_CODE_METRICS)
    if(CMAKE_BUILD_TYPE MATCHES Debug AND CMAKE_COMPILER_IS_GNUCXX)
        include(CodeCoverage)
        set(CODE_COVERAGE_VERBOSE ON)
        append_coverage_compiler_flags()
    endif()
    # Code style check
    include(CppcheckTargets)
    include(Findcppcheck)
endif()

# Code format check
include(CodeFormat)
# CCACHE support
include(FindCcache)

# Code Coverage
if(KPSR_WITH_CODE_METRICS
   AND CMAKE_BUILD_TYPE MATCHES Debug
   AND CMAKE_COMPILER_IS_GNUCXX)
    string(REPLACE ${CMAKE_SOURCE_DIR}/ "" THIRDPARTIES_PATH_COVERAGE
                   ${THIRDPARTIES_PATH})
    list(APPEND COVERAGE_EXCLUDES ${THIRDPARTIES_PATH_COVERAGE}
         ${CMAKE_PREFIX_PATH})
    setup_target_for_coverage_gcovr_xml(NAME coverage EXECUTABLE make test)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(KlepsydraBuild)

# Configuration current project
# ------------------------------------------------------------------------------------------------------

# Dependencies

find_package(Klepsydra 7 REQUIRED)
find_package(KlepsydraRobotics REQUIRED)
find_package(Threads REQUIRED)

# Project Logic
# ------------------------------------------------------------------------------------------------------

# ##############################################################################
# ADD SUB FOLDERS
# ##############################################################################
add_subdirectory(bst_comms)
add_subdirectory(bst_client_server)

if(KPSR_RUNTIME)

else()
    # add a target to generate API documentation with Doxygen
    if(KPSR_WITH_DOXYGEN)
        find_package(Doxygen REQUIRED)
        set(DOC_PROJECT_NAME "Klepsydra BST Integration")
        set(DOC_PROJECT_BRIEF "Klepsydra BST API")

        # Generate documentation only for directories where there is a
        # group_definitions.h file

        file(GLOB_RECURSE DOC_DIRS "*/modules/group_definitions.h")
        string(REGEX REPLACE "/group_definitions.h[;]?" " " DOC_DIRS
                             "${DOC_DIRS}")

        configure_file(${KPSR_BUILD_PATH}/docs/Doxyfile.in
                       ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(
            doc
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            COMMAND
                moxygen --templates ${KPSR_BUILD_PATH}/docs/moxygen/templates
                --groups --output api-%s.md ./xml
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    endif(KPSR_WITH_DOXYGEN)
endif()

set(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/klepsydrabst)

list(APPEND EXPORT_INCLUDE_DIRS ${CMAKE_INSTALL_FULL_INCLUDEDIR})
message("CORE_EXPORT_TARGETS: " ${CORE_EXPORT_TARGETS})
message("DDS_EXPORT_TARGETS: " ${DDS_EXPORT_TARGETS})
message("ZMQ_EXPORT_TARGETS: " ${ZMQ_EXPORT_TARGETS})
message("EXPORT_INCLUDE_DIRS: " ${EXPORT_INCLUDE_DIRS})
export(TARGETS ${CORE_EXPORT_TARGETS} ${DDS_EXPORT_TARGETS}
               ${ZMQ_EXPORT_TARGETS}
       FILE "${PROJECT_BINARY_DIR}/KlepsydraBstTargets.cmake")

# Export the package for use from the build-tree (this registers the build-tree
# with a global CMake-registry)
export(PACKAGE bst)

# Create the KlepsydraBstConfig.cmake and KlepsydraBstConfigVersion files
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "KlepsydraBstConfig.cmake.in"
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/KlepsydraBstConfig.cmake"
    INSTALL_DESTINATION "${INSTALL_CMAKE_DIR}"
    PATH_VARS CMAKE_INSTALL_LIBDIR)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/KlepsydraBstConfigVersion.cmake"
    VERSION ${KPSR_BST_VERSION}
    COMPATIBILITY SameMajorVersion)

# Install the KlepsydraBstConfig.cmake and KlepsydraBstConfigVersion.cmake
install(
    FILES
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/KlepsydraBstConfig.cmake"
        "${PROJECT_BINARY_DIR}/KlepsydraBstConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}"
    COMPONENT dev)

# Install the export set for use with the install-tree
install(
    EXPORT KlepsydraBstTargets
    DESTINATION "${INSTALL_CMAKE_DIR}"
    COMPONENT dev)

# PACKAGING
if(NOT DEFINED KPSR_VERSION_MAJOR)
    set(KPSR_VERSION_MAJOR ${KPSR_BST_MAJOR_VERSION})
endif()

if(NOT DEFINED KPSR_VERSION_MINOR)
    set(KPSR_VERSION_MINOR ${KPSR_BST_MAJOR_VERSION})
endif()

if(NOT DEFINED KPSR_VERSION_PATCH)
    set(KPSR_VERSION_PATCH ${KPSR_BST_PATCH_VERSION})
endif()

set(KPSR_PACKAGE_DESCRIPTION "Klepsydra SDK BST Libraries")
set(KPSR_PACKAGE_DESCRIPTION_SUMMARY "Klepsydra SDK BST Libraries")

include(KlepsydraPackaging)
