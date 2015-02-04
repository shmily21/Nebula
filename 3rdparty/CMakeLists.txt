#设置第三方库输出路径
set(3P_LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/App/bin2" CACHE PATH "Output directory for the third part library" )
#设置模块输出路径
#set(MODULE_LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/App/bin" CACHE PATH "Output directory for library file")

#set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/App/tool" CACHE PATH "Output directory for binary" )

#set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/App/libs" CACHE PATH "Output directory for application")


NEBULA_OPTION(BUILD_STATIC_LUA          "Create Static Lua Library"     OFF)

#
# 编译lua
#---------------------------------------------
add_subdirectory(lua)