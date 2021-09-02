cmake_minimum_required(VERSION 3.8)

set(project_name raptor)
project(${project_name})

set(SRC_INCLUDE_PATH "src/")
set(CMAKE_CXX_STANDARD 20)

set(wxWidgets_CONFIGURATION mswu)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

file(GLOB Sources "src/html/*.cc" "src/gui/*.cc")
add_executable(${project_name} ${Sources})
target_include_directories(${project_name} PRIVATE ${SRC_INCLUDE_PATH})
target_link_libraries(${project_name} ${wxWidgets_LIBRARIES})
