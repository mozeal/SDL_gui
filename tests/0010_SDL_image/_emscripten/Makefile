PROJECT = MyProgram
EMSCRIPTEN_HOME = ~/emsdk

SOURCES = \
src/main.cpp

HTML = \
index.html

OBJECTS = $(patsubst %.cpp,%.bc,$(patsubst %.c,%.bc,$(SOURCES)))

EMFLAGS := \
-s USE_SDL=2 \
-s USE_SDL_IMAGE=2 \
-s SDL2_IMAGE_FORMATS='["png","bmp"]' \
-s WASM=1 \
-s MODULARIZE=1

CFLAGS := \
-Os

CPPFLAGS := \
-Os

LFLAGS := \
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