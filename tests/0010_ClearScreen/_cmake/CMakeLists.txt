cmake_minimum_required(VERSION 3.3)
project(ClearScreen)

file(MAKE_DIRECTORY "build")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(SDL2)
include_directories(${SDL2_INCLUDE_DIR})

set(SOURCE_FILES ../src/main.cpp)
add_executable(ClearScreen ${SOURCE_FILES})
target_link_libraries(ClearScreen ${SDL2_LIBRARY})

