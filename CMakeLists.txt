cmake_minimum_required(VERSION 3.5)

project(ChronoTimer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CXX_FILES)
file(GLOB CXX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_library(${PROJECT_NAME} OBJECT ${CXX_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>  # <prefix>/include/mylib
)

#target_include_directories(${PROJECT_NAME}) PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


#add_executable(ChronoTimer main.cpp)
