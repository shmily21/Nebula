#
#**************************************************************************************************
# FileName      :CMakeLists.txt
#
# Brief         :Root CMake file for Nebula
#
# Author        :WangBin
#
# Description   :invoke:
#                  $ cmake <PATH_TO_NEBULA_ROOT>
#                   <PATH_TO_NEBULA_ROOT> REF TO THIS FILE.
#
# CopyRight    : WangBin.
#                                              <perturbed@sina.com>
#
#**************************************************************************************************
#

project(Nebula CXX C)

set(CMAKE_ALLOC_LOOSE_LOOP_CONSTRUCTS true)

if(NOT CMAKE_TOOLCHAIN_FILE)
    if(WIN32)
        set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
    else()
        set(CMAKE_INSTALL_PREFIX "/usr/share" CACHE PATH "Installation Directory")
    endif()
else(NOT CMAKE_TOOLCHAIN_FILE)
    set(LIBRARY_OUTPUT_PATH_ROOT ${CMAKE_BINARY_DIR} CACHE PATH "root for output")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory")
endif(NOT CMAKE_TOOLCHAIN_FILE)

if(CMAKE_GENERATOR MATCHES Xcode AND XCODE_VERSION VERSION_GREATER 4.3)
    cmake_minimum_required(VERSION 2.8.8)
elseif(IOS)
    cmake_minimum_required(VERSION 2.8.0)
else()
    cmake_minimum_required(VERSION 2.6.3)
endif()

if(POLICY CMP0017)
    cmake_policy(SET CMP0017 NEW)
endif()

if(POLICY CMP0022)
    cmake_policy(SET CMP0022 OLD)
endif()

# must go before the project command
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

if(DEFINED CMAKE_BUILD_TYPE AND CMAKE_VERSION VERSION_GREATER "2.8")
    set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES} )
endif()

if(MSVC)
    set(CMAKE_USE_RELATIVE_PATHS ON CACHE INTERNAL "" FORCE)
endif()

include(cmake/NebulaUtils.cmake)

#clear all target (for compile)
# 变量 "NebulaModules_TARGETS" 存储所有需要编译的模块
nebula_clear_vars(NebulaModules_TARGETS)

if(NOT CMAKE_SIZEOF_VOID_P GREATER 0)
    message(FATAL_ERROR "CMake fails to deterimine the bitness of target platform.
    Please check your CMake and compiler installation.
    If you are crosscompiling then ensure that your CMake toolchain file correctly sets the compiler details.")
endif()

