cmake_minimum_required(VERSION 3.8)

set(project_name raptor)
project(${project_name})

set(SRC_INCLUDE_PATH "src/")
set(CMAKE_CXX_STANDARD 20)

file(GLOB Sources
    "src/liquid/*.cc"
    "src/liquid/html/*.cc"
    "src/liquid/html/style/*.cc"
    "src/liquid/css/*.cc"
    "src/liquid/dom/*.cc"
    
    "src/gui/*.cc"
)

find_package(SFML COMPONENTS REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})

add_executable(${project_name} ${Sources})
target_include_directories(${project_name} PRIVATE ${SRC_INCLUDE_PATH})

target_link_libraries(${project_name} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
target_link_libraries(${project_name} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

