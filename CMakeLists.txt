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

# NebulaModule.cmake
# 基本的模块文件，用于设置模块目标，依赖关系等
include(cmake/NebulaModule.cmake)

# DetectCXXCompiler.cmake
# 检测C++编译器
include(cmake/NebulaDetectCXXCompiler.cmake)


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

# ========================== Android details ==========================
if(ANDROID)
  status("")
  status("  Android: ")
  status("    Android ABI:" ${ANDROID_ABI})
  status("    STL type:" ${ANDROID_STL})
  status("    Native API level:" android-${ANDROID_NATIVE_API_LEVEL})
  android_get_compatible_target(android_sdk_target_status ${ANDROID_NATIVE_API_LEVEL} ${ANDROID_SDK_TARGET} 11)
  status("    SDK target:" "${android_sdk_target_status}")
  if(BUILD_WITH_ANDROID_NDK)
    status("    Android NDK:" "${ANDROID_NDK} (toolchain: ${ANDROID_TOOLCHAIN_NAME})")
  elseif(BUILD_WITH_STANDALONE_TOOLCHAIN)
    status("    Android toolchain:" "${ANDROID_STANDALONE_TOOLCHAIN}")
  endif()
  status("    android tool:"  ANDROID_EXECUTABLE  THEN "${ANDROID_EXECUTABLE} (${ANDROID_TOOLS_Pkg_Desc})" ELSE NO)
  status("    Google Play package:" BUILD_ANDROID_PACKAGE                                         THEN YES ELSE NO)
  status("    Android examples:"    BUILD_ANDROID_EXAMPLES AND CAN_BUILD_ANDROID_PROJECTS         THEN YES ELSE NO)
endif()

# ================== Windows RT features ==================
if(WIN32)
status("")
    status("  Windows RT support:" HAVE_WINRT THEN YES ELSE NO)
    if (ENABLE_WINRT_MODE)
      status("    Windows SDK v8.0:" ${WINDOWS_SDK_PATH})
      status("    Visual Studio 2012:" ${VISUAL_STUDIO_PATH})
    endif()
endif(WIN32)

# ========================== Other third-party libraries ==========================
status("")
status("  Other third-party libraries:")

if(WITH_IPP AND IPP_FOUND)
  status("    Use IPP:" "${IPP_LATEST_VERSION_STR} [${IPP_LATEST_VERSION_MAJOR}.${IPP_LATEST_VERSION_MINOR}.${IPP_LATEST_VERSION_BUILD}]")
  status("         at:" "${IPP_ROOT_DIR}")
else()
  status("    Use IPP:"   WITH_IPP AND NOT IPP_FOUND THEN "IPP not found" ELSE NO)
endif()

status("    Use Eigen:"      HAVE_EIGEN       THEN "YES (ver ${EIGEN_WORLD_VERSION}.${EIGEN_MAJOR_VERSION}.${EIGEN_MINOR_VERSION})" ELSE NO)
status("    Use TBB:"        HAVE_TBB         THEN "YES (ver ${TBB_VERSION_MAJOR}.${TBB_VERSION_MINOR} interface ${TBB_INTERFACE_VERSION})" ELSE NO)
status("    Use OpenMP:"     HAVE_OPENMP      THEN YES ELSE NO)
status("    Use GCD"         HAVE_GCD         THEN YES ELSE NO)
status("    Use Concurrency" HAVE_CONCURRENCY THEN YES ELSE NO)
status("    Use C=:"         HAVE_CSTRIPES    THEN YES ELSE NO)
status("    Use Cuda:"       HAVE_CUDA        THEN "YES (ver ${CUDA_VERSION_STRING})" ELSE NO)
status("    Use OpenCL:"     HAVE_OPENCL      THEN YES ELSE NO)

if(HAVE_CUDA)
  status("")
  status("  NVIDIA CUDA")

  status("    Use CUFFT:"            HAVE_CUFFT   THEN YES ELSE NO)
  status("    Use CUBLAS:"           HAVE_CUBLAS  THEN YES ELSE NO)
  status("    USE NVCUVID:"          HAVE_NVCUVID THEN YES ELSE NO)
  status("    NVIDIA GPU arch:"      ${NEBULA_CUDA_ARCH_BIN})
  status("    NVIDIA PTX archs:"     ${NEBULA_CUDA_ARCH_PTX})
  status("    Use fast math:"        CUDA_FAST_MATH THEN YES ELSE NO)
endif()

if(HAVE_OPENCL)
  status("")
  status("  OpenCL:")
  if(HAVE_OPENCL_STATIC)
    set(__opencl_ver "static")
  else()
    set(__opencl_ver "dynamic")
  endif()
  status("    Version:"       ${__opencl_ver})
  if(OPENCL_INCLUDE_DIR)
    status("    Include path:"       ${OPENCL_INCLUDE_DIRS})
  endif()
  if(OPENCL_LIBRARIES)
    set(__libs "")
    foreach(l ${OPENCL_LIBRARIES})
      if(TARGET ${l})
        get_target_property(p ${l} LOCATION)
        if(p MATCHES NOTFOUND)
          list(APPEND __libs "${l}")
        else()
          list(APPEND __libs "${p}")
        endif()
      else()
        list(APPEND __libs "${l}")
      endif()
    endforeach()
    status("    libraries:"          ${__libs})
  endif()
  status("    Use AMD FFT:"          HAVE_CLAMDFFT  THEN YES ELSE NO)
  status("    Use AMD BLAS:"         HAVE_CLAMDBLAS THEN YES ELSE NO)
endif()

# ========================== python ==========================
status("")
status("  Python:")
status("    Interpreter:"     PYTHON_EXECUTABLE   THEN "${PYTHON_EXECUTABLE} (ver ${PYTHON_VERSION_FULL})"         ELSE NO)
if(BUILD_opencv_python)
  if(PYTHONLIBS_VERSION_STRING)
    status("    Libraries:"   HAVE_opencv_python  THEN  "${PYTHON_LIBRARIES} (ver ${PYTHONLIBS_VERSION_STRING})"   ELSE NO)
  else()
    status("    Libraries:"   HAVE_opencv_python  THEN  ${PYTHON_LIBRARIES}                                        ELSE NO)
  endif()
  status("    numpy:"         PYTHON_USE_NUMPY    THEN "${PYTHON_NUMPY_INCLUDE_DIR} (ver ${PYTHON_NUMPY_VERSION})" ELSE "NO (Python wrappers can not be generated)")
  status("    packages path:" PYTHON_EXECUTABLE   THEN "${PYTHON_PACKAGES_PATH}"                                   ELSE "-")
endif()

# ========================== java ==========================
status("")
status("  Java:")
status("    ant:"           ANT_EXECUTABLE      THEN "${ANT_EXECUTABLE} (ver ${ANT_VERSION})"                    ELSE NO)
if(NOT ANDROID)
  status("    JNI:"         JNI_INCLUDE_DIRS    THEN "${JNI_INCLUDE_DIRS}"                                       ELSE NO)
endif()
status("    Java tests:"    BUILD_TESTS AND (CAN_BUILD_ANDROID_PROJECTS OR HAVE_opencv_java)            THEN YES ELSE NO)

# ========================== documentation ==========================
if(BUILD_DOCS)
  status("")
  status("  Documentation:")
  if(HAVE_SPHINX)
    status("    Build Documentation:" PDFLATEX_COMPILER      THEN YES ELSE "YES (only HTML and without math expressions)")
  else()
    status("    Build Documentation:" NO)
  endif()
  status("    Sphinx:"              HAVE_SPHINX              THEN "${SPHINX_BUILD} (ver ${SPHINX_VERSION})" ELSE NO)
  status("    PdfLaTeX compiler:"   PDFLATEX_COMPILER        THEN "${PDFLATEX_COMPILER}" ELSE NO)
endif()

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
status("  cvconfig.h is in:" "${NEBULA_CONFIG_FILE_INCLUDE_DIR}")
status("-----------------------------------------------------------------")
status("")

ocv_finalize_status()

# ----------------------------------------------------------------------------
# Warn in the case of in-source build
# ----------------------------------------------------------------------------
if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(WARNING "The source directory is the same as binary directory. \"make clean\" may damage the source tree")
endif()

