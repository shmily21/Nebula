set(the_description "The Server Application")
# 添加源代码文件
aux_source_directory(src NEBULA_SERVER_SOURCE_CODE)
# 设置目标名称
set(NEBULA_SERVER_APPLICATION server)

if(UNIX)
    add_definitions(-DNEBULA_FOR_LINUX)
elseif(WIN32)
    add_definitions(-D_WIN32)
    add_definitions(-DNEBULA_FOR_WINDOWS)
endif(UNIX)

#添加包含路径
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# 编译目标
add_executable(${NEBULA_SERVER_APPLICATION}  ${NEBULA_SERVER_SOURCE_CODE})

# 编译器标准
if(HAVE_COMPILER_STD_FLAG_GUN99_SUPPORT)
    set(CMAKE_C_FLAG "${CMAKE_C_FLAGS} ${NEBULA_COMPILER_STD_FLAG_GUN99}")
endif(HAVE_COMPILER_STD_FLAG_GUN99_SUPPORT)


if(WIN32 OR WIN64)
    # 指定 Windows 子系统
    set_target_properties(${NEBULA_SERVER_APPLICATION} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

    #set_target_properties(${NEBULA_SERVER_APPLICATION} PROPERTIES LINK_FLAGS
    #                        "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"
    #                        /SUBSYSTEM:WINDOWS")

    # 指定程序的入口函数
    #set_target_properties(${NEBULA_SERVER_APPLICATION} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")

endif(WIN32)