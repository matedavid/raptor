cmake_minimum_required(VERSION 3.8)

set(project_name raptor)
project(${project_name})

set(CMAKE_CXX_STANDARD 20)

# Create executable
add_executable(${project_name} ${SOURCES})

# Sources
target_sources(${project_name} PRIVATE src/main.cc)

# Add liquid to project
add_subdirectory(${CMAKE_SOURCE_DIR}/src/liquid)
target_link_libraries(${project_name} PRIVATE liquid)

