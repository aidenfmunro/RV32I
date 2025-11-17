cmake_minimum_required(VERSION 3.20)
project(rv32i_sim LANGUAGES CXX)

# Modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(ProjectOptions)  # loads rv32i_apply_project_options() and knobs

file(GLOB RV32I_CORE_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/source/*.cpp")
list(REMOVE_ITEM RV32I_CORE_SOURCES "${CMAKE_SOURCE_DIR}/source/main.cpp")

add_library(rv32i_core ${RV32I_CORE_SOURCES})

target_include_directories(rv32i_core PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/third_party/ELFIO"
)

rv32i_apply_project_options(rv32i_core)

# Exe
add_executable(rv32i "${CMAKE_SOURCE_DIR}/source/main.cpp")
target_link_libraries(rv32i PRIVATE rv32i_core)
rv32i_apply_project_options(rv32i)

# Tests
option(RV32I_ENABLE_TESTS "Build unit tests" OFF)
if(RV32I_ENABLE_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

