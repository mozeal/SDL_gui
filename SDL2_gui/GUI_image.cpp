//
//  GUI_image.cpp
//  GUI_ImageView
//
//  Created by Panutat Tejasen on 23/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_image.h"

GUI_image::GUI_image() :
_texture(NULL),
__shared_ptr_textureIcon(false)
{
}

GUI_image::GUI_image(const std::string &filename) {
    loadTexture(filename);
}

GUI_image::~GUI_image() {
    if (_texture && __shared_ptr_textureIcon == false) {
        SDL_DestroyTexture(_texture);
    }
}

void GUI_image::setTexture(SDL_Texture *tex, bool shared) {
    if (_texture && __shared_ptr_textureIcon == false) {
        SDL_DestroyTexture(_texture);
    }
    if (!tex) {
        _texture = NULL;
        return;
    }
    
    __shared_ptr_textureIcon = shared;
    
    int iW, iH;
    SDL_QueryTexture(tex, NULL, NULL, &iW, &iH);
    
    _texture = tex;
    bounds.x = 0;
    bounds.y = 0;
    bounds.w = iW;
    bounds.h = iH;
}

void GUI_image::loadTexture(const std::string &filename, SDL_Renderer *renderer) {
    SDL_Texture *tex = GUI_LoadTexture( filename, renderer );
    setTexture( tex );
}
