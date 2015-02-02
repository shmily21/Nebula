# Local variables (set for each module):
#
# name       - short name in lower case i.e. core
# the_module - full name in lower case i.e. opencv_core

# Global variables:
#
# NEBULA_MODULE_${the_module}_LOCATION
# NEBULA_MODULE_${the_module}_DESCRIPTION
# NEBULA_MODULE_${the_module}_CLASS - PUBLIC|INTERNAL|BINDINGS
# NEBULA_MODULE_${the_module}_HEADERS
# NEBULA_MODULE_${the_module}_SOURCES
# NEBULA_MODULE_${the_module}_DEPS - final flattened set of module dependencies
# NEBULA_MODULE_${the_module}_DEPS_EXT - non-module dependencies
# NEBULA_MODULE_${the_module}_REQ_DEPS
# NEBULA_MODULE_${the_module}_OPT_DEPS
# NEBULA_MODULE_${the_module}_PRIVATE_REQ_DEPS
# NEBULA_MODULE_${the_module}_PRIVATE_OPT_DEPS
# HAVE_${the_module} - for fast check of module availability

# To control the setup of the module you could also set:
# the_description - text to be used as current module description
# NEBULA_MODULE_TYPE - STATIC|SHARED - set to force override global settings for current module
# NEBULA_MODULE_IS_PART_OF_WORLD - ON|OFF (default ON) - should the module be added to the opencv_world?
# BUILD_${the_module}_INIT - ON|OFF (default ON) - initial value for BUILD_${the_module}

# The verbose template for OpenCV module:
#
#   nebula_add_module(modname <dependencies>)
#   nebula_glob_module_sources() or glob them manually and nebula_set_module_sources(...)
#   nebula_module_include_directories(<extra include directories>)
#   nebula_create_module()
#   <add extra link dependencies, compiler options, etc>
#   nebula_add_precompiled_headers(${the_module})
#   <add extra installation rules>
#   nebula_add_accuracy_tests(<extra dependencies>)
#   nebula_add_perf_tests(<extra dependencies>)
#   nebula_add_samples(<extra dependencies>)
#
#
# If module have no "extra" then you can define it in one line:
#
#   nebula_define_module(modname <dependencies>)

# clean flags for modules enabled on previous cmake run
# this is necessary to correctly handle modules removal
foreach(mod ${NEBULA_MODULES_BUILD} ${NEBULA_MODULES_DISABLED_USER} ${NEBULA_MODULES_DISABLED_AUTO} ${NEBULA_MODULES_DISABLED_FORCE})
  if(HAVE_${mod})
    unset(HAVE_${mod} CACHE)
  endif()
  unset(NEBULA_MODULE_${mod}_REQ_DEPS CACHE)
  unset(NEBULA_MODULE_${mod}_OPT_DEPS CACHE)
  unset(NEBULA_MODULE_${mod}_PRIVATE_REQ_DEPS CACHE)
  unset(NEBULA_MODULE_${mod}_PRIVATE_OPT_DEPS CACHE)
endforeach()

# clean modules info which needs to be recalculated
set(NEBULA_MODULES_PUBLIC         "" CACHE INTERNAL "List of Nebula modules marked for export")
set(NEBULA_MODULES_BUILD          "" CACHE INTERNAL "List of Nebula modules included into the build")
set(NEBULA_MODULES_DISABLED_USER  "" CACHE INTERNAL "List of Nebula modules explicitly disabled by user")
set(NEBULA_MODULES_DISABLED_AUTO  "" CACHE INTERNAL "List of Nebula modules implicitly disabled due to dependencies")
set(NEBULA_MODULES_DISABLED_FORCE "" CACHE INTERNAL "List of Nebula modules which can not be build in current configuration")

# adds dependencies to OpenCV module
# Usage:
#   add_dependencies(Nebula<name> [REQUIRED] [<list of dependencies>] [OPTIONAL <list of modules>])
# Notes:
# * <list of dependencies> - can include full names of modules or full pathes to shared/static libraries or cmake targets
macro(nebula_add_dependencies full_modname)
  #we don't clean the dependencies here to allow this macro several times for every module
  foreach(d "REQUIRED" ${ARGN})
    if(d STREQUAL "REQUIRED")
      set(__depsvar NEBULA_MODULE_${full_modname}_REQ_DEPS)
    elseif(d STREQUAL "OPTIONAL")
      set(__depsvar NEBULA_MODULE_${full_modname}_OPT_DEPS)
    elseif(d STREQUAL "PRIVATE_REQUIRED")
      set(__depsvar NEBULA_MODULE_${full_modname}_PRIVATE_REQ_DEPS)
    elseif(d STREQUAL "PRIVATE_OPTIONAL")
      set(__depsvar NEBULA_MODULE_${full_modname}_PRIVATE_OPT_DEPS)
    else()
      list(APPEND ${__depsvar} "${d}")
    endif()
  endforeach()
  unset(__depsvar)

  nebula_list_unique(NEBULA_MODULE_${full_modname}_REQ_DEPS)
  nebula_list_unique(NEBULA_MODULE_${full_modname}_OPT_DEPS)
  nebula_list_unique(NEBULA_MODULE_${full_modname}_PRIVATE_REQ_DEPS)
  nebula_list_unique(NEBULA_MODULE_${full_modname}_PRIVATE_OPT_DEPS)

  set(NEBULA_MODULE_${full_modname}_REQ_DEPS ${NEBULA_MODULE_${full_modname}_REQ_DEPS}
    CACHE INTERNAL "Required dependencies of ${full_modname} module")
  set(NEBULA_MODULE_${full_modname}_OPT_DEPS ${NEBULA_MODULE_${full_modname}_OPT_DEPS}
    CACHE INTERNAL "Optional dependencies of ${full_modname} module")
  set(NEBULA_MODULE_${full_modname}_PRIVATE_REQ_DEPS ${NEBULA_MODULE_${full_modname}_PRIVATE_REQ_DEPS}
    CACHE INTERNAL "Required private dependencies of ${full_modname} module")
  set(NEBULA_MODULE_${full_modname}_PRIVATE_OPT_DEPS ${NEBULA_MODULE_${full_modname}_PRIVATE_OPT_DEPS}
    CACHE INTERNAL "Optional private dependencies of ${full_modname} module")
endmacro()

# declare new OpenCV module in current folder
# Usage:
#   nebula_add_module(<name> [INTERNAL|BINDINGS] [REQUIRED] [<list of dependencies>] [OPTIONAL <list of optional dependencies>])
# Example:
#   nebula_add_module(yaom INTERNAL opencv_core opencv_highgui opencv_flann OPTIONAL opencv_gpu)
macro(nebula_add_module _name)
  string(TOLOWER "${_name}" name)
  string(REGEX REPLACE "^Nebula" "" ${name} "${name}")
  set(the_module Nebula${name})

  # the first pass - collect modules info, the second pass - create targets
  if(NEBULA_INITIAL_PASS)
    #guard agains redefinition
    if(";${NEBULA_MODULES_BUILD};${NEBULA_MODULES_DISABLED_USER};" MATCHES ";${the_module};")
      message(FATAL_ERROR "Redefinition of the ${the_module} module.
  at:                    ${CMAKE_CURRENT_SOURCE_DIR}
  previously defined at: ${NEBULA_MODULE_${the_module}_LOCATION}
")
    endif()

    if(NOT DEFINED the_description)
      set(the_description "The ${name} Nebula module")
    endif()

    if(NOT DEFINED BUILD_${the_module}_INIT)
      set(BUILD_${the_module}_INIT ON)
    endif()

    # create option to enable/disable this module
    option(BUILD_${the_module} "Include ${the_module} module into the Nebula build" ${BUILD_${the_module}_INIT})

    # remember the module details
    set(NEBULA_MODULE_${the_module}_DESCRIPTION "${the_description}" CACHE INTERNAL "Brief description of ${the_module} module")
    set(NEBULA_MODULE_${the_module}_LOCATION    "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Location of ${the_module} module sources")

    # parse list of dependencies
    if("${ARGV1}" STREQUAL "INTERNAL" OR "${ARGV1}" STREQUAL "BINDINGS")
      set(NEBULA_MODULE_${the_module}_CLASS "${ARGV1}" CACHE INTERNAL "The cathegory of the module")
      set(__nebula_argn__ ${ARGN})
      list(REMOVE_AT __nebula_argn__ 0)
      nebula_add_dependencies(${the_module} ${__nebula_argn__})
      unset(__nebula_argn__)
    else()
      set(NEBULA_MODULE_${the_module}_CLASS "PUBLIC" CACHE INTERNAL "The cathegory of the module")
      nebula_add_dependencies(${the_module} ${ARGN})
      if(BUILD_${the_module})
        set(NEBULA_MODULES_PUBLIC ${NEBULA_MODULES_PUBLIC} "${the_module}" CACHE INTERNAL "List of Nebula modules marked for export")
      endif()
    endif()

    # add self to the world dependencies
    if(NOT DEFINED NEBULA_MODULE_IS_PART_OF_WORLD AND NOT NEBULA_MODULE_${the_module}_CLASS STREQUAL "BINDINGS" OR NEBULA_MODULE_IS_PART_OF_WORLD)
      nebula_add_dependencies(NebulaWorld OPTIONAL ${the_module})
    endif()

    if(BUILD_${the_module})
      set(NEBULA_MODULES_BUILD ${NEBULA_MODULES_BUILD} "${the_module}" CACHE INTERNAL "List of Nebula modules included into the build")
    else()
      set(NEBULA_MODULES_DISABLED_USER ${NEBULA_MODULES_DISABLED_USER} "${the_module}" CACHE INTERNAL "List of Nebula modules explicitly disabled by user")
    endif()

    # TODO: add submodules if any

    # stop processing of current file
    return()
  else(NEBULA_INITIAL_PASS)
    if(NOT BUILD_${the_module})
      return() # extra protection from redefinition
    endif()
    project(${the_module})
  endif(NEBULA_INITIAL_PASS)
endmacro()

# excludes module from current configuration
macro(nebula_module_disable module)
  set(__modname ${module})
  if(NOT __modname MATCHES "^Nebula")
    set(__modname Nebula${module})
  endif()
  list(APPEND NEBULA_MODULES_DISABLED_FORCE "${__modname}")
  set(HAVE_${__modname} OFF CACHE INTERNAL "Module ${__modname} can not be built in current configuration")
  set(NEBULA_MODULE_${__modname}_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "Location of ${__modname} module sources")
  set(NEBULA_MODULES_DISABLED_FORCE "${NEBULA_MODULES_DISABLED_FORCE}" CACHE INTERNAL "List of Nebula modules which can not be build in current configuration")
  if(BUILD_${__modname})
    # touch variable controlling build of the module to suppress "unused variable" CMake warning
  endif()
  unset(__modname)
  return() # leave the current folder
endmacro()


# collect modules from specified directories
# NB: must be called only once!
macro(nebula_glob_modules)
  if(DEFINED NEBULA_INITIAL_PASS)
    message(FATAL_ERROR "Nebula has already loaded its modules. Calling nebula_glob_modules second time is not allowed.")
  endif()
  set(__directories_observed "")

  # collect modules
  set(NEBULA_INITIAL_PASS ON)
  foreach(__path ${ARGN})
    get_filename_component(__path "${__path}" ABSOLUTE)

    list(FIND __directories_observed "${__path}" __pathIdx)
    if(__pathIdx GREATER -1)
      message(FATAL_ERROR "The directory ${__path} is observed for Nebula modules second time.")
    endif()
    list(APPEND __directories_observed "${__path}")

    file(GLOB __nebulamodules RELATIVE "${__path}" "${__path}/*")
    if(__nebulamodules)
      list(SORT __nebulamodules)
      foreach(mod ${__nebulamodules})
        get_filename_component(__modpath "${__path}/${mod}" ABSOLUTE)
        if(EXISTS "${__modpath}/CMakeLists.txt")

          list(FIND __directories_observed "${__modpath}" __pathIdx)
          if(__pathIdx GREATER -1)
            message(FATAL_ERROR "The module from ${__modpath} is already loaded.")
          endif()
          list(APPEND __directories_observed "${__modpath}")

          if(NEBULA_MODULE_RELOCATE_ON_INITIAL_PASS)
            file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${mod}/.${mod}")
            file(COPY "${__modpath}/CMakeLists.txt" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${mod}/.${mod}")
            add_subdirectory("${CMAKE_CURRENT_BINARY_DIR}/${mod}/.${mod}" "${CMAKE_CURRENT_BINARY_DIR}/${mod}/.${mod}")
            if("${NEBULA_MODULE_Nebula${mod}_LOCATION}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}/${mod}/.${mod}")
              set(NEBULA_MODULE_Nebula${mod}_LOCATION "${__modpath}" CACHE PATH "" FORCE)
            endif()
          else()
            add_subdirectory("${__modpath}" "${CMAKE_CURRENT_BINARY_DIR}/${mod}/.${mod}")
          endif()
        endif()
      endforeach()
    endif()
  endforeach()
  nebula_clear_vars(__nebulamodules __directories_observed __path __modpath __pathIdx)

  # resolve dependencies
  __nebula_resolve_dependencies()

  # create modules
  set(NEBULA_INITIAL_PASS OFF PARENT_SCOPE)
  set(NEBULA_INITIAL_PASS OFF)
  foreach(m ${NEBULA_MODULES_BUILD})
    if(m MATCHES "^Nebula")
      string(REGEX REPLACE "^Nebula" "" __shortname "${m}")
      add_subdirectory("${Nebula_MODULE_${m}_LOCATION}" "${CMAKE_CURRENT_BINARY_DIR}/${__shortname}")
    else()
      message(WARNING "Check module name: ${m}")
      add_subdirectory("${Nebula_MODULE_${m}_LOCATION}" "${CMAKE_CURRENT_BINARY_DIR}/${m}")
    endif()
  endforeach()
  unset(__shortname)
endmacro()


# disables OpenCV module with missing dependencies
function(__nebula_module_turn_off the_module)
  list(REMOVE_ITEM NEBULA_MODULES_DISABLED_AUTO "${the_module}")
  list(APPEND NEBULA_MODULES_DISABLED_AUTO "${the_module}")
  list(REMOVE_ITEM NEBULA_MODULES_BUILD "${the_module}")
  list(REMOVE_ITEM NEBULA_MODULES_PUBLIC "${the_module}")
  set(HAVE_${the_module} OFF CACHE INTERNAL "Module ${the_module} can not be built in current configuration")

  set(NEBULA_MODULES_DISABLED_AUTO "${NEBULA_MODULES_DISABLED_AUTO}" CACHE INTERNAL "")
  set(NEBULA_MODULES_BUILD "${NEBULA_MODULES_BUILD}" CACHE INTERNAL "")
  set(NEBULA_MODULES_PUBLIC "${NEBULA_MODULES_PUBLIC}" CACHE INTERNAL "")
endfunction()

# sort modules by dependencies
function(__nebula_sort_modules_by_deps __lst)
  nebula_list_sort(${__lst})
  set(${__lst}_ORDERED ${${__lst}} CACHE INTERNAL "")
  set(__result "")
  foreach (m ${${__lst}})
    list(LENGTH __result __lastindex)
    set(__index ${__lastindex})
    foreach (__d ${__result})
      set(__deps "${NEBULA_MODULE_${__d}_DEPS}")
      if(";${__deps};" MATCHES ";${m};")
        list(FIND __result "${__d}" __i)
        if(__i LESS "${__index}")
          set(__index "${__i}")
        endif()
      endif()
    endforeach()
    if(__index STREQUAL __lastindex)
      list(APPEND __result "${m}")
    else()
      list(INSERT __result ${__index} "${m}")
    endif()
  endforeach()
  set(${__lst} "${__result}" PARENT_SCOPE)
endfunction()

# resolve dependensies
function(__nebula_resolve_dependencies)
  foreach(m ${NEBULA_MODULES_DISABLED_USER})
    set(HAVE_${m} OFF CACHE INTERNAL "Module ${m} will not be built in current configuration")
  endforeach()
  foreach(m ${NEBULA_MODULES_BUILD})
    set(HAVE_${m} ON CACHE INTERNAL "Module ${m} will be built in current configuration")
  endforeach()

  # disable MODULES with unresolved dependencies
  set(has_changes ON)
  while(has_changes)
    set(has_changes OFF)
    foreach(m ${NEBULA_MODULES_BUILD})
      set(__deps ${NEBULA_MODULE_${m}_REQ_DEPS} ${NEBULA_MODULE_${m}_PRIVATE_REQ_DEPS})
      while(__deps)
        nebula_list_pop_front(__deps d)
        string(TOLOWER "${d}" upper_d)
        if(NOT (HAVE_${d} OR HAVE_${upper_d} OR TARGET ${d} OR EXISTS ${d}))
          if(d MATCHES "^Nebula") # TODO Remove this condition in the future and use HAVE_ variables only
            message(STATUS "Module ${m} disabled because ${d} dependency can't be resolved!")
            __nebula_module_turn_off(${m})
            set(has_changes ON)
            break()
          else()
            message(STATUS "Assume that non-module dependency is available: ${d} (for module ${m})")
          endif()
        endif()
      endwhile()
    endforeach()
  endwhile()

#  message(STATUS "List of active modules: ${NEBULA_MODULES_BUILD}")

  foreach(m ${NEBULA_MODULES_BUILD})
    set(deps_${m} ${NEBULA_MODULE_${m}_REQ_DEPS})
    foreach(d ${NEBULA_MODULE_${m}_OPT_DEPS})
      if(NOT (";${deps_${m}};" MATCHES ";${d};"))
        if(HAVE_${d} OR TARGET ${d})
          list(APPEND deps_${m} ${d})
        endif()
      endif()
    endforeach()
#    message(STATUS "Initial deps of ${m} (w/o private deps): ${deps_${m}}")
  endforeach()

  # propagate dependencies
  set(has_changes ON)
  while(has_changes)
    set(has_changes OFF)
    foreach(m2 ${NEBULA_MODULES_BUILD}) # transfer deps of m2 to m
      foreach(m ${NEBULA_MODULES_BUILD})
        if((NOT m STREQUAL m2) AND ";${deps_${m}};" MATCHES ";${m2};")
          foreach(d ${deps_${m2}})
            if(NOT (";${deps_${m}};" MATCHES ";${d};"))
#              message(STATUS "  Transfer dependency ${d} from ${m2} to ${m}")
              list(APPEND deps_${m} ${d})
              set(has_changes ON)
            endif()
          endforeach()
        endif()
      endforeach()
    endforeach()
  endwhile()

  # process private deps
  foreach(m ${NEBULA_MODULES_BUILD})
    foreach(d ${NEBULA_MODULE_${m}_PRIVATE_REQ_DEPS})
      if(NOT (";${deps_${m}};" MATCHES ";${d};"))
        list(APPEND deps_${m} ${d})
      endif()
    endforeach()
    foreach(d ${NEBULA_MODULE_${m}_PRIVATE_OPT_DEPS})
      if(NOT (";${deps_${m}};" MATCHES ";${d};"))
        if(HAVE_${d} OR TARGET ${d})
          list(APPEND deps_${m} ${d})
        endif()
      endif()
    endforeach()
  endforeach()

  nebula_list_sort(NEBULA_MODULES_BUILD)

  foreach(m ${NEBULA_MODULES_BUILD})
#    message(STATUS "FULL deps of ${m}: ${deps_${m}}")
    set(NEBULA_MODULE_${m}_DEPS ${deps_${m}})
    set(NEBULA_MODULE_${m}_DEPS_EXT ${deps_${m}})
    nebula_list_filterout(NEBULA_MODULE_${m}_DEPS_EXT "^Nebula[^ ]+$")
    if(NEBULA_MODULE_${m}_DEPS_EXT AND NEBULA_MODULE_${m}_DEPS)
      list(REMOVE_ITEM NEBULA_MODULE_${m}_DEPS ${NEBULA_MODULE_${m}_DEPS_EXT})
    endif()
  endforeach()

  # reorder dependencies
  foreach(m ${NEBULA_MODULES_BUILD})
    __nebula_sort_modules_by_deps(NEBULA_MODULE_${m}_DEPS)
    nebula_list_sort(NEBULA_MODULE_${m}_DEPS_EXT)

    set(NEBULA_MODULE_${m}_DEPS ${NEBULA_MODULE_${m}_DEPS} CACHE INTERNAL "Flattened dependencies of ${m} module")
    set(NEBULA_MODULE_${m}_DEPS_EXT ${NEBULA_MODULE_${m}_DEPS_EXT} CACHE INTERNAL "Extra dependencies of ${m} module")

#    message(STATUS "  module deps: ${NEBULA_MODULE_${m}_DEPS}")
#    message(STATUS "  extra deps: ${NEBULA_MODULE_${m}_DEPS_EXT}")
  endforeach()

  __nebula_sort_modules_by_deps(NEBULA_MODULES_BUILD)

  set(NEBULA_MODULES_PUBLIC        ${NEBULA_MODULES_PUBLIC}        CACHE INTERNAL "List of OpenCV modules marked for export")
  set(NEBULA_MODULES_BUILD         ${NEBULA_MODULES_BUILD}         CACHE INTERNAL "List of OpenCV modules included into the build")
  set(NEBULA_MODULES_DISABLED_AUTO ${NEBULA_MODULES_DISABLED_AUTO} CACHE INTERNAL "List of OpenCV modules implicitly disabled due to dependencies")
endfunction()


# setup include paths for the list of passed modules
macro(nebula_include_modules)
  foreach(d ${ARGN})
    if(d MATCHES "^Nebula" AND HAVE_${d})
      if (EXISTS "${NEBULA_MODULE_${d}_LOCATION}/include")
        nebula_include_directories("${NEBULA_MODULE_${d}_LOCATION}/include")
      endif()
    elseif(EXISTS "${d}")
      nebula_include_directories("${d}")
    endif()
  endforeach()
endmacro()

# setup include paths for the list of passed modules and recursively add dependent modules
macro(nebula_include_modules_recurse)
  foreach(d ${ARGN})
    if(d MATCHES "^Nebula" AND HAVE_${d})
      if (EXISTS "${NEBULA_MODULE_${d}_LOCATION}/include")
        nebula_include_directories("${NEBULA_MODULE_${d}_LOCATION}/include")
      endif()
      if(NEBULA_MODULE_${d}_DEPS)
        nebula_include_modules(${NEBULA_MODULE_${d}_DEPS})
      endif()
    elseif(EXISTS "${d}")
      nebula_include_directories("${d}")
    endif()
  endforeach()
endmacro()

# setup include path for OpenCV headers for specified module
# nebula_module_include_directories(<extra include directories/extra include modules>)
macro(nebula_module_include_directories)
  nebula_include_directories("${NEBULA_MODULE_${the_module}_LOCATION}/include"
                          "${NEBULA_MODULE_${the_module}_LOCATION}/src"
                          "${CMAKE_CURRENT_BINARY_DIR}" # for precompiled headers
                          )
  nebula_include_modules(${NEBULA_MODULE_${the_module}_DEPS} ${ARGN})
endmacro()


# sets header and source files for the current module
# NB: all files specified as headers will be installed
# Usage:
# nebula_set_module_sources([HEADERS] <list of files> [SOURCES] <list of files>)
macro(nebula_set_module_sources)
  set(NEBULA_MODULE_${the_module}_HEADERS "")
  set(NEBULA_MODULE_${the_module}_SOURCES "")

  foreach(f "HEADERS" ${ARGN})
    if(f STREQUAL "HEADERS" OR f STREQUAL "SOURCES")
      set(__filesvar "NEBULA_MODULE_${the_module}_${f}")
    else()
      list(APPEND ${__filesvar} "${f}")
    endif()
  endforeach()

  # the hacky way to embeed any files into the OpenCV without modification of its build system
  if(COMMAND nebula_get_module_external_sources)
    nebula_get_module_external_sources()
  endif()

  # use full paths for module to be independent from the module location
  nebula_convert_to_full_paths(NEBULA_MODULE_${the_module}_HEADERS)

  set(NEBULA_MODULE_${the_module}_HEADERS ${NEBULA_MODULE_${the_module}_HEADERS} CACHE INTERNAL "List of header files for ${the_module}")
  set(NEBULA_MODULE_${the_module}_SOURCES ${NEBULA_MODULE_${the_module}_SOURCES} CACHE INTERNAL "List of source files for ${the_module}")
endmacro()

# finds and sets headers and sources for the standard OpenCV module
# Usage:
# nebula_glob_module_sources(<extra sources&headers in the same format as used in nebula_set_module_sources>)
macro(nebula_glob_module_sources)
  file(GLOB_RECURSE lib_srcs "src/*.cpp")
  file(GLOB_RECURSE lib_int_hdrs "src/*.hpp" "src/*.h")
  file(GLOB lib_hdrs "include/opencv2/${name}/*.hpp" "include/opencv2/${name}/*.h")
  file(GLOB lib_hdrs_detail "include/opencv2/${name}/detail/*.hpp" "include/opencv2/${name}/detail/*.h")

  file(GLOB lib_cuda_srcs "src/cuda/*.cu")
  set(cuda_objs "")
  set(lib_cuda_hdrs "")
  if(HAVE_CUDA AND lib_cuda_srcs)
    nebula_include_directories(${CUDA_INCLUDE_DIRS})
    file(GLOB lib_cuda_hdrs "src/cuda/*.hpp")

    nebula_cuda_compile(cuda_objs ${lib_cuda_srcs} ${lib_cuda_hdrs})
    source_group("Src\\Cuda"      FILES ${lib_cuda_srcs} ${lib_cuda_hdrs})
  endif()

  source_group("Src" FILES ${lib_srcs} ${lib_int_hdrs})

  file(GLOB cl_kernels "src/opencl/*.cl")
  if(HAVE_opencv_ocl AND cl_kernels)
    nebula_include_directories(${OPENCL_INCLUDE_DIRS})
    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/opencl_kernels.cpp" "${CMAKE_CURRENT_BINARY_DIR}/opencl_kernels.hpp"
      COMMAND ${CMAKE_COMMAND} -DCL_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src/opencl" -DOUTPUT="${CMAKE_CURRENT_BINARY_DIR}/opencl_kernels.cpp" -P "${OpenCV_SOURCE_DIR}/cmake/cl2cpp.cmake"
      DEPENDS ${cl_kernels} "${OpenCV_SOURCE_DIR}/cmake/cl2cpp.cmake")
    source_group("OpenCL" FILES ${cl_kernels} "${CMAKE_CURRENT_BINARY_DIR}/opencl_kernels.cpp" "${CMAKE_CURRENT_BINARY_DIR}/opencl_kernels.hpp")
    list(APPEND lib_srcs ${cl_kernels} "${CMAKE_CURRENT_BINARY_DIR}/opencl_kernels.cpp" "${CMAKE_CURRENT_BINARY_DIR}/opencl_kernels.hpp")
  endif()

  source_group("Include" FILES ${lib_hdrs})
  source_group("Include\\detail" FILES ${lib_hdrs_detail})

  nebula_set_module_sources(${ARGN} HEADERS ${lib_hdrs} ${lib_hdrs_detail}
                                 SOURCES ${lib_srcs} ${lib_int_hdrs} ${cuda_objs} ${lib_cuda_srcs} ${lib_cuda_hdrs})
endmacro()

# creates OpenCV module in current folder
# creates new target, configures standard dependencies, compilers flags, install rules
# Usage:
#   nebula_create_module(<extra link dependencies>)
#   nebula_create_module(SKIP_LINK)
macro(nebula_create_module)
  # The condition we ought to be testing here is whether nebula_add_precompiled_headers will
  # be called at some point in the future. We can't look into the future, though,
  # so this will have to do.
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/precomp.hpp")
    get_native_precompiled_header(${the_module} precomp.hpp)
  endif()

  add_library(${the_module} ${NEBULA_MODULE_TYPE} ${NEBULA_MODULE_${the_module}_HEADERS} ${NEBULA_MODULE_${the_module}_SOURCES}
    "${NEBULA_CONFIG_FILE_INCLUDE_DIR}/cvconfig.h" "${NEBULA_CONFIG_FILE_INCLUDE_DIR}/opencv2/opencv_modules.hpp"
    ${${the_module}_pch})

  if(NOT "${ARGN}" STREQUAL "SKIP_LINK")
    target_link_libraries(${the_module} ${NEBULA_MODULE_${the_module}_DEPS})
    target_link_libraries(${the_module} LINK_INTERFACE_LIBRARIES ${NEBULA_MODULE_${the_module}_DEPS})
    target_link_libraries(${the_module} ${NEBULA_MODULE_${the_module}_DEPS_EXT} ${NEBULA_LINKER_LIBS} ${IPP_LIBS} ${ARGN})
    if (HAVE_CUDA)
      target_link_libraries(${the_module} ${CUDA_LIBRARIES} ${CUDA_npp_LIBRARY})
    endif()
  endif()

  add_dependencies(opencv_modules ${the_module})

  if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${the_module} PROPERTIES FOLDER "modules")
  endif()

  set_target_properties(${the_module} PROPERTIES
    OUTPUT_NAME "${the_module}${NEBULA_DLLVERSION}"
    DEBUG_POSTFIX "${NEBULA_DEBUG_POSTFIX}"
    ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}
    RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    INSTALL_NAME_DIR lib
  )

  # For dynamic link numbering convenions
  if(NOT ANDROID)
    # Android SDK build scripts can include only .so files into final .apk
    # As result we should not set version properties for Android
    set_target_properties(${the_module} PROPERTIES
      VERSION ${NEBULA_LIBVERSION}
      SOVERSION ${NEBULA_SOVERSION}
    )
  endif()

  if((NOT DEFINED NEBULA_MODULE_TYPE AND BUILD_SHARED_LIBS)
      OR (DEFINED NEBULA_MODULE_TYPE AND NEBULA_MODULE_TYPE STREQUAL SHARED))
    set_target_properties(${the_module} PROPERTIES DEFINE_SYMBOL CVAPI_EXPORTS)
  endif()

  if(MSVC)
    if(CMAKE_CROSSCOMPILING)
      set_target_properties(${the_module} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:secchk")
    endif()
    set_target_properties(${the_module} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:libc /DEBUG")
  endif()

  nebula_install_target(${the_module} EXPORT OpenCVModules
    RUNTIME DESTINATION ${NEBULA_BIN_INSTALL_PATH} COMPONENT main
    LIBRARY DESTINATION ${NEBULA_LIB_INSTALL_PATH} COMPONENT main
    ARCHIVE DESTINATION ${NEBULA_LIB_INSTALL_PATH} COMPONENT main
    )

  # only "public" headers need to be installed
  if(NEBULA_MODULE_${the_module}_HEADERS AND ";${NEBULA_MODULES_PUBLIC};" MATCHES ";${the_module};")
    foreach(hdr ${NEBULA_MODULE_${the_module}_HEADERS})
      string(REGEX REPLACE "^.*opencv2/" "opencv2/" hdr2 "${hdr}")
      if(hdr2 MATCHES "^(opencv2/.*)/[^/]+.h(..)?$")
        install(FILES ${hdr} DESTINATION "${NEBULA_INCLUDE_INSTALL_PATH}/${CMAKE_MATCH_1}" COMPONENT main)
      endif()
    endforeach()
  endif()
endmacro()

# opencv precompiled headers macro (can add pch to modules and tests)
# this macro must be called after any "add_definitions" commands, otherwise precompiled headers will not work
# Usage:
# nebula_add_precompiled_headers(${the_module})
macro(nebula_add_precompiled_headers the_target)
  if("${the_target}" MATCHES "^Nebulatest_.*$")
    SET(pch_path "test/test_")
  elseif("${the_target}" MATCHES "^Nebulaperf_.*$")
    SET(pch_path "perf/perf_")
  else()
    SET(pch_path "src/")
  endif()
  nebula_add_precompiled_header_to_target(${the_target} "${CMAKE_CURRENT_SOURCE_DIR}/${pch_path}precomp.hpp")
  unset(pch_path)
endmacro()

# short command for adding simple OpenCV module
# see nebula_add_module for argument details
# Usage:
# nebula_define_module(module_name  [INTERNAL] [REQUIRED] [<list of dependencies>] [OPTIONAL <list of optional dependencies>])
macro(nebula_define_module module_name)
  nebula_add_module(${module_name} ${ARGN})
  nebula_module_include_directories()
  nebula_glob_module_sources()
  nebula_create_module()
  nebula_add_precompiled_headers(${the_module})

  nebula_add_accuracy_tests()
  nebula_add_perf_tests()
  nebula_add_samples()
endmacro()

# ensures that all passed modules are available
# sets OCV_DEPENDENCIES_FOUND variable to TRUE/FALSE
macro(nebula_check_dependencies)
  set(OCV_DEPENDENCIES_FOUND TRUE)
  foreach(d ${ARGN})
    if(d MATCHES "^Nebula[^ ]+$" AND NOT HAVE_${d})
      set(OCV_DEPENDENCIES_FOUND FALSE)
      break()
    endif()
  endforeach()
endmacro()

# auxiliary macro to parse arguments of nebula_add_accuracy_tests and nebula_add_perf_tests commands
macro(__nebula_parse_test_sources tests_type)
  set(NEBULA_${tests_type}_${the_module}_SOURCES "")
  set(NEBULA_${tests_type}_${the_module}_DEPS "")
  set(__file_group_name "")
  set(__file_group_sources "")
  foreach(arg "DEPENDS_ON" ${ARGN} "FILES")
    if(arg STREQUAL "FILES")
      set(__currentvar "__file_group_sources")
      if(__file_group_name AND __file_group_sources)
        source_group("${__file_group_name}" FILES ${__file_group_sources})
        list(APPEND NEBULA_${tests_type}_${the_module}_SOURCES ${__file_group_sources})
      endif()
      set(__file_group_name "")
      set(__file_group_sources "")
    elseif(arg STREQUAL "DEPENDS_ON")
      set(__currentvar "NEBULA_TEST_${the_module}_DEPS")
    elseif("${__currentvar}" STREQUAL "__file_group_sources" AND NOT __file_group_name)
      set(__file_group_name "${arg}")
    else()
      list(APPEND ${__currentvar} "${arg}")
    endif()
  endforeach()
  unset(__file_group_name)
  unset(__file_group_sources)
  unset(__currentvar)
endmacro()

# this is a command for adding OpenCV performance tests to the module
# nebula_add_perf_tests(<extra_dependencies>)
function(nebula_add_perf_tests)
  set(perf_path "${CMAKE_CURRENT_SOURCE_DIR}/perf")
  if(BUILD_PERF_TESTS AND EXISTS "${perf_path}")
    __nebula_parse_test_sources(PERF ${ARGN})

    # opencv_highgui is required for imread/imwrite
    set(perf_deps ${the_module} opencv_ts opencv_highgui ${NEBULA_PERF_${the_module}_DEPS} ${NEBULA_MODULE_opencv_ts_DEPS})
    nebula_check_dependencies(${perf_deps})

    if(OCV_DEPENDENCIES_FOUND)
      set(the_target "opencv_perf_${name}")
      # project(${the_target})

      nebula_module_include_directories(${perf_deps} "${perf_path}")

      if(NOT NEBULA_PERF_${the_module}_SOURCES)
        file(GLOB perf_srcs "${perf_path}/*.cpp")
        file(GLOB perf_hdrs "${perf_path}/*.hpp" "${perf_path}/*.h")
        source_group("Src" FILES ${perf_srcs})
        source_group("Include" FILES ${perf_hdrs})
        set(NEBULA_PERF_${the_module}_SOURCES ${perf_srcs} ${perf_hdrs})
      endif()

      get_native_precompiled_header(${the_target} perf_precomp.hpp)

      add_executable(${the_target} ${NEBULA_PERF_${the_module}_SOURCES} ${${the_target}_pch})
      target_link_libraries(${the_target} ${NEBULA_MODULE_${the_module}_DEPS} ${perf_deps} ${NEBULA_LINKER_LIBS})
      add_dependencies(opencv_perf_tests ${the_target})

      # Additional target properties
      set_target_properties(${the_target} PROPERTIES
        DEBUG_POSTFIX "${NEBULA_DEBUG_POSTFIX}"
        RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
      )

      if(ENABLE_SOLUTION_FOLDERS)
        set_target_properties(${the_target} PROPERTIES FOLDER "tests performance")
      endif()

      nebula_add_precompiled_headers(${the_target})

    else(OCV_DEPENDENCIES_FOUND)
      # TODO: warn about unsatisfied dependencies
    endif(OCV_DEPENDENCIES_FOUND)
  endif()
endfunction()

# this is a command for adding OpenCV accuracy/regression tests to the module
# nebula_add_accuracy_tests([FILES <source group name> <list of sources>] [DEPENDS_ON] <list of extra dependencies>)
function(nebula_add_accuracy_tests)
  set(test_path "${CMAKE_CURRENT_SOURCE_DIR}/test")
  nebula_check_dependencies(${test_deps})
  if(BUILD_TESTS AND EXISTS "${test_path}")
    __nebula_parse_test_sources(TEST ${ARGN})

    # opencv_highgui is required for imread/imwrite
    set(test_deps ${the_module} opencv_ts opencv_highgui ${NEBULA_TEST_${the_module}_DEPS} ${NEBULA_MODULE_opencv_ts_DEPS})
    nebula_check_dependencies(${test_deps})

    if(OCV_DEPENDENCIES_FOUND)
      set(the_target "opencv_test_${name}")
      # project(${the_target})

      nebula_module_include_directories(${test_deps} "${test_path}")

      if(NOT NEBULA_TEST_${the_module}_SOURCES)
        file(GLOB test_srcs "${test_path}/*.cpp")
        file(GLOB test_hdrs "${test_path}/*.hpp" "${test_path}/*.h")
        source_group("Src" FILES ${test_srcs})
        source_group("Include" FILES ${test_hdrs})
        set(NEBULA_TEST_${the_module}_SOURCES ${test_srcs} ${test_hdrs})
      endif()

      get_native_precompiled_header(${the_target} test_precomp.hpp)

      add_executable(${the_target} ${NEBULA_TEST_${the_module}_SOURCES} ${${the_target}_pch})
      target_link_libraries(${the_target} ${NEBULA_MODULE_${the_module}_DEPS} ${test_deps} ${NEBULA_LINKER_LIBS})
      add_dependencies(opencv_tests ${the_target})

      # Additional target properties
      set_target_properties(${the_target} PROPERTIES
        DEBUG_POSTFIX "${NEBULA_DEBUG_POSTFIX}"
        RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
      )

      if(ENABLE_SOLUTION_FOLDERS)
        set_target_properties(${the_target} PROPERTIES FOLDER "tests accuracy")
      endif()

      enable_testing()
      get_target_property(LOC ${the_target} LOCATION)
      add_test(${the_target} "${LOC}")

      nebula_add_precompiled_headers(${the_target})
    else(OCV_DEPENDENCIES_FOUND)
      # TODO: warn about unsatisfied dependencies
    endif(OCV_DEPENDENCIES_FOUND)
  endif()
endfunction()

function(nebula_add_samples)
  set(samples_path "${CMAKE_CURRENT_SOURCE_DIR}/samples")
  string(REGEX REPLACE "^Nebula" "" module_id ${the_module})

  if(BUILD_EXAMPLES AND EXISTS "${samples_path}")
    set(samples_deps ${the_module} ${NEBULA_MODULE_${the_module}_DEPS} opencv_highgui ${ARGN})
    nebula_check_dependencies(${samples_deps})

    if(OCV_DEPENDENCIES_FOUND)
      file(GLOB sample_sources "${samples_path}/*.cpp")
      nebula_include_modules(${NEBULA_MODULE_${the_module}_DEPS})

      foreach(source ${sample_sources})
        get_filename_component(name "${source}" NAME_WE)
        set(the_target "example_${module_id}_${name}")

        add_executable(${the_target} "${source}")
        target_link_libraries(${the_target} ${samples_deps})

        set_target_properties(${the_target} PROPERTIES PROJECT_LABEL "(sample) ${name}")

        if(ENABLE_SOLUTION_FOLDERS)
          set_target_properties(${the_target} PROPERTIES
            OUTPUT_NAME "${module_id}-example-${name}"
            FOLDER "samples/${module_id}")
        endif()

        if(WIN32)
          install(TARGETS ${the_target} RUNTIME DESTINATION "samples/${module_id}" COMPONENT main)
        endif()
      endforeach()
    endif()
  endif()

  if(INSTALL_C_EXAMPLES AND NOT WIN32 AND EXISTS "${samples_path}")
    file(GLOB sample_files "${samples_path}/*")
    install(FILES ${sample_files}
            DESTINATION share/OpenCV/samples/${module_id}
            PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
  endif()
endfunction()

# internal macro; finds all link dependencies of the module
# should be used at the end of CMake processing
macro(__nebula_track_module_link_dependencies the_module optkind)
  set(${the_module}_MODULE_DEPS_${optkind}   "")
  set(${the_module}_EXTRA_DEPS_${optkind}    "")

  get_target_property(__module_type ${the_module} TYPE)
  if(__module_type STREQUAL "STATIC_LIBRARY")
    #in case of static library we have to inherit its dependencies (in right order!!!)
    if(NOT DEFINED ${the_module}_LIB_DEPENDS_${optkind})
      nebula_split_libs_list(${the_module}_LIB_DEPENDS ${the_module}_LIB_DEPENDS_DBG ${the_module}_LIB_DEPENDS_OPT)
    endif()

    set(__resolved_deps "")
    set(__mod_depends ${${the_module}_LIB_DEPENDS_${optkind}})
    set(__has_cycle FALSE)

    while(__mod_depends)
      list(GET __mod_depends 0 __dep)
      list(REMOVE_AT __mod_depends 0)
      if(__dep STREQUAL the_module)
        set(__has_cycle TRUE)
      else()#if("${NEBULA_MODULES_BUILD}" MATCHES "(^|;)${__dep}(;|$)")
        nebula_regex_escape(__rdep "${__dep}")
        if(__resolved_deps MATCHES "(^|;)${__rdep}(;|$)")
          #all dependencies of this module are already resolved
          list(APPEND ${the_module}_MODULE_DEPS_${optkind} "${__dep}")
        else()
          get_target_property(__module_type ${__dep} TYPE)
          if(__module_type STREQUAL "STATIC_LIBRARY")
            if(NOT DEFINED ${__dep}_LIB_DEPENDS_${optkind})
              nebula_split_libs_list(${__dep}_LIB_DEPENDS ${__dep}_LIB_DEPENDS_DBG ${__dep}_LIB_DEPENDS_OPT)
            endif()
            list(INSERT __mod_depends 0 ${${__dep}_LIB_DEPENDS_${optkind}} ${__dep})
            list(APPEND __resolved_deps "${__dep}")
          elseif(NOT __module_type)
            list(APPEND  ${the_module}_EXTRA_DEPS_${optkind} "${__dep}")
          endif()
        endif()
      #else()
       # get_target_property(__dep_location "${__dep}" LOCATION)
      endif()
    endwhile()

    nebula_list_unique(${the_module}_MODULE_DEPS_${optkind})
    #nebula_list_reverse(${the_module}_MODULE_DEPS_${optkind})
    nebula_list_unique(${the_module}_EXTRA_DEPS_${optkind})
    #nebula_list_reverse(${the_module}_EXTRA_DEPS_${optkind})

    if(__has_cycle)
      # not sure if it can work
      list(APPEND ${the_module}_MODULE_DEPS_${optkind} "${the_module}")
    endif()

    unset(__dep_location)
    unset(__mod_depends)
    unset(__resolved_deps)
    unset(__has_cycle)
    unset(__rdep)
  endif()#STATIC_LIBRARY
  unset(__module_type)

  #message("${the_module}_MODULE_DEPS_${optkind}")
  #message("       ${${the_module}_MODULE_DEPS_${optkind}}")
  #message("       ${NEBULA_MODULE_${the_module}_DEPS}")
  #message("")
  #message("${the_module}_EXTRA_DEPS_${optkind}")
  #message("       ${${the_module}_EXTRA_DEPS_${optkind}}")
  #message("")
endmacro()

# creates lists of build dependencies needed for external projects
macro(nebula_track_build_dependencies)
  foreach(m ${NEBULA_MODULES_BUILD})
    __nebula_track_module_link_dependencies("${m}" OPT)
    __nebula_track_module_link_dependencies("${m}" DBG)
  endforeach()
endmacro()
