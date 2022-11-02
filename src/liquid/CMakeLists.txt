cmake_minimum_required(VERSION 3.8)

project(liquid)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} SHARED)

# Sources
target_sources(${PROJECT_NAME} PRIVATE
        html/tokenizer.cc
        )

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

# Include libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/spdlog ${CMAKE_BINARY_DIR}/vendor/spdlog)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/vendor/spdlog/include/)
