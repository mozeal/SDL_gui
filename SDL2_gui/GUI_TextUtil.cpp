//
//  GUI_TextUtil.cpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 17/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_TextUtil.h"
#include <string>
#include "GUI_Utils.h"
#include <SDL_ttf.h>
/*
 
 Each byte starts with a few bits that tell you whether it's a single byte code-point, a multi-byte code point, or a continuation of a multi-byte code point. Like this:
 0xxx xxxx    A single-byte US-ASCII code (from the first 127 characters)
 The multi-byte code-points each start with a few bits that essentially say "hey, you need to also read the next byte (or two, or three) to figure out what I am." They are:
 110x xxxx    One more byte follows
 1110 xxxx    Two more bytes follow
 1111 0xxx    Three more bytes follow
 Finally, the bytes that follow those start codes all look like this:
 10xx xxxx    A continuation of one of the multi-byte characters
 Since you can tell what kind of byte you're looking at from the first few bits, then even if something gets mangled somewhere, you don't lose the whole sequence.
 
 Ref: https://stackoverflow.com/questions/1543613/how-does-utf-8-variable-width-encoding-work
 */

int GUI_GetPreviousUTF8Index( std::string str, int i ) {
    if( i > 0 && i <= str.length() ) {
        while( i > 0 ) {
            i--;
            int c = (str.at(i));
            if( c & 0x80 ) {
                if( c & 0x40 ) {
                    return i;
                }
            }
            else {
                return i;
            }
        }
    }
    return -1;
}

int GUI_isMainUnicodeChar( int unicode ) {
    bool result = (unicode == 0xE31 || (unicode >= 0xE34 && unicode <= 0xE3A) || (unicode >= 0xE47 && unicode <= 0xE4E));
    return !result;
}

int GUI_GetPreviousMainUTF8Index( std::string str, int i ) {
    if( i > 0 && i <= str.length() ) {
        int byte_count = 0;
        int byte = 0;
        while( i > 0 ) {
            i--;
            int c = (str.at(i));
            if( c & 0x80 ) {
                if( c & 0x40 ) {
                    if( (c & 0xf8) == 0xf0 ) {
                        byte = byte + ((c & 0x07) << (6 * byte_count));
                    }
                    else if( (c & 0xf0) == 0xe0 ) {
                        byte = byte + ((c & 0x0f) << (6 * byte_count));
                    }
                    else if( (c & 0xe0) == 0xc0 ) {
                        byte = byte + ((c & 0x1f) << (6 * byte_count));
                    }
                    if( GUI_isMainUnicodeChar(byte))
                        return i;
                    byte = 0;
                    byte_count = -1;
                }
                else {
                    byte = byte + ((c & 0x3f) << (6 * byte_count));
                }
                byte_count++;
            }
            else {
                return i;
            }
        }
    }
    return -1;
}

int GUI_GetUnicodeAtIndex( std::string str, int i, int *nextIndex ) {
    if( i >= 0 && i < str.length() ) {
        int byte_count = 0;
        int byte = 0;
        while( i < str.length() ) {
            int c = (str.at(i));
            if( c & 0x80 ) {
                if( c & 0x40 ) {
                    if( (c & 0xf8) == 0xf0 ) {
                        byte_count = 3;
                        byte = (c & 0x07);
                    }
                    else if( (c & 0xf0) == 0xe0 ) {
                        byte_count = 2;
                        byte = (c & 0x0f);
                    }
                    else if( (c & 0xe0) == 0xc0 ) {
                        byte_count = 1;
                        byte = (c & 0x1f);
                    }
                }
                else {
                    byte = (byte << 6) + (c & 0x3f);
                    byte_count--;
                    if( byte_count == 0 ) {
                        GUI_Log( "Byte: %04X\n", byte );
                        if( nextIndex ) {
                            *nextIndex = i+1;
                        }
                        return byte;
                    }
                }
            }
            else {
                byte = c;
                if( nextIndex ) {
                    *nextIndex = i+1;
                }
                return byte;
            }
            i++;
        }
    }
    return -1;
}

int GUI_GetNextMainUTF8Index( std::string str, int i ) {
    if( i >= 0 && i < str.length() ) {
        int byte;
        int nextI;
        
        byte = GUI_GetUnicodeAtIndex( str, i, &nextI );
        i = nextI;
        while( i < str.length() ) {
            int b = GUI_GetUnicodeAtIndex( str, i, &nextI );
            if( GUI_isMainUnicodeChar(b)) {
                return i;
            }
            i = nextI;
        }
        return nextI;
    }
    return -1;
}

int GUI_GetTextIndexFromPosition( TTF_Font *font, std::string str, int x ) {
    int nextI;
    int i = 0;
    while( i < str.length() ) {
        nextI = GUI_GetNextMainUTF8Index( str, i );
        int w = 0;
        int h = 0;
        TTF_SizeUTF8( font, str.substr(0,nextI).c_str(), &w, &h );
        if( w > x ) {
            return i;
        }
        i = nextI;
    }
    return i;
}
