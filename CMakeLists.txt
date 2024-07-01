cmake_minimum_required(VERSION 3.7)
project(chttpd
    VERSION 2.0.0
    LANGUAGES C
)


# GCC and it's flags
set(CMAKE_C_COMPILER gcc)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fms-extensions")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_GNU_SOURCE=1")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")


# Build type and debug symbols
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Include debug symbols" FORCE)
endif()
if(CMAKE_BUILD_TYPE!=Release)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
endif()


# # Suppress the warning: Policy CMP0076 is not set: target_sources() command 
# # converts relative paths to absolute. 
# cmake_policy(SET CMP0076 NEW)


set(CHTTPD_IOMONITORING "epoll" CACHE STRING "chttpd IO monitoring system.")
set_property(CACHE CHTTPD_IOMONITORING PROPERTY STRINGS "epoll" "select")

option(CHTTPD_SERVER "Enable chttpd built-in server." ON)


if(CHTTPD_SERVER)
add_library(server OBJECT server.c server.h)

add_executable(chttpd 
	server.c 
	server.h 
	# $<TARGET_OBJECTS:server>
)

target_link_libraries(chttpd caio)

endif()


# Menu config
add_custom_target(menu COMMAND ccmake ${PROJECT_BINARY_DIR} )
add_custom_target(fresh COMMAND cmake ${PROJECT_SOURCE_DIR} --fresh)


# Lint
set(PRETTYC_FLAGS
    --recursive
    --verbose=0
    --repository=.
    --extensions=c,h,in
    --linelength=80
    --headers=h,in
    --includeorder=standardcfirst
    --root=.
    #--quiet
    ${PROJECT_SOURCE_DIR}
)
add_custom_target(lint
    COMMAND prettyc
    ${PRETTYC_FLAGS}
)


# Examples
if (BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()


# Tests
if (BUILD_TESTS})
    enable_testing()
    # list(APPEND CMAKE_CTEST_ARGUMENTS "--verbose")
    add_subdirectory(tests)
    list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
endif()
