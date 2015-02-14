#
# 编译lua
#---------------------------------------------
NEBULA_OPTION(BUILD_STATIC_LUA          "Create Static Lua Library"         ON)
add_subdirectory(lua)

#
#
#---------------------------------------------
NEBULA_OPTION(BUILD_STATIC_ZLIB         "Create Shared Zlib Library"        ON)
add_subdirectory(zlib)

#
#
#---------------------------------------------
NEBULA_OPTION(BUILD_STATIC_SQLITE3      "Create Shared SQLite3 Library"     ON)
add_subdirectory(sqlite3)

#
#
#---------------------------------------------
NEBULA_OPTION(BUILD_STATIC_BULLET       "Create Shared Bullet Physic Engine"    ON)
add_subdirectory(bullet)

