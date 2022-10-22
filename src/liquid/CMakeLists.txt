cmake_minimum_required(VERSION 3.8)

set(project_name liquid)
project(${project_name})

set(CMAKE_CXX_STANDARD 20)

add_library(${project_name} SHARED)

# Sources
target_sources(${project_name} PRIVATE
        html/tokenizer.cc
        )

# Include directories
target_include_directories(${project_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/ PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>)

# Include libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/spdlog ${CMAKE_BINARY_DIR}/vendor/spdlog)
target_link_libraries(${project_name} PRIVATE spdlog::spdlog)
set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(${project_name} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/spdlog/include/)
