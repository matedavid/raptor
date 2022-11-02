cmake_minimum_required(VERSION 3.8)

project(raptor)

set(CMAKE_CXX_STANDARD 20)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Sources
target_sources(${PROJECT_NAME} PRIVATE src/main.cc)

# Add liquid to project
add_subdirectory(${CMAKE_SOURCE_DIR}/src/liquid)
target_link_libraries(${PROJECT_NAME} PRIVATE liquid)
