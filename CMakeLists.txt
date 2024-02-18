cmake_minimum_required(VERSION 3.28)
project(geomui)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

# include eigen files, don't use find_package
set(EIGEN3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/eigen)

set(CMAKE_CXX_STANDARD 23)

set(LIBRARY_SOURCES
    src/solver.cpp
    src/ui.cpp
    src/construction.cpp
)

set(MY_BUILD_EXAMPLES ${BUILD_EXAMPLES})

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/raylib)

set(BUILD_EXAMPLES ${MY_BUILD_EXAMPLES} CACHE BOOL "" FORCE)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -Wno-unused-function")

add_library(${PROJECT_NAME} STATIC ${LIBRARY_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN3_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC lib/raylib/src) # let library users include raylib headers

target_link_libraries(${PROJECT_NAME} raylib)

# if BUILD_EXAMPLES is set, build examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
