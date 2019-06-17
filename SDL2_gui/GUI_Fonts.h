//
//  GUI_Fonts.hpp
//  GUI_ImageView
//
//  Created by Panutat Tejasen on 12/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_Fonts_hpp
#define GUI_Fonts_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <SDL2/SDL_ttf.h>

class GUI_Fonts {
protected:

public:
    static std::map<std::string, TTF_Font *> font_map;

    GUI_Fonts();
    ~GUI_Fonts();

    static TTF_Font *getFont( std::string fontName, int fontSize );
    static void clear();
};

#endif /* GUI_Fonts_hpp */
