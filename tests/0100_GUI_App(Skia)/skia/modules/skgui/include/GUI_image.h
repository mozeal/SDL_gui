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
#include <SDL.h>
#include "GUI_Utils.h"
#include "include/core/SkImage.h"

class GUI_image {
protected:
    bool __shared_ptr_textureIcon;
public:
    GUI_image();
    GUI_image(const std::string &filename);
    virtual ~GUI_image();
    
    GUI_Rect bounds;
//    SDL_Texture *_texture;
    sk_sp<SkImage> image;

//    void setTexture(SDL_Texture *tex, bool shared=false);
//    void loadTexture(const std::string &filename, SDL_Renderer *renderer=GUI_renderer);
    void setImage(sk_sp<SkImage> img);
    void loadImage(const std::string &filename);
};

#endif /* GUI_image_hpp */
