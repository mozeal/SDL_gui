#ifndef APP_H
#define APP_H

#include "GUI_App.h"

class AbstractWindow;

class App : public GUI_App
{
public:
    static App* instance;
    static App* create(std::string aTitle, int aExpectedWidth = 0
        , int aExpectedHeight = 0, int aOrientation = 0, int aOptions = 0);
    App(int aOrientation, std::string aTitle, int aExpectedWidth = 0
            , int aExpectedHeight = 0, int aOptions = 0);
    ~App();
public:
    void addWindow(AbstractWindow* aWin);
    void closeWindow(AbstractWindow* aWin);
protected:
    virtual void createSystemMenu(int aOptions);
    void createBackButton(int aOptions);
    void addWindowMenu(AbstractWindow* aWin);
    void removeWindowMenu(AbstractWindow* aWin);
    void backButtonCallback(GUI_View* aButton);
    void menuItemClicked(GUI_View *aItem);
    void systemMenuClicked(GUI_View *aMenu);
    void systemMenuItemClicked(GUI_MenuItem* aItem);
public:
    std::list<AbstractWindow*> mWindows;
protected:
    GUI_Button* mBackButton = nullptr;
    std::list<GUI_MenuItem*> mSystemMenu;   // These buttons are always below
                                            // window buttons
};

#endif // APP_H
