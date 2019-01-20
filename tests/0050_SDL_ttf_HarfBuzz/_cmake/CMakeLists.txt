cmake_minimum_required(VERSION 3.3)
project(TestTTF)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(SDL2)
find_package(SDL2_IMAGE)
find_package(HARFBUZZ)
include_directories(${SDL2_INCLUDE_DIR} 
${SDL2_IMAGE_INCLUDE_DIR} 
${HARFBUZZ_INCLUDE_DIR}
../../../SDL2_ttf_HarfBuzz/external/freetype-2.4.12/include
../../../SDL2_ttf_HarfBuzz/external/harfbuzz-1.4.6)

set(SOURCE_FILES 
../src/main.cpp 
../../../SDL2_ttf_HarfBuzz/SDL_ttf.cpp)
add_executable(TestTTF ${SOURCE_FILES})
target_link_libraries(TestTTF ${SDL2_LIBRARY} 
${SDL2_IMAGE_LIBRARY} 
freetype
harfbuzz)
