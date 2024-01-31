cmake_minimum_required(VERSION 3.11)

project(hope_logger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(CMAKE_INSTALL_DIRECTORY ${CMAKE_BINARY_DIR})

INSTALL(DIRECTORY hope_logger/ DESTINATION ${CMAKE_INSTALL_DIRECTORY}/include/hope_logger
        FILES_MATCHING PATTERN "*.h")
