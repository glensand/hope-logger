cmake_minimum_required(VERSION 3.20)

project(sample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SAMPLE_SOURSES
    *.cpp)

add_executable(sample ${SAMPLE_SOURSES})
target_link_libraries(sample hope_logger)

target_include_directories(sample PUBLIC ../lib)

