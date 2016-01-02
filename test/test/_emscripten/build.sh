emcc ../sdl2_image.c ../../../SDL_gui/GUI_utils.cpp -O2 -I../../../SDL_gui -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s LEGACY_GL_EMULATION=1 -o sdl2.html

