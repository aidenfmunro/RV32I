include_guard(GLOBAL)

option(RV32I_ENABLE_WERROR "Treat warnings as errors"                  OFF)
option(RV32I_ENABLE_ASAN   "Enable AddressSanitizer (Debug)"           OFF)
option(RV32I_ENABLE_UBSAN  "Enable UndefinedBehaviorSanitizer (Debug)" OFF)
option(RV32I_ENABLE_TSAN   "Enable ThreadSanitizer (Debug)"            OFF)
option(RV32I_ENABLE_IPO    "Enable LTO/IPO (Release, RelWithDebInfo)"  OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

function(rv32i_apply_project_options target)
  target_compile_features(${target} PUBLIC cxx_std_20)
  set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)

  # Warnings

  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
    if(RV32I_ENABLE_WERROR)
      target_compile_options(${target} PRIVATE /WX)
    endif()
  else()
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic
      -Wshadow -Wconversion -Wsign-conversion
      -Wcast-qual -Wformat=2 -Wundef
      -Wnon-virtual-dtor -Woverloaded-virtual
      -fdiagnostics-color=always
    )
    if(RV32I_ENABLE_WERROR)
      target_compile_options(${target} PRIVATE -Werror)
    endif()
  endif()


  if(NOT MSVC)
    # Debug
    target_compile_options(${target} PRIVATE
      $<$<CONFIG:Debug>:-Og -g3 -fno-omit-frame-pointer>)
    target_link_options(${target} PRIVATE
      $<$<CONFIG:Debug>:-fno-omit-frame-pointer>)

    # Release
    target_compile_options(${target} PRIVATE
      $<$<CONFIG:Release>:-O3>
      $<$<CONFIG:RelWithDebInfo>:-O2 -g>
      $<$<CONFIG:MinSizeRel>:-Os>)

    # Optional sanitizers (Debug only)
    if(RV32I_ENABLE_ASAN)
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>)
      target_link_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address>)
    endif()
    if(RV32I_ENABLE_UBSAN)
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=undefined>)
      target_link_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=undefined>)
    endif()
    if(RV32I_ENABLE_TSAN)
      target_compile_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=thread>)
      target_link_options(${target} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=thread>)
    endif()
  endif()

  # PIC helps with sanitizers and shared libs
  set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)

  # LTO/IPO for release configs
  if(RV32I_ENABLE_IPO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_ok OUTPUT ipo_msg)
    if(ipo_ok)
      set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
      set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)
    else()
      message(STATUS "IPO/LTO not supported: ${ipo_msg}")
    endif()
  endif()
endfunction()

