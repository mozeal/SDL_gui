//
//  GUI_Fonts.cpp
//  GUI_TextView
//
//  Created by Panutat Tejasen on 12/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Fonts.h"
#include "skgui.h"
#include "include/core/SkFont.h"
#include "tools/ResourceFactory.h"
#include "tools/Resources.h"

std::map<std::string, SkFont> GUI_Fonts::font_map;

GUI_Fonts::GUI_Fonts() {
}

GUI_Fonts::~GUI_Fonts() {
    
}

SkFont GUI_Fonts::getFont( std::string fontName, int fontSize ) {
    std::string font_id = fontName + std::to_string(fontSize * GUI_scale);
    
    if(font_map.find(font_id) != font_map.end())
        return font_map[font_id];
    
//    GUI_Log( font_id.c_str() );
    
    std::string fontPath = "fonts/"+fontName;
    auto tf = MakeResourceAsTypeface(fontPath.c_str());
    if (tf) {
        SkFont font(tf);
        font.setSize(fontSize * GUI_scale);
        font_map.insert(std::make_pair(font_id, font));
        return font;
    } else {
        // not all platforms support data fonts, so we just note that failure
        SkDebugf("MakeResourceAsTypeface fail!\n");
        return SkFont();
    }
    
    return SkFont();
}

void GUI_Fonts::clear() {
    std::map<std::string, SkFont>::iterator it;
    
    for ( it = font_map.begin(); it != font_map.end(); it++ )
    {
//        TTF_Font *font = it->second;   // string's value
//        TTF_CloseFont( font );
    }
    font_map.clear();
}
