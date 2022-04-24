cmake_minimum_required(VERSION 3.5)

project(ChronoTimer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/include)

