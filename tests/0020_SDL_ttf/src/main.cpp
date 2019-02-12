#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#if defined(WIN32)
#include "..\_VisualC\WinApp\WinApp\resource.h"
#include <SDL_syswm.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;

float scale = 1;

int expectedWidth = 960;
int expectedHeight = 540;

const long MILLESECONDS_PER_FRAME = 1000/60;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool done = false;
void doLoop();

static SDL_Texture *texture = 0;
static SDL_Texture *background = 0;

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
    std::string fontFile = filename;

    printf("Load font:%s\n", fontFile.c_str() );

    //Open the font
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == NULL){
        printf("TTF_OpenFont");
        return NULL;
    }
    //We need to first render to a surface as that's what TTF_RenderText returns, then
    //load that surface into a texture
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, message.c_str(), color);
    if (surf == NULL){
        TTF_CloseFont(font);
        printf( "TTF_RenderText");
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    if (texture == NULL){
        printf( "CreateTexture");
    }
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}


/*
 * Loads an image into a texture on the rendering device
 * @param file The image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &filename, SDL_Renderer *ren)
{
    std::string imagePath = filename;

    printf("Load:%s\n", imagePath.c_str() );
    SDL_Texture *tex = IMG_LoadTexture(ren, imagePath.c_str());
    if (tex == NULL){
        printf( "Load image %s failed.\n", filename.c_str() );
    }
    return tex;
}

int main(int argc, char *argv[]) {
    cout << "Hello, World!" << endl;
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "Portrait LandscapeLeft LandscapeRight");

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL\n");
        exit(1);
    }
    //Also need to init SDL_ttf
    if (TTF_Init() != 0){
        printf( "TTF_Init failed");
        SDL_Quit();
        return 1;
    }

    // Get Sccreen size
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }
    printf("Display: %d %d\n", dm.w, dm.h);

    // Create Window
    SDL_Log("request: %d %d\n", expectedWidth, expectedHeight);

    //Now create a window with title "SDL" at 0, 0 on the screen with w:800 h:600 and show it
    // ::SDL_WINDOW_FULLSCREEN,    ::SDL_WINDOW_OPENGL,
    // ::SDL_WINDOW_HIDDEN,        ::SDL_WINDOW_BORDERLESS,
    // ::SDL_WINDOW_RESIZABLE,     ::SDL_WINDOW_MAXIMIZED,
    // ::SDL_WINDOW_MINIMIZED,     ::SDL_WINDOW_INPUT_GRABBED,
    // ::SDL_WINDOW_ALLOW_HIGHDPI.
#if defined(__IPHONEOS__) || defined(__ANDROID__)
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_BORDERLESS;
#else
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE;
#endif
    int cx = (dm.w - expectedWidth) / 2;
    int cy = (dm.h - expectedHeight) / 2;
    window = SDL_CreateWindow("0020_SDL_ttf", cx, cy, expectedWidth, expectedHeight, style);

    if (window == NULL) {
        printf("SDL_CreateRenderer Error\n");
        exit(1);
    }

#if defined(WIN32)
	HICON hicon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	SDL_SysWMinfo info = {0};
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo(window, &info);
	HWND hwnd = info.info.win.window;
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
#endif

	SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    printf("given: %d %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    //Create a renderer that will draw to the window, -1 specifies that we want to load whichever
    //video driver supports the flags we're passing
    //Flags: SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
    //SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
    //synchornized with the monitor's refresh rate
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error\n");
        exit(1);
    }

    int drawableWidth = 0;
    int drawableHeight = 0;
    SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);

#ifdef __ANDROID__
    int scalex = drawableWidth / expectedWidth;
    int scaley = drawableHeight / expectedHeight;
#else
    int scalex = drawableWidth / SCREEN_WIDTH;
    int scaley = drawableHeight / SCREEN_HEIGHT;
#endif
	scale = (float)((scalex < scaley) ? scaley : scalex);
	if (scale < 1.0f) {
		scale = 1.0f;
	}

    background = loadTexture("data/background.bmp", renderer);

    //We'll render the string "TTF fonts are cool!" in white
    //Color is in RGB format
    SDL_Color color = { 255, 255, 255, 255 };
    //texture = renderText("Truetype fonts \xCA\xC7\xD1\xCA\xB4\xD5", "data/TS-kaewpet-NP.ttf", color, 32, renderer);
    texture = renderText("Hello World ไก่ที่เป่าปี่ดีสุด", "data/Kanit-Regular.ttf", color, 48, renderer);
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
    emscripten_set_main_loop(doLoop, 0, 1);
#else
    // main loop
    Uint32 startFrame;
    Uint32 endFrame;
    int delay;

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
    SDL_DestroyTexture( texture );
    SDL_DestroyTexture( background );

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    /* shutdown SDL */
    SDL_Quit();

    return 0;
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
	dst.w = (int)(dst.w * scale);
	dst.h = (int)(dst.h * scale);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

void render(SDL_Renderer *renderer) {
    /* fill background in with red */
    SDL_SetRenderDrawColor(renderer, 0x64, 0x95, 0xED, 255); // Corn flower blue
    SDL_RenderClear(renderer);

    //Draw the texture
    SDL_RenderCopy(renderer, background, NULL, NULL);

    //Draw our image in the center of the window
    //We need the foreground image's width to properly compute the position
    //of it's top left corner so that the image will be centered
    int iW, iH;
    SDL_QueryTexture(texture, NULL, NULL, &iW, &iH);
    int drawableWidth = 0;
    int drawableHeight = 0;
    SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
    int x = (int)((drawableWidth - (iW * scale)) / 2);
    int y = (int)((drawableHeight - (iH * scale)) / 2);
    renderTexture(texture, renderer, x, y);

    /* update screen */
    SDL_RenderPresent(renderer);
}

void doLoop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch( event.type ) {
            case SDL_QUIT:
                done = 1;
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        printf( "Event: Window Resized: %i, %i\n", event.window.data1, event.window.data2 );
                        SCREEN_WIDTH = event.window.data1;
                        SCREEN_HEIGHT = event.window.data2;
                        break;
                }
                break;
        }
    }
    render(renderer);
}

