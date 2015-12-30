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
#include "GUI_utils.h"

static SDL_Texture *background = 0;
static SDL_Texture *texture = 0;

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

/*
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image filename to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &filename, SDL_Renderer *ren)
{
    SDL_Surface *bmp_surface;
    /* load the bmp */
    std::string imagePath = GUI_getResourcePath() + filename;
    
    GUI_log("Load:%s\n", imagePath.c_str() );
    bmp_surface = SDL_LoadBMP(imagePath.c_str());
    if (bmp_surface == NULL) {
        GUI_log("could not load bmp");
    }
    /* set white to transparent on the happyface */
    //SDL_SetColorKey(bmp_surface, 1, SDL_MapRGB(bmp_surface->format, 255, 255, 255));
    
    /* convert RGBA surface to texture */
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, bmp_surface);
    if (tex == NULL) {
        GUI_log("could not create texture");
    }
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    
    /* free up allocated memory */
    SDL_FreeSurface(bmp_surface);
    
    return tex;
}

int
main(int argc, char *argv[])
{
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fatalError("Could not initialize SDL");
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
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
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
    texture = loadTexture( "image.bmp", renderer );
    
    /* main loop */
    Uint32 startFrame;
    Uint32 endFrame;
    int delay;
    bool done = false;
    
    while (!done) {
        startFrame = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
            //If user presses any key
            if (event.type == SDL_KEYDOWN){
                done = true;
            }

            if (event.type == SDL_FINGERDOWN){
                SDL_TouchFingerEvent e = event.tfinger;
                //logOut( "Touch #%i Down: %li %i %ui\n", (long)e.fingerId, (int)(e.x*100), (int)(e.y * 100), e.timestamp );
            }
            if (event.type == SDL_FINGERUP){
                SDL_TouchFingerEvent e = event.tfinger;
                //logOut( "Touch #%i Up: %i %i %ui\n", (int)e.fingerId, (int)(e.x*100), (int)(e.y * 100), e.timestamp );
            }
            if (event.type == SDL_FINGERMOTION){
                SDL_TouchFingerEvent e = event.tfinger;
                //logOut( "Touch #%i Motion: %i %i %ui\n", (int)e.fingerId, (int)(e.x*100), (int)(e.y * 100), e.timestamp );
            }

            //If user clicks the mouse
            if (event.type == SDL_MOUSEBUTTONDOWN){
                SDL_MouseButtonEvent e = event.button;
                GUI_log( "Mouse #%i Down: %i %i %ui\n", e.which, e.x, e.y, e.timestamp );
            }
            if (event.type == SDL_MOUSEBUTTONUP){
                SDL_MouseButtonEvent e = event.button;
                //logOut( "Mouse #%i Up: %i %i %ui\n", e.which, e.x, e.y, e.timestamp );
            }
            if (event.type == SDL_MOUSEMOTION){
                SDL_MouseMotionEvent e = event.motion;
                //logOut( "Mouse #%i move: %i %i %ui\n", e.which, e.x, e.y, e.timestamp );
            }
            if (event.type == SDL_MOUSEWHEEL){
                SDL_MouseWheelEvent e = event.wheel;
                //logOut( "Mouse #%i wheel: %i %i %ui\n", e.which, e.x, e.y, e.timestamp );
            }

        }
        render(renderer);
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
    
    //Clean up our objects and quit
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    /* shutdown SDL */
    SDL_Quit();
    
    return 0;
    
}
