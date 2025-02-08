cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    display
    VERSION 0.1.8
    DESCRIPTION "TUI library"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

find_package(alec 0.1.13 CONFIG REQUIRED)

# ---- Declare library ----

add_library(
    display_display
    source/display.cpp
    source/layout.cpp
)
target_link_libraries(display_display PUBLIC alec::alec)
add_library(display::display ALIAS display_display)

include(GenerateExportHeader)
generate_export_header(
    display_display
    BASE_NAME display
    EXPORT_FILE_NAME export/display/display_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(display_display PUBLIC DISPLAY_STATIC_DEFINE)
endif()

set_target_properties(
    display_display PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME display
    OUTPUT_NAME display
)

target_include_directories(
    display_display ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    display_display SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(display_display PUBLIC cxx_std_20)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Examples ----

if(PROJECT_IS_TOP_LEVEL)
  option(BUILD_EXAMPLES "Build examples tree." "${display_DEVELOPER_MODE}")
  if(BUILD_EXAMPLES)
    add_subdirectory(example)
  endif()
endif()

# ---- Developer mode ----

if(NOT display_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of display"
  )
endif()

include(cmake/dev-mode.cmake)
