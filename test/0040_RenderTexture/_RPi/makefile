CC=g++
CFLAGS=`sdl2-config --cflags --libs` -I$(SDL_GUI_ROOT)/SDL_gui -std=gnu++11
OBJ=../src/main.cpp\
	$(SDL_GUI_ROOT)/SDL_gui/GUI_utils.o 

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)





