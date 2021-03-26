cmake_minimum_required(VERSION 3.16)

project(
    libflow
    VERSION 0.1
    LANGUAGES CXX
    DESCRIPTION "dataflow library for C++"
)

message("adding compile options")
add_compile_options(
    -Wall 
    -Wextra 
    -Werror 
    -pedantic
)

message("C++17 standard")
set(CMAKE_CXX_STANDARD 17)

message("loading headers")
include_directories(hdrs)

file(GLOB LIBFLOW_SOURCE_FILES src/*.cc)

add_library(flow SHARED ${LIBFLOW_SOURCE_FILES})

list(APPEND MY_TESTS 
    synchronous_flow_test
    asynchronous_flow_test
)

foreach(TARGET_TEST ${MY_TESTS})
    message("building target ${TARGET_TEST}")
    add_executable(${TARGET_TEST} tests/${TARGET_TEST}.cc)
    target_link_libraries(${TARGET_TEST} gtest flow pthread)
    add_test(${TARGET_TEST} ${TARGET_TEST})
endforeach()

message("Enabling testing")
enable_testing()
