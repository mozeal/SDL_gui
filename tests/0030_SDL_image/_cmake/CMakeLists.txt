cmake_minimum_required(VERSION 3.3)
project(LoadTexture)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(SDL2)
find_package(SDL2_IMAGE)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

set(SOURCE_FILES ../src/main.cpp)
add_executable(LoadTexture ${SOURCE_FILES})
target_link_libraries(LoadTexture ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})
