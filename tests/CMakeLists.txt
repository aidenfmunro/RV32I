# Prefer a config package if installed:
find_package(GTest CONFIG QUIET)
if(NOT GTest_FOUND)
  # Fallback to standard find if no CMake package
  find_package(GTest REQUIRED)
endif()

# Your test sources
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/tests/source/*.cpp")

add_executable(rv32i_tests ${TEST_SOURCES})
target_link_libraries(rv32i_tests PRIVATE rv32i_core GTest::gtest GTest::gtest_main)
target_include_directories(rv32i_tests PRIVATE "${CMAKE_SOURCE_DIR}/include")
rv32i_apply_project_options(rv32i_tests)

include(GoogleTest)
gtest_discover_tests(rv32i_tests)

