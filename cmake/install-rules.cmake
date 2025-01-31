if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/display-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package display)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT display_Development
)

install(
    TARGETS display_display
    EXPORT displayTargets
    RUNTIME #
    COMPONENT display_Runtime
    LIBRARY #
    COMPONENT display_Runtime
    NAMELINK_COMPONENT display_Development
    ARCHIVE #
    COMPONENT display_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    display_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE display_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(display_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${display_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT display_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${display_INSTALL_CMAKEDIR}"
    COMPONENT display_Development
)

install(
    EXPORT displayTargets
    NAMESPACE display::
    DESTINATION "${display_INSTALL_CMAKEDIR}"
    COMPONENT display_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
