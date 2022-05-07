cmake_minimum_required(VERSION 2.6)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

project(msc)

add_compile_options(-D_GNU_SOURCE -g)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GTK3 REQUIRED gtkmm-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

# Link the target to the GTK+ libraries

# main
add_executable(main main.cpp)
#target_include_directories(main_pmsm PRIVATE /usr/include/bms ../../elepot-progs/lib/control ${Glib_INCLUDE_DIRS})
target_link_libraries(main -lm -lgsl -lblas -lbms PkgConfig::deps  ${GTK3_LIBRARIES})

