PROJECT = MyProgram
EMSCRIPTEN_HOME = ~/emsdk

SOURCES = \
src/main.cpp \
src/SDL2_gui/SDL_gui.cpp \
src/SDL2_gui/GUI_shapes.cpp \
src/SDL2_gui/GUI_Utils.cpp \
src/SDL2_gui/GUI_View.cpp \
src/SDL2_gui/GUI_image.cpp \
src/SDL2_gui/GUI_ImageView.cpp \
src/SDL2_gui/GUI_Fonts.cpp \
src/SDL2_gui/GUI_TextView.cpp \
src/SDL2_gui/GUI_IconView.cpp \
src/SDL2_gui/GUI_Label.cpp \
src/SDL2_gfx/SDL2_gfxPrimitives.c \
../../../SDL2_ttf_HarfBuzz/SDL_ttf.cpp

HTML = \
index.html

OBJECTS = $(patsubst %.cpp,%.bc,$(patsubst %.c,%.bc,$(SOURCES)))

EMFLAGS := \
-s USE_SDL=2 \
-s USE_SDL_IMAGE=2 \
-s USE_FREETYPE=1 \
-s SDL2_IMAGE_FORMATS='["png","bmp"]' \
-s WASM=1 \
-s MODULARIZE=1

CFLAGS := \
-Os \
-Isrc/SDL2_gui \
-Isrc/SDL2_gfx 

CPPFLAGS := \
-Os \
-std=c++14 \
-I../../../SDL2_ttf_HarfBuzz \
-I../../../SDL2_ttf_HarfBuzz/external/harfbuzz-1.4.6 \
-Isrc/SDL2_gui \
-Isrc/SDL2_gfx 

LFLAGS := \
-s USE_HARFBUZZ=1 \
--preload-file ../data 

OUTFILES := \
build/$(PROJECT).js \
build/$(PROJECT).wasm \
build/index.html \
$(OBJECTS)

preset:
	$(eval CC := emcc)
	$(eval TARGETTRAIL := .js)

listobj:
	echo $(OBJECTS)

%.bc: %.c
	$(CC) $(CFLAGS) $(EMFLAGS) $(OPTFLAGS)  -c $< -o $@	

%.bc: %.cpp
	$(CC) $(CPPFLAGS) $(EMFLAGS) $(OPTFLAGS)  -c $< -o $@		

build/$(PROJECT)$(TARGETTRAIL): $(OBJECTS)	
	$(CC) $(CPPFLAGS) $(EMFLAGS) $(OPTFLAGS) $(LFLAGS) $(EXTRALIBS) $^ -o build/$(PROJECT)$(TARGETTRAIL)

build/index.html: $(HTML)
	cp $^ build/

webapp: preset build/$(PROJECT)$(TARGETTRAIL) build/index.html 

cleanobj:
	rm -f $(OBJECTS) 

clean:
	rm -f $(OUTFILES)

all: clean webapp

.DEFAULT_GOAL := webapp