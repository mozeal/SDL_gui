//
//  GUI_EditText.cpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 17/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_EditText.h"
#include "GUI_TextUtil.h"
#include "GUI_Utils.h"

extern int GUI_windowWidth;
extern int GUI_windowHeight;

extern GUI_EditText *GUI_lastEditTextView;
extern SDL_Window *GUI_window;
#if defined(WIN32)
#include <SDL_syswm.h>

HMENU textSelectionContextualMenu = NULL;
UINT textSelectionContextualMenuCut = 0;
UINT textSelectionContextualMenuCopy = 0;
UINT textSelectionContextualMenuPaste = 0;
UINT textSelectionContextualMenuDelete = 0;
UINT textSelectionContextualMenuSelectAll = 0;
#endif

GUI_EditText *GUI_EditText::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                          std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_EditText( parent, title, x, y, width, height, callbackFunction );
}

GUI_EditText::GUI_EditText(GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction ) :
GUI_TextView(parent, title, GUI_GetUITextFontName().c_str(), GUI_GetUITextFontSize(), x, y, width, height, NULL )
{
    dragable = false;
    clickable = true;
    focusable = true;
    showInteract = false;
    mouseReceive = true;
    focus_need_input = true;

    forceEmptyText = true;
    
    setBorder( 1 );
    setCorner( 4 );
    focusBorder = 2;
    
    updateContent();
    
    textEditIndex = 0;
    if( title ) {
        textEditIndex = (int)strlen(title);
    }
    
    setTextColor(cBlack);
    setBackgroundColor(cWhite);
    setContentAlign( GUI_ALIGN_TOP | GUI_ALIGN_LEFT );
    setPadding( 4, 4, 4, 4 );

    textSelectionIsShiftPressed = false;
    textSelectionIsMouseDown = false;
    textSelectionMousePoint.x = 0;
    textSelectionMousePoint.y = 0;
}

GUI_EditText::~GUI_EditText() {
    
}

void GUI_EditText::draw() {
    if (title.length()) {
        if (textSelectionIsSelected()) {
            if (GUI_lastEditTextView == this) {
                SDL_Rect rect;

                if (textSelectionGetRect(&rect)) {
                    if (isFocus()) {
                        SDL_SetRenderDrawColor(GUI_renderer, cTextSelected.r, cTextSelected.g, cTextSelected.b, cTextSelected.a);
                    } else {
                        SDL_SetRenderDrawColor(GUI_renderer, cGrey.r, cGrey.g, cGrey.b, cGrey.a);
                    }

                    SDL_RenderFillRect(GUI_renderer, &rect);
                }
            } else {
                textSelectionCancel();
            }
        }
    }

    GUI_TextView::draw();

    if (isFocus()) {
        long currentTime = SDL_GetTicks() / 500;

        if (currentTime & 1) {
            int w = 0;
            int h = 0;

            TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex, textEditIndex - textSelectionScrollIndex).c_str(), &w, &h);
            int x = (_padding[3] * GUI_scale) + w;

            SDL_SetRenderDrawColor(GUI_renderer, cBlack.r, cBlack.g, cBlack.b, cBlack.a);
            SDL_RenderDrawLine(GUI_renderer, x, _padding[0] * GUI_scale, x, rectView.h - (_padding[2] + 2) * GUI_scale);
        }

        textSelectionMouseMove();
    }
}

bool GUI_EditText::eventHandler(SDL_Event*event) {
    SDL_Scancode scancode;
    
    switch (event->type) {
        case SDL_KEYDOWN:
        {
            if (GUI_lastEditTextView != this) {
                return false;
            }

            if (this->isEnable() == false) {
                textSelectionCancel();
                return false;
            }

            bool isCtrlPressed = event->key.keysym.mod & CUSTOM_KMOD_CTRL;

            if (isCtrlPressed) {
                if (event->key.keysym.sym == SDLK_a) {
                    textSelectionSelectAll();
                } else if (event->key.keysym.sym == SDLK_x) {
                    textSelectionCut();
                } else if (event->key.keysym.sym == SDLK_c) {
                    textSelectionCopy();
                } else if (event->key.keysym.sym == SDLK_v) {
                    textSelectionPaste();
                }
            } else {
                scancode = event->key.keysym.scancode;
                bool isUpdateSubLayout = false;
                bool isResetScrollIndex = false;
                int len = (int)title.length();
                int oi = textEditIndex;

                if (scancode == SDL_SCANCODE_LSHIFT || scancode == SDL_SCANCODE_RSHIFT) {
                    if (!textSelectionIsShiftPressed) {
                        textSelectionIsShiftPressed = true;

                        if (!textSelectionIsSelected()) {
                            textSelectionStartIndex = textEditIndex;
                            textSelectionEndIndex = textSelectionStartIndex;
                        }
                    }
                } else if (scancode == SDL_SCANCODE_HOME) {
                    textSelectionHome();
                } else if (scancode == SDL_SCANCODE_END) {
                    textSelectionEnd();
                } else if (scancode == SDL_SCANCODE_LEFT) {
                    textEditIndex = textSelectionGetPreviousMiddleCharacter(textEditIndex);

                    if (oi != textEditIndex && textEditIndex < textSelectionScrollIndex) {
                        textSelectionScrollIndex = textEditIndex;
                        isUpdateSubLayout = true;
                    }

                    if (textSelectionIsShiftPressed) {
                        textSelectionEndIndex = textEditIndex;
                        isUpdateSubLayout = true;
                    } else if (textSelectionIsSelected()) {
                        int startIndex = 0;
                        int endIndex = 0;
                        textSelectionGetSelectedIndex(&startIndex, &endIndex);
                        textEditIndex = startIndex;
                        textSelectionStartIndex = textEditIndex;
                        textSelectionEndIndex = textSelectionStartIndex;
                        isUpdateSubLayout = true;
                    }
                } else if (scancode == SDL_SCANCODE_RIGHT) {
                    textEditIndex = textSelectionGetNextMiddleCharacter(textEditIndex);

                    if (oi != textEditIndex) {
                        int areaWidth = textSelectionGetAreaWidth();
                        int w = 0;
                        int h = 0;
                        TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex, textEditIndex - textSelectionScrollIndex).c_str(), &w, &h);
                        if (w > areaWidth) {
                            isUpdateSubLayout = true;
                        }
                    }

                    if (textSelectionIsShiftPressed) {
                        textSelectionEndIndex = textEditIndex;
                        isUpdateSubLayout = true;
                    } else if (textSelectionIsSelected()) {
                        int startIndex = 0;
                        int endIndex = 0;
                        textSelectionGetSelectedIndex(&startIndex, &endIndex);
                        textEditIndex = endIndex;
                        textSelectionStartIndex = textEditIndex;
                        textSelectionEndIndex = textSelectionStartIndex;
                        isUpdateSubLayout = true;
                    }
                } else if (scancode == SDL_SCANCODE_DELETE) {
                    if (textSelectionIsSelected()) {
                        textSelectionDelete();
                        isUpdateSubLayout = true;
                    } else {
                        int nextIndex = textSelectionGetNextMiddleCharacter(textEditIndex);

                        if (textEditIndex < len) {
                            title = title.substr(0, textEditIndex) + title.substr(nextIndex);
                            isUpdateSubLayout = true;
                        }
                    }
                } else if (scancode == SDL_SCANCODE_BACKSPACE) {
                    if (textSelectionIsSelected()) {
                        textSelectionDelete();
                        isUpdateSubLayout = true;
                    } else {
                        if (!isFocus()) {
                            return false;
                        }
                        textEditIndex = textSelectionGetPreviousIndex(textEditIndex);

                        if (oi != textEditIndex) {
                            int areaWidth = textSelectionGetAreaWidth();
                            int w = 0;
                            int h = 0;
                            TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex).c_str(), &w, &h);
                            title = title.substr(0, textEditIndex) + title.substr(oi);

                            if (w <= areaWidth) {
                                int prevIndex = textSelectionGetPreviousMiddleCharacter(textSelectionScrollIndex);
                                TTF_SizeUTF8(font, title.substr(prevIndex).c_str(), &w, &h);
                                if (w <= areaWidth) {
                                    textSelectionScrollIndex = prevIndex;
                                }
                            } else if (textEditIndex < textSelectionScrollIndex) {
                                w = 0;
                                while (textSelectionScrollIndex > 0 && w < areaWidth / 2) {
                                    textSelectionScrollIndex = textSelectionGetPreviousMiddleCharacter(textSelectionScrollIndex);
                                    TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex, textEditIndex - textSelectionScrollIndex).c_str(), &w, &h);
                                }
                            }

                            isUpdateSubLayout = true;
                        }
                    }
                }

                if (isUpdateSubLayout) {
                    textSelectionUpdate(isResetScrollIndex);
                }
            }

            return true;
        }

        case SDL_KEYUP:
        {
            if (GUI_lastEditTextView != this) {
                return false;
            }

            if (this->isEnable() == false) {
                textSelectionCancel();
                return false;
            }

            scancode = event->key.keysym.scancode;

            if (scancode == SDL_SCANCODE_LSHIFT || scancode == SDL_SCANCODE_RSHIFT) {
                if (textSelectionIsShiftPressed) {
                    textSelectionIsShiftPressed = false;
                }
            }

            if (!isFocus()) {
                return false;
            }

            return true;
        }

        case SDL_TEXTINPUT:
        {
            if (GUI_lastEditTextView != this) {
                return false;
            }

            if (this->isEnable() == false) {
                textSelectionCancel();
                return false;
            }

            if (!isFocus()) {
                return false;
            }

            if (textSelectionIsSelected()) {
                textSelectionDelete();
            }

            title = title.insert(textEditIndex, std::string(event->text.text));
            textEditIndex += (int)strlen(event->text.text);

            textSelectionIsShiftPressed = event->key.keysym.mod & KMOD_SHIFT;

            if (textSelectionIsShiftPressed) {
                textSelectionStartIndex = textEditIndex;
                textSelectionEndIndex = textSelectionStartIndex;
            }

            textSelectionUpdate(false);
            return true;
        }

        /*case SDL_FINGERDOWN:
        {
            GUI_TextView::eventHandler(event);
            
            SDL_MouseButtonEvent e = event->button;
            
            int x = (int)(e.x*GUI_windowWidth*GUI_mouseScale);
            int y = (int)(e.y*GUI_windowHeight*GUI_mouseScale);
            if( hitTest(x, y, false) ) {
                x -= rectView.x;
                int textX = (_padding[3] * GUI_scale) + contentScrollPosnX;
                GUI_Log( "Hit edit %i\n", x - textX );
                textEditIndex = GUI_GetTextIndexFromPosition(font, title, x - textX);
                updateContent();
            }
            break;
        }*/

        case SDL_MOUSEBUTTONDOWN:
        {
            GUI_TextView::eventHandler(event);
            SDL_MouseButtonEvent e = event->button;
            int x = (int)(e.x*GUI_mouseScale);
            int y = (int)(e.y*GUI_mouseScale);
            
            if (hitTest(x, y, false)) {
                if (e.button == SDL_BUTTON_LEFT) {
                    setFocus();
                    textSelectionIsMouseDown = true;
                    textSelectionSetCaretPosition(x, y);
                } else if (e.button == SDL_BUTTON_RIGHT) {
                    setFocus();

                    SDL_Rect rect;
                    if (textSelectionGetRect(&rect)) {
                        rect.x += rectView.x;
                        rect.y += rectView.y;
                        SDL_Point pt{x, y};

                        if (!SDL_PointInRect(&pt, &rect)) {
                            textSelectionCancel();
                            textSelectionSetCaretPosition(x, y);
                        }
                    }
                }
            } else {
                killFocus();
                return false;
            }

            break;
        }

        case SDL_MOUSEMOTION:
        {
            if (GUI_lastEditTextView != this) {
                return false;
            }

            SDL_MouseButtonEvent e = event->button;
            int x = (int)(e.x * GUI_mouseScale);
            int y = (int)(e.y * GUI_mouseScale);

            if (e.button == SDL_BUTTON_LEFT) {
                if (isFocus() && textSelectionIsMouseDown && textSelectionMousePoint.x != x) {
                    textSelectionMousePoint.x = x;
                    textSelectionMousePoint.y = y;
                    textSelectionMouseMove();
                    return true;
                }
            }

            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            if (GUI_lastEditTextView != this) {
                return false;
            }

            SDL_MouseButtonEvent e = event->button;
            int x = (int)(e.x * GUI_mouseScale);
            int y = (int)(e.y * GUI_mouseScale);

            if (e.button == SDL_BUTTON_LEFT) {
                textSelectionIsMouseDown = false;
            } else if (e.button == SDL_BUTTON_RIGHT) {
#if defined(WIN32)
                if (textSelectionContextualMenu && hitTest(x, y, false) && isFocus()) {
                    UINT canCutCopy = textSelectionIsSelected() ? MF_ENABLED : MF_GRAYED;
                    UINT canPaste = textSelectionIsValidClipboardText() ? MF_ENABLED : MF_GRAYED;
                    UINT canDelete = textSelectionIsSelected() ? MF_ENABLED : MF_GRAYED;

                    int startIndex = 0;
                    int endIndex = 0;
                    textSelectionGetSelectedIndex(&startIndex, &endIndex);
                    UINT canSelectAll = (title.length() > 0 && !(startIndex == 0 && endIndex == title.length())) ? MF_ENABLED : MF_GRAYED;

                    EnableMenuItem(textSelectionContextualMenu, textSelectionContextualMenuCut, canCutCopy);
                    EnableMenuItem(textSelectionContextualMenu, textSelectionContextualMenuCopy, canCutCopy);
                    EnableMenuItem(textSelectionContextualMenu, textSelectionContextualMenuPaste, canPaste);
                    EnableMenuItem(textSelectionContextualMenu, textSelectionContextualMenuDelete, canDelete);
                    EnableMenuItem(textSelectionContextualMenu, textSelectionContextualMenuSelectAll, canSelectAll);

                    SDL_SysWMinfo info = {0};
                    SDL_VERSION(&info.version);
                    SDL_GetWindowWMInfo(GUI_window, &info);
                    HWND hwnd = info.info.win.window;
                    POINT point = {0};
                    GetCursorPos(&point);
                    TrackPopupMenu(GetSubMenu(textSelectionContextualMenu, 0), TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL);
                    return true;
                }
#endif
            }

            break;
        }

#if defined(WIN32)
        case SDL_SYSWMEVENT:
            if (GUI_lastEditTextView != this) {
                return false;
            }

            if (event->syswm.msg->msg.win.msg == WM_COMMAND) {
                if (textSelectionContextualMenu) {
                    DWORD menuId = LOWORD(event->syswm.msg->msg.win.wParam);

                    if (menuId == textSelectionContextualMenuCut) {
                        textSelectionCut();
                    } else if (menuId == textSelectionContextualMenuCopy) {
                        textSelectionCopy();
                    } else if (menuId == textSelectionContextualMenuPaste) {
                        textSelectionPaste();
                    } else if (menuId == textSelectionContextualMenuDelete) {
                        textSelectionDelete();
                    } else if (menuId == textSelectionContextualMenuSelectAll) {
                        textSelectionSelectAll();
                    }

                    return false;
                }
            }
            break;
#endif


        default:
        {
            return GUI_TextView::eventHandler(event);
        }
    }
    return false;
}

void GUI_EditText::setFocus() {
    GUI_View::setFocus();
    GUI_lastEditTextView = this;
}

int GUI_EditText::textSelectionGetIndexAtScreenPosition(int x) {
    if ((int)title.length() == 0) {
        textEditIndex = 0;
        textSelectionScrollIndex = 0;
    }

    int index = textEditIndex;
    //int left = rectView.x + topLeft.x;
    int left = rectView.x + (_padding[3] * GUI_scale);
    int j = 0;
    //x += 5;

    for (int i = textSelectionScrollIndex; i < (int)title.length(); i++) {
        //unsigned char c = title.at(i);
        int currentCharWidth = 0;
        int h = 0;
        TTF_SizeUTF8(font, title.substr(i, 1).c_str(), &currentCharWidth, &h);

        int w = 0;
        j = textSelectionGetNextIndex(i) - i;
        TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex, i + j - textSelectionScrollIndex).c_str(), &w, &h);

        if (left + w > x + (currentCharWidth / 2)) {
            index = i;
            break;
        } else {
            index = i + j;
        }

        i += j - 1;
        j = 0;
    }

    return index;
}

int GUI_EditText::textSelectionGetNextIndex(int i) {
    int j = 0;
    int len = (int)title.length();

    if (i < len) {
        unsigned char c = title.at(i);

        if ((c & 0x80) == 0) {
            j++;
        } else if ((c & 0xc0) == 0xc0) {
            j++;
            c = title.at(i + j);

            while ((c & 0xc0) == 0x80) {
                j++;

                if (i + j >= len)
                    break;

                c = title.at(i + j);
            }
        }
    }

    return i + j;
}

int GUI_EditText::textSelectionGetPreviousIndex(int i) {
    while (i > 0) {
        i--;
        int c = (title.at(i) & 0xC0);

        if (c != 0x80) {
            break;
        }
    }

    return i;
}

void GUI_EditText::textSelectionSetScrollIndex(bool isResetScrollIndex) {
    if (isResetScrollIndex || textEditIndex < textSelectionScrollIndex) {
        textSelectionScrollIndex = 0;
    }

    int i = textSelectionScrollIndex;
    int areaWidth = textSelectionGetAreaWidth();
    int w = 0;
    int h = 0;
    bool success = false;

    do {
        TTF_SizeUTF8(font, title.substr(i, textEditIndex - i).c_str(), &w, &h);
        if (w > areaWidth) {
            i = textSelectionGetNextMiddleCharacter(i);
        } else {
            success = true;
        }
    } while (!success && i < (int)title.length());

    if (success) {
        textSelectionScrollIndex = i;
    }
}

bool GUI_EditText::textSelectionIsUpperLowerCharacter(int *i) {
    bool result = false;
    int index = *i;
    int c = (unsigned char)title[index++];
    int unicode = c;

    if ((c & 0xc0) == 0xc0) { // convert unicode bytes to int
        unicode = (c & 0x0F) << 12;
        c = (unsigned char)title[index++];
        unicode += (c & 0x3F) << 6;
        c = (unsigned char)title[index++];
        unicode += (c & 0x3F);
    }

    *i = index;
    result = (unicode == 0xE31 || (unicode >= 0xE34 && unicode <= 0xE3A) || (unicode >= 0xE47 && unicode <= 0xE4E));

    return result;
}

int GUI_EditText::textSelectionGetNextMiddleCharacter(int i) {
    i = textSelectionGetNextIndex(i);
    int lastIndex = i;

    while (i < (int)title.length() && textSelectionIsUpperLowerCharacter(&i)) {
        lastIndex = i;
    };

    return lastIndex;
}

int GUI_EditText::textSelectionGetPreviousMiddleCharacter(int i) {
    int previousIndex = i;

    do {
        i = textSelectionGetPreviousIndex(i);
        previousIndex = i;
    } while (i > 0 && textSelectionIsUpperLowerCharacter(&previousIndex));

    return i;
}

void GUI_EditText::textSelectionMouseMove() {
    if (GUI_lastEditTextView == this) {
        if (isFocus() && textSelectionIsMouseDown) {
            int left = rectView.x + topLeft.x;
            int areaWidth = textSelectionGetAreaWidth();
            int right = left + areaWidth;

            if (textSelectionMousePoint.x <= left) {
                textSelectionScrollIndex = textSelectionGetPreviousMiddleCharacter(textSelectionScrollIndex);
            } else if (textSelectionMousePoint.x >= right) {
                textEditIndex = textSelectionGetNextMiddleCharacter(textEditIndex);
            }

            textEditIndex = textSelectionGetIndexAtScreenPosition(textSelectionMousePoint.x);
            textSelectionEndIndex = textEditIndex;
            textSelectionUpdate(false);
        }
    }
}

void GUI_EditText::textSelectionUpdate(bool isResetScrollIndex) {
    textSelectionSetScrollIndex(isResetScrollIndex);
    updateContent();
}

void GUI_EditText::textSelectionSelectAll() {
    if (GUI_lastEditTextView == this) {
        textSelectionStartIndex = 0;
        textSelectionEndIndex = (int)title.length();
        textEditIndex = textSelectionEndIndex;
        textSelectionUpdate(true);
    }
}

void GUI_EditText::textSelectionCut() {
    if (GUI_lastEditTextView == this) {
        textSelectionCopy();
        textSelectionDelete();
    }
}

void GUI_EditText::textSelectionCopy() {
    if (GUI_lastEditTextView == this) {
        if (textSelectionIsSelected()) {
            int startIndex = 0;
            int endIndex = 0;
            textSelectionGetSelectedIndex(&startIndex, &endIndex);
            std::string str = title.substr(startIndex, endIndex - startIndex);
            SDL_SetClipboardText(str.c_str());
        }
    }
}

void GUI_EditText::textSelectionPaste() {
    if (GUI_lastEditTextView == this) {
        if (textSelectionIsSelected()) {
            textSelectionDelete();
        }

        if (textSelectionIsValidClipboardText()) {
            char* text = SDL_GetClipboardText();
            title = title.insert(textEditIndex, std::string(text));
            textEditIndex += (int)strlen(text);
        }

        textSelectionUpdate(false);
    }
}

void GUI_EditText::textSelectionDelete() {
    if (GUI_lastEditTextView == this) {
        if (textSelectionIsSelected()) {
            int startIndex = 0;
            int endIndex = 0;
            textSelectionGetSelectedIndex(&startIndex, &endIndex);
            title = title.substr(0, startIndex) + title.substr(endIndex);
            textEditIndex = startIndex;
            textSelectionStartIndex = textEditIndex;
            textSelectionEndIndex = textSelectionStartIndex;
            textSelectionUpdate(false);
        }
    }
}

void GUI_EditText::textSelectionCancel() {
    textEditIndex = 0;
    textSelectionScrollIndex = 0;
    textSelectionStartIndex = 0;
    textSelectionEndIndex = textSelectionStartIndex;

    textSelectionUpdate(true);
}

void GUI_EditText::textSelectionGoToIndex(int i) {
    bool isUpdateSubLayout = false;
    bool isResetScrollIndex = false;
    int oi = textEditIndex;
    textEditIndex = i;

    if (oi != textEditIndex) {
        textSelectionScrollIndex = 0;
        isUpdateSubLayout = true;
        isResetScrollIndex = true;
    }

    if (textSelectionIsShiftPressed) {
        textSelectionEndIndex = textEditIndex;
        isUpdateSubLayout = true;
    } else if (textSelectionIsSelected()) {
        textSelectionStartIndex = textEditIndex;
        textSelectionEndIndex = textSelectionStartIndex;
        isUpdateSubLayout = true;
    }

    if (isUpdateSubLayout) {
        textSelectionUpdate(isResetScrollIndex);
    }
}

void GUI_EditText::textSelectionHome() {
    textSelectionGoToIndex(0);
}

void GUI_EditText::textSelectionEnd() {
    textSelectionGoToIndex((int)title.length());
}

int GUI_EditText::textSelectionGetAreaWidth() {
    int areaWidth = rectView.w - ((_padding[3] + _padding[1]) * GUI_scale);
    return areaWidth;
}

bool GUI_EditText::textSelectionIsThaiText(std::string text) {
    bool isThai = false;
    int len = (int)text.length();
    int i = 0;

    while (i < len && !isThai) {
        unsigned char c = text.at(i++);

        if ((c & 0xc0) == 0xc0) {
            isThai = true;
        }
    }

    return isThai;
}

bool GUI_EditText::textSelectionIsValidClipboardText() {
    bool result = false;

    if (SDL_HasClipboardText()) {
        char* text = SDL_GetClipboardText();

        if (text) {
            int len = (int)strlen(text);

            if (len > 0) {
                result = true;
            }
        }
    }

    return result;
}

bool GUI_EditText::textSelectionIsDigitText(std::string strText) {
    bool result = true;

    for (int i = 0; i < (int)strText.length() && result; i++) {
        unsigned char c = strText.at(i);

        if ((c & 0xc0) == 0xc0 || !(SDL_isdigit(c) || c == '.')) {
            result = false;
        }
    }

    return result;
}

bool GUI_EditText::textSelectionGetRect(SDL_Rect* rect) {
    bool success = false;

    if (rect && textSelectionIsSelected()) {
        int startIndex = 0;
        int endIndex = 0;
        textSelectionGetSelectedIndex(&startIndex, &endIndex);

        if (startIndex < textSelectionScrollIndex) {
            startIndex = textSelectionScrollIndex;
        }

        int w = 0;
        int h = 0;
        std::string str = title.substr(startIndex, endIndex - startIndex);
        TTF_SizeUTF8(font, title.substr(startIndex, endIndex - startIndex).c_str(), &w, &h);
        rect->x = _padding[3] * GUI_scale;
        rect->y = _padding[0] * GUI_scale;
        rect->w = w;
        rect->h = h;

        if (startIndex > textSelectionScrollIndex) {
            TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex, startIndex - textSelectionScrollIndex).c_str(), &w, &h);
            rect->x += w;
        }

        success = true;
    }

    return success;
}

void GUI_EditText::textSelectionSetCaretPosition(int x, int y) {
    textEditIndex = textSelectionGetIndexAtScreenPosition(x);
    textSelectionStartIndex = textEditIndex;
    textSelectionEndIndex = textSelectionStartIndex;
    textSelectionMousePoint.x = x;
    textSelectionMousePoint.y = y;
}

#if defined(WIN32)
void GUI_EditText::setContextualMenu(UINT menu, UINT menuCut, UINT menuCopy, UINT menuPaste, UINT menuDelete, UINT menuSelectAll) {
    if (textSelectionContextualMenu == NULL) {
        SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

        textSelectionContextualMenu = LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(menu));
        textSelectionContextualMenuCut = menuCut;
        textSelectionContextualMenuCopy = menuCopy;
        textSelectionContextualMenuPaste = menuPaste;
        textSelectionContextualMenuDelete = menuDelete;
        textSelectionContextualMenuSelectAll = menuSelectAll;
    }
}
#endif
