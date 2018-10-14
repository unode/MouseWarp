cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/.cmake")
set(NIX_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/includes.nix)

project(Mousewarp)

add_executable(mousewarp src/mousewarp.c)

find_package(Cairo REQUIRED)
include_directories(${CAIRO_INCLUDE_DIRS})
target_link_libraries(mousewarp ${CAIRO_LIBRARIES})

find_package(X11 REQUIRED)
include_directories(${X11_INCLUDE_DIR})
target_link_libraries(mousewarp ${X11_LIBRARIES})

add_library(aosd SHARED IMPORTED) # or STATIC instead of SHARED
set_target_properties(aosd PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libaosd/libaosd/libaosd.so"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/libaosd/libaosd"
)
target_link_libraries(mousewarp aosd)

install(TARGETS mousewarp DESTINATION bin)

add_custom_command(
    OUTPUT includes
    COMMAND nix-build ${NIX_INCLUDE} -o ${CMAKE_CURRENT_BINARY_DIR}/includes
    MAIN_DEPENDENCY ${NIX_INCLUDE}
)

add_custom_target(devel DEPENDS includes)
