cmake_minimum_required(VERSION 3.8)

set(project_name raptor)
project(${project_name})

set(SRC_INCLUDE_PATH "src/")
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

file(GLOB Sources "src/html/*.cc" "src/gui/*.cc")
add_executable(${project_name} ${Sources})
target_include_directories(${project_name} PRIVATE ${SRC_INCLUDE_PATH})


link_directories(${project_name} ${GTKMM_LIBRARY_DIRS})
include_directories(${project_name} ${GTKMM_INCLUDE_DIRS})

target_link_libraries(${project_name} ${GTKMM_LIBRARIES})
