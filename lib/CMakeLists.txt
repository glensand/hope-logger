cmake_minimum_required(VERSION 3.11)

project(hope_logger)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3 -fPIC" )
set( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -O3 -fPIC" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

file(GLOB CL_HEADERS
    hope_logger/*.h
    hope_logger/misc/*.h
)

file(GLOB CL_SOURCES
    hope_logger/*.cpp
)

add_library(hope_logger STATIC ${CL_SOURCES})
target_include_directories(hope_logger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
