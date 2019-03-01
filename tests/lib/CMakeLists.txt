cmake_minimum_required(VERSION 3.0)

option(USE_CLANG "build application with clang" ON)

if(NOT USE_CLANG)
    message(FATAL_ERROR "You have to install llvm+clang")
endif()

SET (CMAKE_C_FLAGS "-Wall -fembed-bitcode -std=c99")

add_library (foobar foo.c bar.c)

target_include_directories (foobar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
