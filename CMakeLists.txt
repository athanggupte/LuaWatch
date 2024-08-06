cmake_minimum_required(VERSION 3.12)

project(LuaWatch LANGUAGES C VERSION 0.1.1)

add_library(lwlib SHARED)

set(LWLIB_SOURCES lwdebug.c lua_ext.c hashmap.c)
set(LWLIB_HEADERS lwdebug.h lua_ext.h hashmap.h lwlog.h)

target_sources(lwlib PRIVATE ${LWLIB_SOURCES} ${LWLIB_HEADERS})

if(UNIX)
	target_link_libraries(lwlib lua5.3 m dl)
	target_include_directories(lwlib PRIVATE "/usr/include/lua5.3")
endif()

add_library(lwlua SHARED)

set(LWLUA_SOURCES lwlua.c)
set(LWLUA_HEADERS lwlua.h)

target_sources(lwlua PRIVATE ${LWLUA_SOURCES} ${LWLUA_HEADERS})
target_link_libraries(lwlua lwlib)

if(UNIX)
	set_target_properties(lwlua PROPERTIES PREFIX "")
	target_include_directories(lwlua PRIVATE "/usr/include/lua5.3")
endif()

add_executable(rbtree_test tests/rbtree_test.c rbtree.c rbtree.h)

add_executable(lw_test tests/lw_test.c)
target_link_libraries(lw_test lwlib)

if(UNIX)
	target_include_directories(lw_test PRIVATE "/usr/include/lua5.3")
endif()

install(TARGETS lwlib lwlua lw_test
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
)
