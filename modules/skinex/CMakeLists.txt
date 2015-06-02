set(the_description "The SkinEx Library")
# 添加源代码文件
file(GLOB_RECURSE SOURCE_HEADER
    include/*.h *h)

aux_source_directory(src NEBULA_SKINEX_SOURCE_CODE)
# 设置目标名称
set(NEBULA_SKINEX_LIBRARY skinex)

if(UNIX)
    add_definitions(-DNEBULA_FOR_LINUX)
elseif(WIN32)
    add_definitions(-D_WIN32)
    add_definitions(-DNEBULA_FOR_WINDOWS)
endif(UNIX)

#添加包含路径
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

source_group("Header Files" FILES ${SOURCE_HEADER})
source_group("Source Files" FILES ${NEBULA_SKINEX_SOURCE_CODE})
# 编译目标
add_library(${NEBULA_SKINEX_LIBRARY} SHARED ${NEBULA_SKINEX_SOURCE_CODE} ${SOURCE_HEADER})

# 编译器标准
if(HAVE_COMPILER_STD_FLAG_GUN99_SUPPORT)
    set(CMAKE_C_FLAG "${CMAKE_C_FLAGS} ${NEBULA_COMPILER_STD_FLAG_GUN99}")
endif(HAVE_COMPILER_STD_FLAG_GUN99_SUPPORT)


if(WIN32 OR WIN64)
    # 指定 Windows 子系统
    #set_target_properties(${NEBULA_SKINEX_LIBRARY} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

endif(WIN32 OR WIN64)