cmake_minimum_required(VERSION 3.14)

project(displayTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(display REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(utility_test source/utility_test.cpp)
target_link_libraries(utility_test PRIVATE display::display)
target_compile_features(utility_test PRIVATE cxx_std_20)

add_test(NAME utility_test COMMAND utility_test)

# ---- End-of-file commands ----

add_folders(Test)
