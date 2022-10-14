cmake_minimum_required(VERSION 3.8)

set(project_name liquid)
project(${project_name})

set(SRC_INCLUDE_PATH "./")
set(CMAKE_CXX_STANDARD 20)

add_library(${project_name} SHARED)

# Sources
target_sources(${project_name} PRIVATE "library.cc")

# Include directories
target_include_directories(${project_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>)