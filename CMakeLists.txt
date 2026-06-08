cmake_minimum_required(VERSION 3.10)

project(lab06h VERSION 0.1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(formatter_lib)
add_subdirectory(formatter_ex_lib)
add_subdirectory(solver_lib)
add_subdirectory(hello_world_application)
add_subdirectory(solver_application)

set_target_properties(equation PROPERTIES OUTPUT_NAME solver)

install(TARGETS equation
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
)

if(BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest/CMakeLists.txt)
  enable_testing()

  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  add_subdirectory(third-party/gtest)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    foreach(_target gtest gtest_main gmock gmock_main)
      if(TARGET ${_target})
        target_compile_options(${_target} PRIVATE -Wno-error)
      endif()
    endforeach()
  endif()

  file(GLOB TEST_SOURCES tests/*.cpp)

  if(TEST_SOURCES)
    add_executable(check ${TEST_SOURCES})
    target_link_libraries(check solver gtest_main gmock)
    add_test(NAME check COMMAND $<TARGET_FILE:check>)
  endif()
endif()

include(CPackConfig.cmake)
