cmake_minimum_required(VERSION 3.0)

option(USE_CLANG "build application with clang" ON)

if(NOT USE_CLANG)
    message(FATAL_ERROR "You have to install llvm+clang")
endif()

SET (CMAKE_C_FLAGS "-Wall -fembed-bitcode -std=c99")

add_executable (test main.c)

target_link_libraries (test LINK_PUBLIC foobar)
