cmake_minimum_required(VERSION 3.16)

project(
    libflow
    VERSION 0.1
    LANGUAGES CXX
    DESCRIPTION "dataflow library for C++"
)

add_compile_options(
    -Wall 
    -Wextra 
    -Werror 
    -pedantic
)

set(CMAKE_CXX_STANDARD 17)

include_directories(hdrs)

file(GLOB LIBFLOW_SRC_FILES src/*.cc)

add_library(flow SHARED ${LIBFLOW_SRC_FILES})

target_link_libraries(flow pthread)

list(APPEND MY_TESTS 
    basic_circuit_test
)

foreach(TARGET_TEST ${MY_TESTS})
    add_executable(${TARGET_TEST} tests/${TARGET_TEST}.cc)
    target_link_libraries(${TARGET_TEST} flow)
    add_test(${TARGET_TEST} ${TARGET_TEST})
endforeach()

message("Enabling testing")

enable_testing()
