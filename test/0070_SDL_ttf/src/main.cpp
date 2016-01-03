//
//  main.c
//  OSX
//
//  Created by Panutat Tejasen on 10/14/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "GUI_utils.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


static SDL_Texture *background = 0;
static SDL_Texture *texture = 0;
SDL_Renderer *renderer = NULL;
int done = 0;

/*
 * Render the message we want to display to a texture for drawing
 * @param message The message we want to display
 * @param fontFile The font we want to use to render the text
 * @param color The color we want the text to be
 * @param fontSize The size we want the font to be
 * @param renderer The renderer to load the texture in
 * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */
SDL_Texture* renderText(const std::string &message, const std::string &filename, SDL_Color color,
                        int fontSize, SDL_Renderer *ren)
{
    std::string fontFile = GUI_getResourcePath() + filename;
    
    GUI_log("Load font:%s\n", fontFile.c_str() );
    
    //Open the font
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == NULL){
        GUI_log("TTF_OpenFont");
        return NULL;
    }
    //We need to first render to a surface as that's what TTF_RenderText returns, then
    //load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
    if (surf == NULL){
        TTF_CloseFont(font);
        GUI_log( "TTF_RenderText");
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    if (texture == NULL){
        GUI_log( "CreateTexture");
    }
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
    //Setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    //Query the texture to get its width and height to use
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void
render(SDL_Renderer *renderer)
{
    /* fill background in with red */
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    //Draw the texture
    SDL_RenderCopy(renderer, background, NULL, NULL);
    
    //Draw our image in the center of the window
    //We need the foreground image's width to properly compute the position
    //of it's top left corner so that the image will be centered
    int iW, iH;
    SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(texture, renderer, x, y);
    
    /* update screen */
    SDL_RenderPresent(renderer);
}

void doLoop()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			done = 1;
		}
	}
	render(renderer);
}


/*
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &filename, SDL_Renderer *ren)
{
    std::string imagePath = GUI_getResourcePath() + filename;
    
    GUI_log("Load:%s\n", imagePath.c_str() );
    SDL_Texture *tex = IMG_LoadTexture(ren, imagePath.c_str());
    if (tex == NULL){
        GUI_log( "Load image %s failed.", filename.c_str() );
    }
    return tex;
}

int
main(int argc, char *argv[])
{
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        GUI_log("Could not initialize SDL");
    }
    //Also need to init SDL_ttf
    if (TTF_Init() != 0){
        GUI_log( "TTF_Init");
        SDL_Quit();
        return 1;
    }
    
#if __IPHONEOS__
    int landscape = 0;
    int modes = SDL_GetNumDisplayModes(0);
    int sx = 0, sy = 0;
    GUI_log( "Modes available: %i\n", modes );
    
    for (int i = 0; i < modes; i++)
    {
        SDL_DisplayMode mode;
        SDL_GetDisplayMode(0, i, &mode);
        GUI_log( "Mode #%i: %i, %i\n", i+1, mode.w, mode.h );
        if (landscape ? mode.w > sx : mode.h > sy)
        {
            sx = mode.w;
            sy = mode.h;
        }
    }
#else
    int sx = 720, sy = 480;
#endif
    printf("picked: %d %d\n", sx, sy);
    
    //Now create a window with title "SDL" at 0, 0 on the screen with w:800 h:600 and show it
    SDL_Window *window = SDL_CreateWindow("SDL", 0, 0, sx, sy,
                              SDL_WINDOW_OPENGL );
    if (window == NULL){
        fatalError("SDL_CreateRenderer Error");
    }
    SDL_GetWindowSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    GUI_log("given: %d %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    //Create a renderer that will draw to the window, -1 specifies that we want to load whichever
    //video driver supports the flags we're passing
    //Flags: SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
    //SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
    //synchornized with the monitor's refresh rate
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL){
        fatalError("SDL_CreateRenderer Error");
    }
    
    int scalex = SCREEN_WIDTH / sx;
    int scaley = SCREEN_HEIGHT / sy;
    int scale = (scalex < scaley) ? scalex : scaley;
    if( scale < 1 )
        scale = 1;
    SDL_RenderSetScale( renderer, scale, scale );
    SCREEN_WIDTH /= scale;
    SCREEN_HEIGHT /= scale;
    GUI_log( "Scale: %i\n", scale );
    
    background = loadTexture( "background.bmp", renderer );
    
    //We'll render the string "TTF fonts are cool!" in white
    //Color is in RGB format
    SDL_Color color = { 255, 255, 255, 255 };
    texture = renderText("Truetype fonts \xCA\xC7\xD1\xCA\xB4\xD5", "TS-kaewpet-NP.ttf", color, 64, renderer);
    if (texture == NULL){
        SDL_DestroyTexture(background);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

#ifdef __EMSCRIPTEN__
  // void emscripten_set_main_loop(em_callback_func func, int fps, int simulate_infinite_loop);
  emscripten_set_main_loop(doLoop, 60, 1);
#else    
    /* main loop */
    Uint32 startFrame;
    Uint32 endFrame;
    int delay;
    done = 0;
    
    
    while (!done) {
        startFrame = SDL_GetTicks();
        
        doLoop();
        
        endFrame = SDL_GetTicks();
        
        /* figure out how much time we have left, and then sleep */
        delay = MILLESECONDS_PER_FRAME - (endFrame - startFrame);
        if (delay < 0) {
            delay = 0;
        } else if (delay > MILLESECONDS_PER_FRAME) {
            delay = MILLESECONDS_PER_FRAME;
        }
        SDL_Delay(delay);
    }
#endif 
    
    
    //Clean up our objects and quit
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    /* shutdown SDL */
    SDL_Quit();
    
    return 0;
    
}
