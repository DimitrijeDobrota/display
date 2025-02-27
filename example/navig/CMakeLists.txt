find_package(stamen 1.3 REQUIRED)
find_package(poafloc 1.2 CONFIG REQUIRED)

configure_file(menu.conf menu.conf COPYONLY)

add_custom_command(
    OUTPUT menu.hpp menu.cpp
    COMMAND stamen -n example menu.conf
    DEPENDS menu.conf
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Generating menu files"
)

add_example(navig)
target_sources(navig PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/menu.cpp")
target_include_directories(navig PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

add_folders(Navig)
