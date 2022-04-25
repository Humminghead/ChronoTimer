cmake_minimum_required(VERSION 3.5)

project(ChronoTimer VERSION 1.0.0 HOMEPAGE_URL https://github.com/Humminghead/ChronoTimer LANGUAGES CXX)

include(GNUInstallDirs)

# Create library
add_library(${PROJECT_NAME} INTERFACE)

# Collect header files in include directory
file(GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

target_sources(${PROJECT_NAME} INTERFACE "$<BUILD_INTERFACE:${H_FILES}>")
target_include_directories(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_include_directories(${PROJECT_NAME} SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)

# Define install path
set(INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${INCLUDEDIR}/${PROJECT_NAME})

# Print install path
message(VERBOSE "Install add_subdirectory(): " ${INSTALL_DIR})

# Install target FROM to DESTINATION
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/chronotimer.h" DESTINATION ${INSTALL_DIR})
