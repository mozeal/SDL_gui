PROJECT = MyProgram
SDL_GUI_ROOT = ../SDL2_gui

SOURCES = \
../src/main.cpp \
../../../SDL2_ttf_HarfBuzz/SDL_ttf.cpp

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
-I/$(SDL_GUI_ROOT) \
-D_REENTRANT

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

