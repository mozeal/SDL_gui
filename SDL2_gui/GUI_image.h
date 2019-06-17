//
//  GUI_image.hpp
//  GUI_ImageView
//
//  Created by Panutat Tejasen on 23/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_image_hpp
#define GUI_image_hpp

#include <string>
#include <stdio.h>
#include <functional>
#include <SDL2/SDL.h>
#include "GUI_Utils.h"

class GUI_image {
protected:
    bool __shared_ptr_textureIcon;
public:
    GUI_image();
    GUI_image(const std::string &filename);
    virtual ~GUI_image();
    
    GUI_Rect bounds;
    SDL_Texture *_texture;

    void setTexture(SDL_Texture *tex, bool shared=false);
    void loadTexture(const std::string &filename, SDL_Renderer *renderer=GUI_renderer);
};

#endif /* GUI_image_hpp */
