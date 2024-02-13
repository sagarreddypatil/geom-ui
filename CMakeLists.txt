cmake_minimum_required(VERSION 3.28)
project(geomui)

# include eigen files, don't use find_package
set(EIGEN3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/eigen)

set(CMAKE_CXX_STANDARD 23)

set(LIBRARY_SOURCES
    src/solver/solver.cpp
)

add_library(${PROJECT_NAME} STATIC ${LIBRARY_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE ${EIGEN3_INCLUDE_DIR})

# if BUILD_EXAMPLES is set, build examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
