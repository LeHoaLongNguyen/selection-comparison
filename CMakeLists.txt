cmake_minimum_required(VERSION 3.16)
project(selection_comparison LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default to an optimised build so benchmark numbers are meaningful.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Same flags for every algorithm. Record these in the report's methodology section.
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# All algorithms are header-only templates (like std::nth_element),
# so they live in include/selection/ and are shared by every target.
add_library(selection INTERFACE)
target_include_directories(selection INTERFACE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/bench)

add_executable(correctness tests/correctness.cpp)
target_link_libraries(correctness PRIVATE selection)

add_executable(harness bench/harness.cpp)
target_link_libraries(harness PRIVATE selection)

enable_testing()
add_test(NAME correctness COMMAND correctness)
