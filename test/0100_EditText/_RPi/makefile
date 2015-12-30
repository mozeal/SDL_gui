CC=g++
CFLAGS=`sdl2-config --cflags --libs` -I$(SDL_GUI_ROOT)/SDL_gui -std=gnu++11 -lSDL2_image -lSDL2_ttf
OBJ=../src/main.cpp\
	$(SDL_GUI_ROOT)/SDL_gui/GUI_utils.o \
        $(SDL_GUI_ROOT)/SDL_gui/GUI_BasicWidgets.o \
        $(SDL_GUI_ROOT)/SDL_gui/GUI_MainWin.o \
        $(SDL_GUI_ROOT)/SDL_gui/GUI_TopWin.o \
        $(SDL_GUI_ROOT)/SDL_gui/GUI_WinBase.o \
        $(SDL_GUI_ROOT)/SDL_gui/SDL_gui.o

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)





