#ifndef CONTACTEDIT_H
#define CONTACTEDIT_H

#include "SDL_gui.h"
#include "AbstractWindow.h"
#include "Contact.h"

class MainWindow;

class ContactEdit : public AbstractWindow
{
public:
    static ContactEdit* create(ContactPtr aContact, GUI_View *aParent
        , const char *aTitle, int x = 0, int y = 0
        , int aWidth = 0, int aHeight = 0
        , std::function<bool(SDL_Event* ev)> aUserEventHandler = nullptr);
    ContactEdit(ContactPtr aContact, GUI_View *aParent, const char *aTitle
        , int x = 0, int y = 0, int aWidth = 0
        , int aHeight = 0
        , std::function<bool(SDL_Event* ev)> aUserEventHandler = nullptr);

    void createContent();

    GUI_Label* mNameLabel = nullptr;
    GUI_EditText* mNameEdit = nullptr;
    GUI_Label* mIconLabel = nullptr;
    GUI_EditText* mIconEdit = nullptr;
    GUI_Label* mMobileLabel = nullptr;
    GUI_EditText* mMobileEdit = nullptr;
    GUI_Label* mEmailLabel = nullptr;
    GUI_EditText* mEmailEdit = nullptr;
    GUI_Label* mAddressLabel = nullptr;
    GUI_EditText* mAddressEdit = nullptr;
    GUI_Label* mChildCountLabel = nullptr;
    GUI_EditText* mChildCountEdit = nullptr;
    GUI_Button* mSaveButton = nullptr;
    GUI_FPSView *fps = nullptr;

protected:
    void createWindowMenu();
    void menuItemClicked(GUI_MenuItem* aItem);
    void withdrawChangesClicked(GUI_MenuItem* aItem);
    void clearDataClicked(GUI_MenuItem* aItem);
    void deleteContactClicked(GUI_MenuItem* aItem);
    void saveContact();
protected:
    ContactPtr mContact;
};

#endif // CONTACTEDIT_H
