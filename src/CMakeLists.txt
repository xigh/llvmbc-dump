cmake_minimum_required(VERSION 3.0)

set(SRCS    bit.c
            dump.c
            extr.c
            func.c
            main.c
            mod.c
            defs.h
)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(llvmbc-dump ${SRCS})

llvm_map_components_to_libnames(llvm_libs
  Analysis
  AsmPrinter
  CodeGen
  Core
  Support
  Target
  ${LLVM_TARGETS_TO_BUILD}
)

target_link_libraries(llvmbc-dump ${llvm_libs})
