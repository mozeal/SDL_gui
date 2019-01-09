LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := SDL2_ttf

FREETYPE_LIBRARY_PATH := external/freetype-2.4.12
HARFBUZZ_LIBRARY_PATH := external/harfbuzz-1.4.6

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_SRC_FILES := SDL_ttf.cpp \

ifneq ($(HARFBUZZ_LIBRARY_PATH),)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(HARFBUZZ_LIBRARY_PATH)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(HARFBUZZ_LIBRARY_PATH)/hb-ucdn
    LOCAL_CFLAGS += -DFT2_BUILD_LIBRARY
    LOCAL_CFLAGS += -DHB_NO_MT -DHAVE_OT -DHAVE_UCDN
    LOCAL_SRC_FILES += \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ucdn/ucdn.c \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-blob.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-buffer-serialize.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-buffer.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-common.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-face.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-font.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ft.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-tag.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-set.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-shape.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-shape-plan.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-shaper.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-unicode.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-warning.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-font.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-layout.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-map.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-arabic.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-default.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-hangul.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-hebrew.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-indic.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-indic-table.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-myanmar.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-thai.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-tibetan.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-use.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-complex-use-table.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-normalize.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-shape-fallback.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ucdn.cc \
	    $(HARFBUZZ_LIBRARY_PATH)/hb-ot-var.cc
endif

ifneq ($(FREETYPE_LIBRARY_PATH),)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(FREETYPE_LIBRARY_PATH)/include
    LOCAL_CFLAGS += -DFT2_BUILD_LIBRARY
    LOCAL_SRC_FILES += \
        $(FREETYPE_LIBRARY_PATH)/src/autofit/autofit.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftbase.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftbbox.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftbdf.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftbitmap.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftcid.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftdebug.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftfstype.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftgasp.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftglyph.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftgxval.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftinit.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftlcdfil.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftmm.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftotval.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftpatent.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftpfr.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftstroke.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftsynth.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftsystem.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/fttype1.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftwinfnt.c \
        $(FREETYPE_LIBRARY_PATH)/src/base/ftxf86.c \
        $(FREETYPE_LIBRARY_PATH)/src/bdf/bdf.c \
        $(FREETYPE_LIBRARY_PATH)/src/bzip2/ftbzip2.c \
        $(FREETYPE_LIBRARY_PATH)/src/cache/ftcache.c \
        $(FREETYPE_LIBRARY_PATH)/src/cff/cff.c \
        $(FREETYPE_LIBRARY_PATH)/src/cid/type1cid.c \
        $(FREETYPE_LIBRARY_PATH)/src/gzip/ftgzip.c \
        $(FREETYPE_LIBRARY_PATH)/src/lzw/ftlzw.c \
        $(FREETYPE_LIBRARY_PATH)/src/pcf/pcf.c \
        $(FREETYPE_LIBRARY_PATH)/src/pfr/pfr.c \
        $(FREETYPE_LIBRARY_PATH)/src/psaux/psaux.c \
        $(FREETYPE_LIBRARY_PATH)/src/pshinter/pshinter.c \
        $(FREETYPE_LIBRARY_PATH)/src/psnames/psmodule.c \
        $(FREETYPE_LIBRARY_PATH)/src/raster/raster.c \
        $(FREETYPE_LIBRARY_PATH)/src/sfnt/sfnt.c \
        $(FREETYPE_LIBRARY_PATH)/src/smooth/smooth.c \
        $(FREETYPE_LIBRARY_PATH)/src/tools/apinames.c \
        $(FREETYPE_LIBRARY_PATH)/src/truetype/truetype.c \
        $(FREETYPE_LIBRARY_PATH)/src/type1/type1.c \
        $(FREETYPE_LIBRARY_PATH)/src/type42/type42.c \
        $(FREETYPE_LIBRARY_PATH)/src/winfonts/winfnt.c
endif

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_EXPORT_C_INCLUDES += $(LOCAL_C_INCLUDES)

include $(BUILD_SHARED_LIBRARY)
