emcc ../src/main.cpp ../../../SDL_gui/GUI_utils.cpp -O2 -I../../../SDL_gui -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s LEGACY_GL_EMULATION=1 -s SDL2_IMAGE_FORMATS='["png","bmp"]' -o sdl2.html --preload-file ../data

