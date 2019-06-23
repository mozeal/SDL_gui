//
//  GUI_EditText.hpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 17/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_EditText_hpp
#define GUI_EditText_hpp

#include <stdio.h>
#include "GUI_TextView.h"
#include "GUI_MenuBar.h"

#if defined(__MACOSX__)
#define CUSTOM_KMOD_CTRL (KMOD_LGUI|KMOD_RGUI)
#else

#if defined(WIN32)
#define _WINSOCKAPI_
#include <Windows.h>
#endif

#define CUSTOM_KMOD_CTRL KMOD_CTRL
#endif

class GUI_EditText : public GUI_TextView {
protected:
    virtual void draw();
    int textEditIndex;
public:
	static GUI_EditText *create(GUI_View *parent, const char *title, int x = 0, int y = 0, int width = 0, int height = 0,
		                        std::function<void(GUI_View*)>callbackFunction = NULL);
	GUI_EditText(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                 std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_EditText();
    
    virtual bool eventHandler(SDL_Event*event);

    bool textSelectionIsShiftPressed;
    bool textSelectionIsMouseDown;
    SDL_Point textSelectionMousePoint;
    int textSelectionGetIndexAtScreenPosition(int x);
    int textSelectionGetNextIndex(int i);
    int textSelectionGetPreviousIndex(int i);
    void textSelectionSetScrollIndex(bool isResetScrollIndex = false);
    bool textSelectionIsUpperLowerCharacter(int *i);
    int textSelectionGetNextMiddleCharacter(int i);
    int textSelectionGetPreviousMiddleCharacter(int i);
    void textSelectionMouseMove();
    void textSelectionUpdate(bool isResetScrollIndex = false);
    void textSelectionSelectAll(); // Windows: Ctrl+A, Mac: Command+A
    void textSelectionCut();       // Windows: Ctrl+X, Mac: Command+X
    void textSelectionCopy();      // Windows: Ctrl+C, Mac: Command+C
    void textSelectionPaste();     // Windows: Ctrl+V, Mac: Command+V
    void textSelectionDelete();    // Delete, Backspace
    virtual void textSelectionCancel();
    void textSelectionGoToIndex(int i);
    void textSelectionHome();
    void textSelectionEnd();
    int textSelectionGetAreaWidth();
    bool textSelectionIsThaiText(std::string text);
    bool textSelectionIsValidClipboardText();
    bool textSelectionIsDigitText(std::string strText);
    bool textSelectionGetRect(SDL_Rect* rect);
    void textSelectionSetCaretPosition(int x, int y);
    void setFocus();
	bool canCut();
	bool canCopy();
	bool canPaste();
	bool canDelete();
	bool canSelectAll();
	void createContextualMenu();
	float touchTime;
};

#endif /* GUI_EditText_hpp */
