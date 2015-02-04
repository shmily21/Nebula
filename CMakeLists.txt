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

#
#------------------------------------------------------------
NEBULA_OPTION(CMAKE_VERBOSE "Verbose Mode" OFF)

# THE THIRD PART LIBRARIES
# 编译第三方库选项
#------------------------------------------------------------
NEBULA_OPTION(BUILD_LUA                 "Build Lua Library"             ON)

#------------------------------------------------------------
# NebulaModule.cmake
# 基本的模块文件，用于设置模块目标，依赖关系等
include(cmake/NebulaModule.cmake)

# DetectCXXCompiler.cmake
# 检测C++编译器
include(cmake/NebulaDetectCXXCompiler.cmake)



#------------------------------------------------------------

# 3rdparty
# 编译第三方库
add_subdirectory(3rdparty)

# data
# 编译或处理数据
add_subdirectory(data)

# platforms
# 编译平台文件
add_subdirectory(platforms)

# modules
# 编译模块文件
add_subdirectory(modules)

# doc
# 编译或处理文档
add_subdirectory(doc)

# test
# 编译或处理测试模块
add_subdirectory(test)

# 输出检测、配置结果
# ----------------------------------------------------------------------------
# Summary:
# ----------------------------------------------------------------------------
status("")
status("General configuration for Nebula ${NEBULA_VERSION} =====================================")
if(NEBULA_VCSVERSION)
  status("  Version control:" ${NEBULA_VCSVERSION})
endif()

# ========================== build platform ==========================
status("")
status("  Platform:")
status("    Host:"             ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_HOST_SYSTEM_VERSION} ${CMAKE_HOST_SYSTEM_PROCESSOR})
if(CMAKE_CROSSCOMPILING)
  status("    Target:"         ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR})
endif()
status("    CMake:"            ${CMAKE_VERSION})
status("    CMake generator:"  ${CMAKE_GENERATOR})
status("    CMake build tool:" ${CMAKE_BUILD_TOOL})
if(MSVC)
  status("    MSVC:"           ${MSVC_VERSION})
endif()
if(CMAKE_GENERATOR MATCHES Xcode)
  status("    Xcode:"          ${XCODE_VERSION})
endif()
if(NOT CMAKE_GENERATOR MATCHES "Xcode|Visual Studio")
  status("    Configuration:"  ${CMAKE_BUILD_TYPE})
endif()

# ========================== C/C++ options ==========================
if(CMAKE_CXX_COMPILER_VERSION)
  set(NEBULA_COMPILER_STR "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} (ver ${CMAKE_CXX_COMPILER_VERSION})")
elseif(CMAKE_COMPILER_IS_CLANGCXX)
  set(NEBULA_COMPILER_STR "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} (ver ${CMAKE_CLANG_REGEX_VERSION})")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(NEBULA_COMPILER_STR "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1} (ver ${CMAKE_GCC_REGEX_VERSION})")
else()
  set(NEBULA_COMPILER_STR "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ARG1}")
endif()
string(STRIP "${NEBULA_COMPILER_STR}" NEBULA_COMPILER_STR)

status("")
status("  C/C++:")
status("    Built as dynamic libs?:" BUILD_SHARED_LIBS THEN YES ELSE NO)
status("    C++ Compiler:"           ${NEBULA_COMPILER_STR})
status("    C++ flags (Release):"    ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE})
status("    C++ flags (Debug):"      ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG})
status("    C Compiler:"             ${CMAKE_C_COMPILER} ${CMAKE_C_COMPILER_ARG1})
status("    C flags (Release):"      ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE})
status("    C flags (Debug):"        ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG})
if(WIN32)
  status("    Linker flags (Release):" ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
  status("    Linker flags (Debug):"   ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_DEBUG})
else()
  status("    Linker flags (Release):" ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_RELEASE})
  status("    Linker flags (Debug):"   ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS_DEBUG})
endif()
status("    Precompiled headers:"     PCHSupport_FOUND AND ENABLE_PRECOMPILED_HEADERS THEN YES ELSE NO)

#======================== The Third Part ============================
status("")
status(" third part:")
status("    Lua"      BUILD_LUA       THEN "Build lua(ver ${LUA_VERSION_STRING})"               ELSE "${LUA_LIBRARY} (ver ${LUA_VERSION_STRING})" )

# ========================== Nebula modules ==========================
status("")
status("  Nebula modules:")
string(REPLACE "Nebula" "" NEBULA_MODULES_BUILD_ST          "${NEBULA_MODULES_BUILD}")
string(REPLACE "Nebula" "" NEBULA_MODULES_DISABLED_USER_ST  "${NEBULA_MODULES_DISABLED_USER}")
string(REPLACE "Nebula" "" NEBULA_MODULES_DISABLED_FORCE_ST "${NEBULA_MODULES_DISABLED_FORCE}")
set(NEBULA_MODULES_DISABLED_AUTO_ST "")
foreach(m ${NEBULA_MODULES_DISABLED_AUTO})
  set(__mdeps "")
  foreach(d ${NEBULA_MODULE_${m}_DEPS})
    if(d MATCHES "^Nebula" AND NOT HAVE_${d})
      list(APPEND __mdeps ${d})
    endif()
  endforeach()
  if(__mdeps)
    list(APPEND NEBULA_MODULES_DISABLED_AUTO_ST "${m}(deps: ${__mdeps})")
  else()
    list(APPEND NEBULA_MODULES_DISABLED_AUTO_ST "${m}")
  endif()
endforeach()
string(REPLACE "Nebula" "" NEBULA_MODULES_DISABLED_AUTO_ST  "${NEBULA_MODULES_DISABLED_AUTO_ST}")

status("    To be built:"            NEBULA_MODULES_BUILD          THEN ${NEBULA_MODULES_BUILD_ST}          ELSE "-")
status("    Disabled:"               NEBULA_MODULES_DISABLED_USER  THEN ${NEBULA_MODULES_DISABLED_USER_ST}  ELSE "-")
status("    Disabled by dependency:" NEBULA_MODULES_DISABLED_AUTO  THEN ${NEBULA_MODULES_DISABLED_AUTO_ST}  ELSE "-")
status("    Unavailable:"            NEBULA_MODULES_DISABLED_FORCE THEN ${NEBULA_MODULES_DISABLED_FORCE_ST} ELSE "-")

# ================== Windows RT features ==================
if(WIN32)
status("")
    status("  Windows RT support:" HAVE_WINRT THEN YES ELSE NO)
    if (ENABLE_WINRT_MODE)
      status("    Windows SDK v8.0:" ${WINDOWS_SDK_PATH})
      status("    Visual Studio 2012:" ${VISUAL_STUDIO_PATH})
    endif()
endif(WIN32)

# ========================== samples and tests ==========================
status("")
status("  Tests and samples:")
status("    Tests:"             BUILD_TESTS AND HAVE_opencv_ts       THEN YES ELSE NO)
status("    Performance tests:" BUILD_PERF_TESTS AND HAVE_opencv_ts  THEN YES ELSE NO)
status("    C/C++ Examples:"    BUILD_EXAMPLES                       THEN YES ELSE NO)

# ========================== auxiliary ==========================
status("")
status("  Install path:" "${CMAKE_INSTALL_PREFIX}")
status("")
status("  nebulaconfig.h is in:" "${NEBULA_CONFIG_FILE_INCLUDE_DIR}")
status("-----------------------------------------------------------------")
status("")

nebula_finalize_status()

# ----------------------------------------------------------------------------
# Warn in the case of in-source build
# ----------------------------------------------------------------------------
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(WARNING "The source directory is the same as binary directory. \"make clean\" may damage the source tree")
endif()

