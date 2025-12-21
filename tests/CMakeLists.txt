find_package(GTest CONFIG QUIET)
if(NOT GTest_FOUND)
  find_package(GTest REQUIRED)
endif()

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")

add_executable(rv32i_tests
  ${TEST_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(rv32i_tests PRIVATE
  rv32i_core
  GTest::gtest
  GTest::gtest_main
)

target_include_directories(rv32i_tests PRIVATE
  "${CMAKE_SOURCE_DIR}/include"
)

rv32i_apply_project_options(rv32i_tests)

set(E2E_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/e2e)
set(E2E_COMMON_DIR ${CMAKE_SOURCE_DIR}/common)
set(E2E_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/e2e_bins)
file(MAKE_DIRECTORY ${E2E_BIN_DIR})

set(E2E_PROGRAMS
  echo
  isqrt
  bubblesort
  fcalc
  fib
  # add more here
)

set(E2E_BINARIES "")

foreach(name IN LISTS E2E_PROGRAMS)
  set(src_c  ${E2E_SRC_DIR}/${name}.c)
  set(outbin ${E2E_BIN_DIR}/${name}.rv32)

  add_custom_command(
    OUTPUT ${outbin}
    COMMAND riscv64-unknown-elf-as
            -mabi=ilp32 -march=rv32imf_zbb
            ${E2E_COMMON_DIR}/api.s -o api.o
    COMMAND riscv64-unknown-elf-gcc
            -mabi=ilp32 -march=rv32imf_zbb
            -nostartfiles -nostdlib -static
            -I${E2E_COMMON_DIR}
            api.o ${src_c} -o ${outbin}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${E2E_COMMON_DIR}/api.s
            ${src_c}
    COMMENT "Building RISC-V e2e program ${name}"
  )

  list(APPEND E2E_BINARIES ${outbin})
endforeach()

add_custom_target(e2e_programs ALL DEPENDS ${E2E_BINARIES})

# Ensure e2e binaries exist before tests run
add_dependencies(rv32i_tests e2e_programs)

# Pass paths into the test binary
target_compile_definitions(rv32i_tests PRIVATE
  E2E_BIN_DIR="${E2E_BIN_DIR}"
  INTERP_PATH="$<TARGET_FILE:rv32i>"
)

# ---- Register tests with CTest ----
include(GoogleTest)
gtest_discover_tests(rv32i_tests)
