set(the_description "The Lua Library")
aux_source_directory(src NEBULA_LUA_SOURCE_CODE)

set(NEBULA_LUA_LIBRARY lua)

# if(WIN32 OR WIN64)
#     set(NEBULA_LUA_LIBRARY lua)
# elseif(UNIX)
#     set(NEBULA_LUA_LIBRARY lua)
# endif(WIN32 OR WIN64)

#project(${NEBULA_LUA_LIBRARY} C)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)
include(CheckTypeSize)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
endif()

if(WIN32 OR WIN64)
    add_definitions(-DLUA_COMPAT_5_2)
endif()

#nebula_include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

set(BUILD_TYPE_LUA "SHARED")

if(BUILD_STATIC_LUA)
    set(BUILD_TYPE_LUA  "STATIC")
endif(BUILD_STATIC_LUA)

add_library(${NEBULA_LUA_LIBRARY} ${BUILD_TYPE_LUA} ${NEBULA_LUA_SOURCE_CODE})
set_target_properties(${NEBULA_LUA_LIBRARY} PROPERTIES DEFINE_SYMBOL -DLUA_BUILD_AS_DLL)

if(UNIX)
    if(CMAKE_COMPILER_IS_GUNCXX)
        set(CMAKE_C_FLAG "${CMAKE_C_FLAGS} -fPIC")
    endif(CMAKE_COMPILER_IS_GUNCXX)
endif(UNIX)

nebula_warnings_disable(CMAKE_C_FLAGS -Wextra)

set_target_properties(${NEBULA_LUA_LIBRARY} PROPERTIES
        OUTPUT_NAME ${NEBULA_LUA_LIBRARY}
        ARCHIVE_OUTPUT_DIRECTORY ${3P_LIBRARY_OUTPUT_PATH}
    )

if(ENABLE_SOLUTION_FOLDERS)
  set_target_properties(${NEBULA_LUA_LIBRARY} PROPERTIES FOLDER "3rdparty")
endif()


