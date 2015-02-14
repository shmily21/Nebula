set(the_description "The Zlib Library")

set(NEBULA_ZLIB_LIBRARY Zlib)

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)
include(CheckTypeSize)

#
# Check for fseeko
#
check_function_exists(fseeko HAVE_FSEEKO)
if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
endif()

#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

#
# Check to see if we have large file support
#
check_type_size(off64_t OFF64_T)
if(HAVE_OFF64_T)
    add_definitions(-D_LARGEFILE64_SOURCE=1)
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/zconf.h.cmakein"
                "${CMAKE_CURRENT_BINARY_DIR}/zconf.h" @ONLY)
nebula_include_directories("${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

set(ZLIB_PUBLIC_HDRS
    "${CMAKE_CURRENT_BINARY_DIR}/zconf.h"
    zlib.h
)
set(ZLIB_PRIVATE_HDRS
    crc32.h
    deflate.h
    gzguts.h
    inffast.h
    inffixed.h
    inflate.h
    inftrees.h
    trees.h
    zutil.h
)
set(ZLIB_SRCS
    adler32.c
    compress.c
    crc32.c
    deflate.c
    gzclose.c
    gzlib.c
    gzread.c
    gzwrite.c
    inflate.c
    infback.c
    inftrees.c
    inffast.c
    trees.c
    uncompr.c
    zutil.c
)

set(BUILD_TYPE_ZLIB "SHARED")

if(BUILD_STATIC_ZLIB)
    set(BUILD_TYPE_ZLIB "STATIC")
endif(BUILD_STATIC_ZLIB)

add_library(${NEBULA_ZLIB_LIBRARY} ${BUILD_TYPE_ZLIB} ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
set_target_properties(${NEBULA_ZLIB_LIBRARY} PROPERTIES DEFINE_SYMBOL ZLIB_DLL)

if(UNIX)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    endif()
endif()

nebula_warnings_disable(CMAKE_C_FLAGS -Wattributes -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations)

set_target_properties(${NEBULA_ZLIB_LIBRARY} PROPERTIES
        OUTPUT_NAME ${NEBULA_ZLIB_LIBRARY}
        ARCHIVE_OUTPUT_DIRECTORY ${3P_LIBRARY_OUTPUT_PATH}
    )

if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${NEBULA_ZLIB_LIBRARY} PROPERTIES FOLDER "3rdparty")
endif()
