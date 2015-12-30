# SDL_gui
GUI library for SDL2

## How to Compile

Checkout following source to the same direcotry:
- Checkout SDL from https://github.com/mozeal/SDL_gui, switch to 'jimmy' branch
- Checkout SDL_image from https://github.com/mozeal/SDL_image-mirror, switch to 'jimmy' branch
- Checkout SDL_ttf from https://github.com/mozeal/SDL_ttf-mirror, switch to 'jimmy' branch
- Checkout SDL_gui from https://github.com/mozeal/SDL_gui 

Set environment:
- SDL_ROOT to <SDL directory>
- SDL_IMAGE_ROOT to <SDL_image directory>
- SDL_TTF_ROOT to <SDL_ttf directory>
- SDL_GUI_ROOT to <SDL_gui directory>

Compile Android Examples:
- run "jni_rebuild.sh" to rebuild all jni (or run "jni_build.sh" to build only changed files)
- run "android_build.sh" while connect your android device to your PC.

Screenshots:

![Alt text](/screenshot/screenshot1.png?raw=true)
![Alt text](/screenshot/screenshot2.png?raw=true)
![Alt text](/screenshot/screenshot3.jpg?raw=true)