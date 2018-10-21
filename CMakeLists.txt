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

find_package(PkgConfig)
pkg_check_modules(AOSD REQUIRED libaosd)

target_link_libraries(mousewarp ${AOSD_LIBRARIES})
target_include_directories(mousewarp PUBLIC ${AOSD_INCLUDE_DIRS})
target_compile_options(mousewarp PUBLIC ${AOSD_CFLAGS_OTHER})

install(TARGETS mousewarp DESTINATION bin)

add_custom_command(
    OUTPUT includes
    COMMAND nix-build ${NIX_INCLUDE} -o ${CMAKE_CURRENT_BINARY_DIR}/includes
    MAIN_DEPENDENCY ${NIX_INCLUDE}
)

add_custom_target(devel DEPENDS includes)
