//
//  GUI_Fonts.cpp
//  GUI_TextView
//
//  Created by Panutat Tejasen on 12/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Fonts.h"
#include "SDL_gui.h"

std::map<std::string, TTF_Font *> GUI_Fonts::font_map;

GUI_Fonts::GUI_Fonts() {
}

GUI_Fonts::~GUI_Fonts() {
    
}

TTF_Font *GUI_Fonts::getFont( std::string fontName, int fontSize ) {
    std::string font_id = fontName + std::to_string(fontSize);
    
    if(font_map.find(font_id) != font_map.end())
        return font_map[font_id];
    
    GUI_Log( font_id.c_str() );
    
    std::string fontPath = "data/"+fontName;
    TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize * GUI_scale);
    
    if (!font) {
        GUI_Log("font-spec %s not found\n", fontPath.c_str());
        return NULL;
    }
    
    font_map.insert(std::make_pair(font_id, font));
    
    return font;
}

void GUI_Fonts::clear() {
    std::map<std::string, TTF_Font *>::iterator it;
    
    for ( it = font_map.begin(); it != font_map.end(); it++ )
    {
        TTF_Font *font = it->second;   // string's value
        TTF_CloseFont( font );
    }
    font_map.clear();
}
