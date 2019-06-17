#include <algorithm>
#include "App.h"
#include "AbstractWindow.h"
#include "Tools.h"

using namespace std;

App* App::instance = nullptr;

App* App::create(std::string aTitle, int aExpectedWidth, int aExpectedHeight
    , int aOrientation, int aOptions)
{
#if defined( __ANDROID__ )
    int _orientation = 0; //GUI_ORIENTATION_PORTRAIT | GUI_ORIENTATION_LANDSCAPE;
    if( SDL_IsTablet() ) {
        GUI_Log( "Android: is Tablet\n" );
        _expectedWidth = 480;
        _expectedHeight = 800;
    }
    else {
        GUI_Log( "Android: is Phone\n" );
    }
#elif defined( __EMSCRIPTEN__ )
    _expectedWidth = -1;
    _expectedHeight = -1;
    int _orientation = 0;
#else
    int _orientation = GUI_ORIENTATION_PORTRAIT | GUI_ORIENTATION_LANDSCAPE;
#endif
    if(aExpectedWidth == 0)
        aExpectedWidth = _expectedWidth;
    if(aExpectedHeight == 0)
        aExpectedHeight = _expectedHeight;
    if(aOrientation == 0)
        aOrientation = _orientation;

    if(instance)
        return instance;

    App* lApp = new App(aOrientation, aTitle, aExpectedWidth, aExpectedHeight
        , aOptions);

    if(lApp->topView == nullptr)
    {
        delete lApp;
        lApp = nullptr;
    }

    GUI_App::instance = instance = lApp;
    return lApp;
}

App::App(int aOrientation, std::string aTitle, int aExpectedWidth
    , int aExpectedHeight, int aOptions)
    : GUI_App(aOrientation, aTitle, aExpectedWidth, aExpectedHeight, aOptions)
{
    createBackButton(aOptions);
    createSystemMenu(aOptions);

    if(menuView)
    {
        menuView->setCallback(std::bind(&App::systemMenuClicked, this
            , std::placeholders::_1));
        menuView->callback_on_mouse_down = true;
    }
}

App::~App()
{
    for(auto w : mWindows)
        delete w;
    mWindows.clear();
}

void App::createSystemMenu(int aOptions)
{
    mSystemMenu.push_back(menuView->addSimpleMenu(tr("About app...")));
    mSystemMenu.push_back(menuView->addSimpleMenu(tr("Quit"), true));
}

void App::createBackButton(int aOptions)
{
    if(mBackButton)
        return;
    int lAlign(GUI_ALIGN_VCENTER);
    if((aOptions & GUI_APP_MENU_LEFT) || (aOptions & GUI_APP_MENU_LEFT_TOP)
        || (aOptions & GUI_APP_MENU_LEFT_BOTTOM))
        lAlign |= GUI_ALIGN_RIGHT;
    else
        lAlign |= GUI_ALIGN_LEFT;

    mBackButton = GUI_Button::create(topBar->contentView, NULL
        , kIcon_solid_arrow_left, 0, 0, 0, 0
        , std::bind(&App::backButtonCallback, this, std::placeholders::_1));
    mBackButton->setBorder(0);
    mBackButton->setMargin(0, 0, 0, 5);
    mBackButton->setAlign(lAlign);
    mBackButton->setBackgroundColor(cClear);
    mBackButton->showInteract = false;
}

void App::addWindow(AbstractWindow* aWin)
{
    if(!aWin)
        return;

    if(static_cast<void*>(contentView) != static_cast<void*>(mWindows.back()))
    {
        topView->remove_child(contentView);
        delete contentView;
        contentView = nullptr;
    }

    // Remove window and system menu
    for(auto i : mSystemMenu)
        menuView->remove(i);
    removeWindowMenu(mWindows.back());

    topView->remove_child(mWindows.back());

    // Add window and system menu
    addWindowMenu(aWin);
    for(auto i : mSystemMenu)
    {
        menuView->add(i);
        i->setCallback(std::bind(&App::menuItemClicked, this
                                 , std::placeholders::_1));
    }

    menuView->updateLayout();

    mWindows.push_back(aWin);
    contentView = aWin;

    topView->add_child(aWin);

    if(menuView)
        menuView->toTop();

    mBackButton->setEnable(mWindows.size() > 1);

    topView->updateLayout();
}

void App::closeWindow(AbstractWindow* aWin)
{
    if(!aWin)
        return;

    AbstractWindow* lNextWindow(nullptr);
    if(contentView && (contentView != aWin))
    {
        cerr << tr("Window stack broken!") << std::endl;

        removeWindowMenu(aWin);
        delete aWin;

        while(AbstractWindow* w = mWindows.back())
        {
            topView->remove_child(w);
            mWindows.pop_back();
            if((contentView == w) || (mWindows.size() == 1))
            {
                lNextWindow = mWindows.back();
                break;
            }
        }
    }
    if(contentView != lNextWindow)
    {
        AbstractWindow* lWin(dynamic_cast<AbstractWindow*>(contentView));
        if(lWin)
            removeWindowMenu(lWin);
        topView->remove_child(contentView);
        delete contentView;
        contentView = 0;
        mWindows.pop_back();
    }
    if(!lNextWindow && mWindows.size())
        lNextWindow = mWindows.back();

    if(contentView != lNextWindow)
    {
        addWindowMenu(lNextWindow);
        contentView = lNextWindow;
        topView->add_child(contentView);
    }

    mBackButton->setEnable(mWindows.size() > 1);

    if(menuView)
        menuView->toTop();

    topView->updateLayout();
}

void App::addWindowMenu(AbstractWindow* aWin)
{
    if(!aWin)
        return;

    auto lWinMenu = aWin->menu();
    for(auto i : lWinMenu)
    {
        i->separator = (i == lWinMenu.back());
        menuView->add(i);
        i->setCallback(std::bind(&App::menuItemClicked, this
                                 , std::placeholders::_1));
    }

    menuView->updateLayout();
}

void App::removeWindowMenu(AbstractWindow* aWin)
{
    if(!aWin)
        return;

    auto lWinMenu = aWin->menu();
    for(auto i : lWinMenu)
        menuView->remove(i);

    menuView->updateLayout();
}

void App::backButtonCallback(GUI_View* aButton)
{
    if(mWindows.size() <= 1)
        return;
    closeWindow(mWindows.back());
}

void App::menuItemClicked(GUI_View *aItem)
{
    GUI_MenuItem* lItem(dynamic_cast<GUI_MenuItem*>(aItem));
    if(!lItem)
        return;

    lItem->setSelected(true);

    for(vector<GUI_MenuItem *>::iterator it =
        menuView->menuItems.begin();
        it != menuView->menuItems.end(); ++it)
    {
        if((*it) != lItem)
            (*it)->setSelected(false);
    }
}

void App::systemMenuClicked(GUI_View* aMenu)
{
    GUI_Menu* lMenu = dynamic_cast<GUI_Menu*>(aMenu);
    if(!lMenu)
        return;
    auto lItem(lMenu->selectedItem);
    if(!lItem)
        return;

    if(find(mSystemMenu.begin(), mSystemMenu.end(), lItem)
        != mSystemMenu.end())
    {
        systemMenuItemClicked(lItem);
    }
    else
        mWindows.back()->menuItemClicked(lItem);

    lMenu->close(GUI_AppMenuCollapseTime);
}

void App::systemMenuItemClicked(GUI_MenuItem *aItem)
{
    int lCounter(0);
    for(auto i : mSystemMenu)
    {
        if((lCounter == 0) && (i == aItem))
            mWindows.back()->createPopup(tr("This is fancy mobile demo app!"));
        else if((lCounter == 1) && (i == aItem))
            exit(EXIT_SUCCESS);
        ++lCounter;
    }
}
