cmake_minimum_required(VERSION 3.12)

project(LuaWatch LANGUAGES C VERSION 0.1.1)

add_library(lwlib SHARED)

set(LWLIB_SOURCES
	src/lwdebug.c src/lua_ext.c
	src/hashmap.c src/hashmap.h
	src/rbtree.c  src/rbtree.h)
set(LWLIB_HEADERS src/lwdebug.h src/lua_ext.h src/lwlog.h)

target_sources(lwlib PRIVATE ${LWLIB_SOURCES} ${LWLIB_HEADERS})
set_target_properties(lwlib PROPERTIES PUBLIC_HEADER "${LWLIB_HEADERS}")

if(UNIX)
	target_link_libraries(lwlib lua5.3 m dl)
	target_include_directories(lwlib PRIVATE "/usr/include/lua5.3")
endif()

add_library(lwlua SHARED)

set(LWLUA_SOURCES src/lwlua.c)
set(LWLUA_HEADERS src/lwlua.h)

target_sources(lwlua PRIVATE ${LWLUA_SOURCES} ${LWLUA_HEADERS})
set_target_properties(lwlua PROPERTIES PUBLIC_HEADER "${LWLUA_HEADERS}")

target_link_libraries(lwlua lwlib)

if(UNIX)
	set_target_properties(lwlua PROPERTIES PREFIX "")
	target_include_directories(lwlua PRIVATE "/usr/include/lua5.3")
endif()


# tests 

add_executable(rbtree_test tests/rbtree_test.c src/rbtree.c src/rbtree.h)

add_executable(lw_test tests/lw_test.c)
target_link_libraries(lw_test lwlib)
if(UNIX)
	target_include_directories(lw_test PRIVATE "/usr/include/lua5.3")
endif()

add_executable(lwlua_test tests/lwlua_test.c)
target_link_libraries(lwlua_test lwlua)
if(UNIX)
	target_include_directories(lwlua_test PRIVATE "/usr/include/lua5.3")
endif()


install(TARGETS lwlib lwlua lw_test
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		PUBLIC_HEADER DESTINATION include
)
