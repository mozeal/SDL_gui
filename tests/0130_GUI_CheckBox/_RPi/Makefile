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
../../../SDL2_gui/GUI_Button.cpp \
../../../SDL2_gui/GUI_CheckBox.cpp \
../../../SDL2_gui/GUI_RadioButton.cpp \
../../../SDL2_gui/GUI_RadioButtonGroup.cpp

OUTDIR = build

$(OUTDIR)/:
	mkdir -p build
	ln -s ../../data build/data

OBJECTS = $(patsubst %.cpp,%.o,$(patsubst %.c,%.o,$(SOURCES)))

preset: 
	$(eval CC := g++)
	



CPPFLAGS := \
-Os
	
CFLAGS := \
-I/usr/local/include/SDL2 \
-I/opt/vc/include \
-I/opt/vc/include/interface/vcos/pthreads \
-I/opt/vc/include/interface/vmcs_host/linux \
-I../../../SDL2_ttf_HarfBuzz/external/freetype-2.4.12/include \
-I../../../SDL2_ttf_HarfBuzz/external/harfbuzz-1.4.6 \
-I$(SDL_GUI_ROOT) \
-I$(SDL_GFX_ROOT) \
-D_REENTRANT \
-D__RPI__

LFLAGS := \
-L/usr/local/lib \
-Wl,-rpath,/usr/local/lib \
-Wl,--enable-new-dtags \
-lSDL2 \
-lSDL2_image \
-lSDL2_ttf \
-lfreetype \
-lharfbuzz

OUTFILES := \
$(OUTDIR)/$(PROJECT) \
$(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) $(OPTFLAGS)  -c $< -o $@	
	
%.o: %.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) $(OPTFLAGS)  -c $< -o $@		

build/$(PROJECT): $(OBJECTS)	
	$(CC) $(CPPFLAGS) $(LFLAGS) $(EXTRALIBS) $^ -o build/$(PROJECT)

app: preset $(OUTDIR)/ $(OUTDIR)/$(PROJECT)

clean:
	rm -f $(OUTFILES)

all: clean app

.DEFAULT_GOAL := app

