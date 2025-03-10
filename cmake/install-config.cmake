include(CMakeFindDependencyMacro)
find_dependency(alec)

if(alec_FOUND)
  include("${CMAKE_CURRENT_LIST_DIR}/displayTargets.cmake")
endif()
