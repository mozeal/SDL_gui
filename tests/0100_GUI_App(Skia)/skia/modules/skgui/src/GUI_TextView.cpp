//
//  GUI_TextView.cpp
//  GUI_TextView
//
//  Created by Panutat Tejasen on 12/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_TextView.h"
#include "GUI_Fonts.h"
#include "skgui.h"
#include "include/core/SkSurface.h"
#include "include/core/SkImage.h"
#include "include/core/SkPaint.h"

GUI_TextView *GUI_TextView::create( GUI_View *parent, const char *title, const char *fontname, int fontsize, int x, int y, int width, int height,
                                     std::function<bool(SDL_Event* ev)>userEventHandler ) {
    return new GUI_TextView(parent, title, fontname, fontsize, x, y, width, height, userEventHandler );
}

GUI_TextView::GUI_TextView( GUI_View *parent, const char *title, const char *fontname, int fontsize, int x, int y, int width, int height,
                             std::function<bool(SDL_Event* ev)>userEventHandler):
GUI_ImageView( parent, title, NULL, x, y, width, height, userEventHandler),
//font(NULL),
forceEmptyText(false)
{
    mouseReceive = false;
    
    _fontName = fontname;
    _fontSize = fontsize;
    
    std::string fn;
    
    if( fontname ) {
        fn = fontname;
    }
    else {
        fn = GUI_GetUITextFontName();
    }
    int fs = fontsize;
    if( fs == 0 ) {
        fs = GUI_GetUIIconFontSize();
    }

    std::string fontPath = std::string("fonts/")+fn;
    font = GUI_Fonts::getFont(fn, fs);
    
//    if (!font) {
//        GUI_Log("font-spec %s not found\n", fontPath.c_str());
//        return;
//    }
    
    updateContent();
}

static void sk_release_direct_surface_storage(void* pixels, void* context){
    if (pixels == context) {
        SkDebugf("expected release context\n");
    }
    sk_free(pixels);
}

static sk_sp<SkSurface> MakeRasterDirectReleaseProc(int w, int h){
    SkImageInfo imageInfo = SkImageInfo::Make(w, h, SkColorType::kRGBA_8888_SkColorType, SkAlphaType::kUnpremul_SkAlphaType);
    size_t pixelLen = w * h * 4; // // it's 8888, so 4 bytes per pixel
    void* pixelPtr = sk_malloc_throw(pixelLen); //sk_malloc_canfail(pixelLen);
    if(pixelPtr){
        sk_sp<SkSurface> surface(SkSurface::MakeRasterDirectReleaseProc(imageInfo, pixelPtr, w*4,
                                                                        sk_release_direct_surface_storage, pixelPtr));
        return surface;
    }
    return nullptr;
}

void GUI_TextView::updateContent() {
//    static sk_sp<SkImage> create_gpu_image(GrContext* context,
//                                           bool withMips = false,
//                                           SkBudgeted budgeted = SkBudgeted::kYes) {
//        const SkImageInfo info = SkImageInfo::MakeN32(20, 20, kOpaque_SkAlphaType);
//        auto surface = SkSurface::MakeRenderTarget(context, budgeted, info, 0,
//                                                   kBottomLeft_GrSurfaceOrigin, nullptr, withMips);
//        draw_image_test_pattern(surface->getCanvas());
//        return surface->makeImageSnapshot();
//    }
    
    const char *text = " ";
    if(title.length()>0){
        text = title.substr(textSelectionScrollIndex).c_str();
    }
    else{
        text = " ";
    }
    
    SkRect bounds;
    font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds);
    auto surface = MakeRasterDirectReleaseProc(bounds.width()>2?bounds.width():2,
                                               bounds.height()>2?bounds.height():2);
    if(!surface){
        SkDebugf("cannot create surface\n");
        return;
    }
    auto canvas = surface->getCanvas();
    canvas->translate(-bounds.x(), -bounds.y());
    SkPaint p;
    canvas->clear(cClear);
    canvas->drawString(text, 0, 0, font, p);
    canvas->flush();
    auto img = surface->makeImageSnapshot();
    image.setImage(img);
    
    updateSize();
    if( parent ) {
        parent->updateLayout();
    }
    else {
        updateLayout();
    }
}

void GUI_TextView::updateSize() {
    GUI_ImageView::updateSize();
    if( forceEmptyText ) {
        if( ow == 0 ) {
            int w = 1 + (_padding[1] + _padding[3]) * GUI_scale;
            rectView.w = w;
        }
        if( oh == 0 ) {
//            rectView.h = TTF_FontHeight(font) + (_padding[0] + _padding[2]) * GUI_scale;
            int h = font.getSize() + (_padding[0] + _padding[2]) * GUI_scale;
            rectView.h = h;
        }
    }
}

GUI_TextView::~GUI_TextView() {
    
}

void GUI_TextView::setTextColor( SkColor c ) {
    _textColor = c;
    colorMod = c;
}

bool GUI_TextView::eventHandler(SDL_Event*event) {
    switch (event->type) {
        case GUI_FontChanged:
        {
            std::string fn;
            
            fn = _fontName;
            
            int fs = _fontSize;
            if( fs == 0 ) {
                fs = GUI_GetUIIconFontSize();
            }
            
//            std::string fontPath = std::string("data/")+fn;
            std::string fontPath = std::string("fonts/")+fn;
            font = GUI_Fonts::getFont(fn, fs);
            
            updateContent();
            updateSize();
            
            break;
        }
        default:
        {
            return GUI_ImageView::eventHandler(event);
        }
    }
    return false;
}

GUI_FPSView *GUI_FPSView::create( GUI_View *parent, int x, int y, int width, int height ) {
    return new GUI_FPSView( parent, x, y, width, height );
}

GUI_FPSView::GUI_FPSView(GUI_View *parent, int x, int y, int width, int height) :
GUI_TextView( parent, NULL, GUI_GetUITextFontName().c_str(), GUI_GetUITextFontSize(), x, y, width, height ),
frame_count(0),
timer_start(0)
{
    setBorder( 0 );
    setBackgroundColor(cClear);
    setTextColor(cBlack);
}

GUI_FPSView::~GUI_FPSView() {
    
}

void GUI_FPSView::update() {
    if( timer_start == 0 ) {
        timer_start = SDL_GetTicks();
        return;
    }
    frame_count++;
    Uint32 duration = SDL_GetTicks() - timer_start;
    
    if (frame_count % 10 == 0) {
        float fps = (float)frame_count / duration * 1000.0f;
        char fps_text[16];
        sprintf(fps_text, "%0.2f fps", fps);
        setTitle(fps_text);
    }
}


