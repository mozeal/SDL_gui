emcc \
../src/main.cpp \
../../../SDL_gui/GUI_utils.cpp \
../../../SDL_gui/GUI_BasicWidgets.cpp \
../../../SDL_gui/GUI_MainWin.cpp \
../../../SDL_gui/GUI_TopWin.cpp \
../../../SDL_gui/GUI_WinBase.cpp \
../../../SDL_gui/SDL_gui.cpp \
-I../../../SDL_gui \
-O2 \
-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 \
-s LEGACY_GL_EMULATION=1 \
-s SDL2_IMAGE_FORMATS='["png","bmp"]' \
-o sdl2.html \
-Wc++11-extensions \
-std=c++11 \
--preload-file ../data

