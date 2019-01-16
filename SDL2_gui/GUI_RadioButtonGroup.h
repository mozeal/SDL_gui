//
//  GUI_RadioButtonGroup.hpp
//  GUI_CheckBox
//
//  Created by Panutat Tejasen on 16/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_RadioButtonGroup_hpp
#define GUI_RadioButtonGroup_hpp

#include <stdio.h>
#include "GUI_RadioButton.h"

class GUI_RadioButtonGroup : public GUI_View {
protected:
    std::vector<GUI_RadioButton *>radioButtonChildren;
public:
    static GUI_RadioButtonGroup *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                                 std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_RadioButtonGroup(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                  std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_RadioButtonGroup();
    
    virtual void add(GUI_RadioButton* child);
    virtual void remove(GUI_RadioButton* child);

    GUI_RadioButton *selectedRadioButton;
};

#endif /* GUI_RadioButtonGroup_hpp */
