cmake_minimum_required(VERSION 3.3)
project(app)

set(OUTFILE app)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/modules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D__RPI__")

find_package(SDL2)
find_package(SDL2_IMAGE)
find_package(HARFBUZZ)
find_package(Freetype REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} 
${SDL2_IMAGE_INCLUDE_DIR} 
${HARFBUZZ_INCLUDE_DIR}
${FREETYPE_INCLUDE_DIRS}
../../../SDL2_gui
../../../SDL2_gfx)

set(SOURCE_FILES 
../src/main.cpp 
../../../SDL2_ttf_HarfBuzz/SDL_ttf.cpp
../../../SDL2_gfx/SDL2_gfxPrimitives.c
../../../SDL2_gfx/SDL2_rotozoom.c
../../../SDL2_gui/SDL_gui.cpp
../../../SDL2_gui/GUI_Utils.cpp
../../../SDL2_gui/GUI_shapes.cpp
../../../SDL2_gui/GUI_View.cpp
../../../SDL2_gui/GUI_ImageView.cpp
../../../SDL2_gui/GUI_image.cpp
../../../SDL2_gui/GUI_TextView.cpp
../../../SDL2_gui/GUI_Fonts.cpp
../../../SDL2_gui/GUI_IconView.cpp)
add_executable(${OUTFILE} ${SOURCE_FILES})
target_link_libraries(${OUTFILE} ${SDL2_LIBRARY} 
${SDL2_IMAGE_LIBRARY} ${HARFBUZZ_LIBRARY}
${FREETYPE_LIBRARIES})
