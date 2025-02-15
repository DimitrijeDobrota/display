find_package(stamen 1.2.1 REQUIRED)
find_package(poafloc 1.2 CONFIG REQUIRED)

configure_file(menu.conf menu.conf COPYONLY)

add_custom_command(
    OUTPUT menu.hpp menu.cpp
    COMMAND stamen -d test_display --cpp -n example menu.conf
    DEPENDS menu.conf
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating menu files"
)

function(add_example NAME)
  add_executable("${NAME}" "${NAME}.cpp")
  target_include_directories("${NAME}" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
  target_link_libraries("${NAME}" PUBLIC display::display stamen::stamen)
  target_compile_features("${NAME}" PRIVATE cxx_std_20)
  add_custom_target("run_${NAME}" COMMAND "${NAME}" menu.conf VERBATIM)
  add_dependencies("run_${NAME}" "${NAME}")
  add_dependencies(run-examples "run_${NAME}")
endfunction()


add_example(navig)
target_sources(navig PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/menu.cpp")

add_folders(Navig)
