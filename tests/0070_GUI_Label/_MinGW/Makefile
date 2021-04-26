PROJECT = MyProgram
SDL_GUI_ROOT = ../../../SDL2_gui
SDL_GFX_ROOT = ../../../SDL2_gfx

SOURCES = \
../src/main.cpp \
../../../SDL2_ttf_HarfBuzz/SDL_ttf.cpp \
../../../SDL2_gfx/SDL2_gfxPrimitives.c \
../../../SDL2_gfx/SDL2_rotozoom.c \
../../../SDL2_gui/SDL_gui.cpp \
../../../SDL2_gui/GUI_Utils.cpp \
../../../SDL2_gui/GUI_View.cpp \
../../../SDL2_gui/GUI_shapes.cpp \
../../../SDL2_gui/GUI_ImageView.cpp \
../../../SDL2_gui/GUI_image.cpp \
../../../SDL2_gui/GUI_TextView.cpp \
../../../SDL2_gui/GUI_Fonts.cpp \
../../../SDL2_gui/GUI_IconView.cpp \
../../../SDL2_gui/GUI_Label.cpp \
../../../SDL2_gui/GUI_Config.cpp

OUTDIR = build

$(OUTDIR)/:
	mkdir -p build
	ln -s ../../data build/data 2>/dev/null || echo "Ignoring error"

OBJECTS = $(patsubst %.cpp,%local.o,$(patsubst %.c,%local.o,$(SOURCES)))

preset: 
	$(eval CC := g++)

CPPFLAGS := \
-Os

PKGS := freetype2 \
libpng \
libjpeg \
libtiff-4 \
gthread-2.0 \
harfbuzz \
libwebp \
SDL2_ttf \
SDL2_image \
sdl2 \

	
CFLAGS := \
$(shell pkg-config --cflags --static $(PKGS)) \
-I$(SDL_GUI_ROOT) \
-I$(SDL_GFX_ROOT) \
-D_REENTRANT \
-DUNICODE

LFLAGS := \
$(shell pkg-config --libs --static $(PKGS)) \
# -municode

OUTFILES := \
$(OUTDIR)/$(PROJECT) \
$(OBJECTS)

%local.o: %.c
	$(CC) $(CFLAGS) $(OPTFLAGS)  -c $< -o $@	
	
%local.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OPTFLAGS)  -c $< -o $@		

build/$(PROJECT): $(OBJECTS)
	windres resource.rc -o localrsrc.o
	$(CC) $(CPPFLAGS) localrsrc.o $^ -o build/$(PROJECT) -static $(LFLAGS) $(LFLAGS)

app: preset $(OUTDIR)/ $(OUTDIR)/$(PROJECT)

clean:
	rm -f $(OUTFILES)

all: clean app

.DEFAULT_GOAL := app

