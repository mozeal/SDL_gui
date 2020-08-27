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
#include "GUI_shapes.h"

extern int GUI_windowWidth;
extern int GUI_windowHeight;

extern GUI_EditText *GUI_lastEditTextView;
extern SDL_Window *GUI_window;
extern GUI_View *GUI_topView;

#if defined(WIN32)
#include <SDL_syswm.h>

HMENU g_hContextualMenu = NULL;
UINT g_nContextualMenuCut = 0;
UINT g_nContextualMenuCopy = 0;
UINT g_nContextualMenuPaste = 0;
UINT g_nContextualMenuDelete = 0;
UINT g_nContextualMenuSelectAll = 0;
#endif

GUI_PopupMenu* contextualMenu = NULL;

GUI_EditText *GUI_EditText::create(GUI_View *parent, const char *title, int x, int y, int width, int height,
	std::function<void(GUI_View*)>callbackFunction) {
	return new GUI_EditText(parent, title, x, y, width, height, callbackFunction);
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
	touchTime = 0.0f;

	createContextualMenu();
}

GUI_EditText::~GUI_EditText() {
}

void GUI_EditText::draw() {
    if (title.length()) {
        if (textSelectionIsSelected()) {
            if (GUI_lastEditTextView == this) {
                SDL_Rect rect;

                if (textSelectionGetRect(&rect)) {
//                    if (isFocus()) {
//                        SDL_SetRenderDrawColor(GUI_renderer, cTextSelected.r, cTextSelected.g, cTextSelected.b, cTextSelected.a);
//                    } else {
//                        SDL_SetRenderDrawColor(GUI_renderer, cGrey.r, cGrey.g, cGrey.b, cGrey.a);
//                    }

//                    SDL_RenderFillRect(GUI_renderer, &rect);
                    
                    SkColor fc = isFocus() ? cTextSelected : cGrey;
                    GUI_FillRect(rect.x, rect.y, rect.w, rect.h, fc);
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
            const char * text = title.substr(textSelectionScrollIndex, textEditIndex - textSelectionScrollIndex).c_str();
            auto w = font.measureText(text, strlen(text), SkTextEncoding::kUTF8);
            int x = (_padding[3] * GUI_scale) + w;
            GUI_DrawLine(x, _padding[0] * GUI_scale, x, rectView.height() - (_padding[2] + 2) * GUI_scale, cBlack);
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

			if (!isFocus()) {
				return false;
			}

			if (!isVisible()) {
				return false;
			}

			if (!isEnable()) {
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
//                        TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex, textEditIndex - textSelectionScrollIndex).c_str(), &w, &h);
                        const char* txt = title.substr(textSelectionScrollIndex, textEditIndex - textSelectionScrollIndex).c_str();
                        w = font.measureText(txt, strlen(txt), SkTextEncoding::kUTF8);
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
//                            int h = 0;
//                            TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex).c_str(), &w, &h);
                            const char* txt = title.substr(textSelectionScrollIndex).c_str();
                            w = font.measureText(txt, strlen(txt), SkTextEncoding::kUTF8);
                            title = title.substr(0, textEditIndex) + title.substr(oi);

                            if (w <= areaWidth) {
                                int prevIndex = textSelectionGetPreviousMiddleCharacter(textSelectionScrollIndex);
//                                TTF_SizeUTF8(font, title.substr(prevIndex).c_str(), &w, &h);
                                const char* txt = title.substr(prevIndex).c_str();
                                w = font.measureText(txt, strlen(txt), SkTextEncoding::kUTF8);
                                if (w <= areaWidth) {
                                    textSelectionScrollIndex = prevIndex;
                                }
                            } else if (textEditIndex < textSelectionScrollIndex) {
                                w = 0;
                                while (textSelectionScrollIndex > 0 && w < areaWidth / 2) {
                                    textSelectionScrollIndex = textSelectionGetPreviousMiddleCharacter(textSelectionScrollIndex);
//                                    TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex, textEditIndex - textSelectionScrollIndex).c_str(), &w, &h);
                                    const char* txt = title.substr(textSelectionScrollIndex, textEditIndex - textSelectionScrollIndex).c_str();
                                    w = font.measureText(txt, strlen(txt), SkTextEncoding::kUTF8);
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

			if (!isVisible()) {
				return false;
			}

			if (!isEnable()) {
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

			if (!isFocus()) {
				return false;
			}

			if (!isVisible()) {
				return false;
			}

			if (!isEnable()) {
				textSelectionCancel();
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

        case SDL_MOUSEBUTTONDOWN:
        {
			if (!isVisible()) {
				return false;
			}

			touchTime = (float)SDL_GetTicks();

			if (!isEnable()) {
				textSelectionCancel();
				return false;
			}
			
			GUI_TextView::eventHandler(event);
            SDL_MouseButtonEvent e = event->button;
            int x = (int)(e.x*GUI_mouseScale);
            int y = (int)(e.y*GUI_mouseScale);
            
			textSelectionIsShiftPressed = false;
			
			if (hitTest(x, y, false)) {
				bool setCaret = false;
				
				if (e.button == SDL_BUTTON_LEFT) {
					if (_focus) {
						textSelectionCancel();
						SDL_StartTextInput();
						setCaret = true;
					} else {
						setFocus();
						setCaret = true;
					}
				} else if (e.button == SDL_BUTTON_RIGHT) {
					if (!_focus) {
						setFocus();
						setCaret = true;
					}
				}
			
				if (setCaret) {
					textSelectionSetCaretPosition(x, y);
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

			if (!isFocus()) {
				return false;
			}

			if (!isVisible()) {
				return false;
			}

			if (!isEnable()) {
				textSelectionCancel();
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

			if (!isFocus()) {
				return false;
			}

			if (!isVisible()) {
				return false;
			}

			if (!isEnable()) {
				textSelectionCancel();
				return false;
			}
			
			SDL_MouseButtonEvent e = event->button;
            int x = (int)(e.x * GUI_mouseScale);
            int y = (int)(e.y * GUI_mouseScale);

			if (e.button == SDL_BUTTON_LEFT || e.button == SDL_BUTTON_RIGHT) {
				if (isFocus()) {
					if (e.button == SDL_BUTTON_RIGHT && !textSelectionIsSelected()) {
						textSelectionSetCaretPosition(x, y);
					}

					textSelectionIsMouseDown = false;
					if (textSelectionIsSelected() || e.button == SDL_BUTTON_RIGHT || (float)SDL_GetTicks() - touchTime >= 500.0f) {
						if (contextualMenu) {
							x = (x / GUI_scale) + 1;
							y = (y / GUI_scale) + 1;

							int left = (rectView.x / GUI_scale) + _padding[0];
							int right = left + (rectView.width() / GUI_scale) - 1 - _padding[2];
							int top = (rectView.y / GUI_scale) + _padding[1];
							int bottom = top + (rectView.height() / GUI_scale) - 1 - _padding[3];

							if (x < left) {
								x = left;
							} else if (x > right) {
								int different = x - right;
								x -= different;
							}

							if (y < top) {
								y = top;
							} else if (y > bottom) {
								int different = y - bottom;
								y -= different;
							}

							right = x + (contextualMenu->rectView.width() / GUI_scale) - 1;
							bottom = y + (contextualMenu->rectView.height() / GUI_scale) - 1;

							int windowLeft = (GUI_topView->rectView.x / GUI_scale) + GUI_topView->_padding[0];
							int windowRight = windowLeft + (GUI_topView->rectView.width() / GUI_scale) - 1 - GUI_topView->_padding[1];
							int windowTop = (GUI_topView->rectView.y / GUI_scale) + GUI_topView->_padding[0];
							int windowBottom = windowTop + (GUI_topView->rectView.height() / GUI_scale) - 1 - GUI_topView->_padding[1];

							if (x < windowLeft) {
								x = windowLeft;
							} else if (right > windowRight) {
								int different = right - windowRight;
								x -= different;
							}

							if (y < windowTop) {
								y = windowTop;
							} else if (bottom > windowBottom) {
								int different = bottom - windowBottom;
								y -= different;
							}

							contextualMenu->setAbsolutePosition(x, y);

							for (std::vector<GUI_MenuItem *>::iterator it = contextualMenu->menuItems.begin(); it != contextualMenu->menuItems.end(); ++it) {
								GUI_MenuItem *c = *it;
								if (c->title == "Undo") {
									c->setEnable(true);
								} else if (c->title == "Cut") {
									c->setEnable(GUI_lastEditTextView ? GUI_lastEditTextView->canCut() : false);
								} else if (c->title == "Copy") {
									c->setEnable(GUI_lastEditTextView ? GUI_lastEditTextView->canCopy() : false);
								} else if (c->title == "Paste") {
									c->setEnable(GUI_lastEditTextView ? GUI_lastEditTextView->canPaste() : false);
								} else if (c->title == "Delete") {
									c->setEnable(GUI_lastEditTextView ? GUI_lastEditTextView->canDelete() : false);
								} else if (c->title == "Select All") {
									c->setEnable(GUI_lastEditTextView ? GUI_lastEditTextView->canSelectAll() : false);
								}
							}

							contextualMenu->show();
							GUI_SetMouseCapture(contextualMenu);
						}
					}
					return false;
				} else {
					textSelectionCancel();
				}
			}

			break;
        }

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
    //int left = rectView.x() + topLeft.x;
    int left = rectView.x + (_padding[3] * GUI_scale);
    int j = 0;
    //x += 5;

    for (int i = textSelectionScrollIndex; i < (int)title.length(); i++) {
        //unsigned char c = title.at(i);
        int currentCharWidth = 0;
//        int h = 0;
//        TTF_SizeUTF8(font, title.substr(i, 1).c_str(), &currentCharWidth, &h);
        const char* txt0 = title.substr(i, 1).c_str();
        currentCharWidth = font.measureText(txt0, strlen(txt0), SkTextEncoding::kUTF8);

        int w = 0;
        j = textSelectionGetNextIndex(i) - i;
//        TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex, i + j - textSelectionScrollIndex).c_str(), &w, &h);
        const char* txt = title.substr(textSelectionScrollIndex, i + j - textSelectionScrollIndex).c_str();
        w = font.measureText(txt, strlen(txt), SkTextEncoding::kUTF8);

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
//        TTF_SizeUTF8(font, title.substr(i, textEditIndex - i).c_str(), &w, &h);
        const char* txt = title.substr(i, textEditIndex - i).c_str();
        w = font.measureText(txt, strlen(txt), SkTextEncoding::kUTF8);
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
    int areaWidth = rectView.width() - ((_padding[3] + _padding[1]) * GUI_scale);
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
//        TTF_SizeUTF8(font, title.substr(startIndex, endIndex - startIndex).c_str(), &w, &h);
        const char* txt = title.substr(startIndex, endIndex - startIndex).c_str();
        w = font.measureText(txt, strlen(txt), SkTextEncoding::kUTF8);
        rect->x = _padding[3] * GUI_scale;
        rect->y = _padding[0] * GUI_scale;
        rect->w = w;
        rect->h = h;

        if (startIndex > textSelectionScrollIndex) {
//            TTF_SizeUTF8(font, title.substr(textSelectionScrollIndex, startIndex - textSelectionScrollIndex).c_str(), &w, &h);
            const char* txt = title.substr(textSelectionScrollIndex, startIndex - textSelectionScrollIndex).c_str();
            w = font.measureText(txt, strlen(txt), SkTextEncoding::kUTF8);
            rect->x += w;
        }

        success = true;
    }

    return success;
}

void GUI_EditText::textSelectionSetCaretPosition(int x, int y) {
    textSelectionIsMouseDown = true;
    textEditIndex = textSelectionGetIndexAtScreenPosition(x);
    textSelectionStartIndex = textEditIndex;
    textSelectionEndIndex = textSelectionStartIndex;
    textSelectionMousePoint.x = x;
    textSelectionMousePoint.y = y;
}

bool GUI_EditText::canCut() {
	return textSelectionIsSelected();
}

bool GUI_EditText::canCopy() {
	return textSelectionIsSelected();
}

bool GUI_EditText::canPaste() {
	return textSelectionIsValidClipboardText();
}

bool GUI_EditText::canDelete() {
	return textSelectionIsSelected();
}

bool GUI_EditText::canSelectAll() {
	int startIndex = 0;
	int endIndex = 0;
	textSelectionGetSelectedIndex(&startIndex, &endIndex);
	return (title.length() > 0 && !(startIndex == 0 && endIndex == title.length()));
}

void GUI_EditText::createContextualMenu() {
	if (contextualMenu == NULL && GUI_topView) {
		contextualMenu = GUI_PopupMenu::create(GUI_topView, "contextualMenu", 0, 0, 160, 0);
		contextualMenu->hide();
		contextualMenu->addSimpleMenu("Cut");
		contextualMenu->addSimpleMenu("Copy");
		contextualMenu->addSimpleMenu("Paste");
		contextualMenu->addSimpleMenu("Delete", true);
		contextualMenu->addSimpleMenu("Select All");

		contextualMenu->setCallback([=](GUI_View *v) {
			if (GUI_lastEditTextView) {
				GUI_PopupMenu *pm = (GUI_PopupMenu *)v;
				GUI_MenuItem *it = pm->selectedItem;
				it->setSelected(false);
				GUI_MenuItem* selectedItem = pm->selectedItem;
				pm->selectedItem = NULL;
				pm->hide();

				if (selectedItem->title == "Cut") {
					GUI_lastEditTextView->textSelectionCut();
				} else if (selectedItem->title == "Copy") {
					GUI_lastEditTextView->textSelectionCopy();
				} else if (selectedItem->title == "Paste") {
					GUI_lastEditTextView->textSelectionPaste();
				} else if (selectedItem->title == "Delete") {
					GUI_lastEditTextView->textSelectionDelete();
				} else if (selectedItem->title == "Select All") {
					GUI_lastEditTextView->textSelectionSelectAll();
				}
			}
		});
	}
}
