#include <string>
#include "ContactEdit.h"
#include "Tools.h"
#include "MainWindow.h"
#include "App.h"
#include "ListItem.h"

using namespace std;

ContactEdit* ContactEdit::create(ContactPtr aContact, GUI_View *aParent
    , const char *aTitle, int x, int y, int aWidth
    , int aHeight, std::function<bool(SDL_Event* ev)> aUserEventHandler)
{
    return new ContactEdit(aContact, aParent, aTitle, x, y, aWidth, aHeight,
        aUserEventHandler);
}

ContactEdit::ContactEdit(ContactPtr aContact, GUI_View *aParent
    , const char *aTitle, int x, int y, int aWidth, int aHeight
    , std::function<bool(SDL_Event* ev)> aUserEventHandler)
    : AbstractWindow(aParent, aTitle, x, y, aWidth, aHeight, aUserEventHandler)
    , mContact(aContact)
{
    createWindowMenu();
    createContent();
}

void ContactEdit::createContent()
{
    mNameLabel = GUI_Label::create(this, tr("Name:"));
    mNameEdit = GUI_EditText::create(this, "", 0, 0, -1, 0);
    mIconLabel = GUI_Label::create(this, tr("Icon:"));
    mIconEdit = GUI_EditText::create(this, "", 0, 0, -1, 0);
    mMobileLabel = GUI_Label::create(this, tr("Mobile:"));
    mMobileEdit = GUI_EditText::create(this, "", 0, 0, -1, 0);
    mEmailLabel = GUI_Label::create(this, tr("Email:"));
    mEmailEdit = GUI_EditText::create(this, "", 0, 0, -1, 0);
    mAddressLabel = GUI_Label::create(this, tr("Addres:"));
    mAddressEdit = GUI_EditText::create(this, "", 0, 0, -1, 0);
    mChildCountLabel = GUI_Label::create(this, tr("Children counter:"));
    mChildCountEdit = GUI_EditText::create(this, "", 0, 0, -1, 0);
    fps = GUI_FPSView::create(this);
    mSaveButton = GUI_Button::create(this, tr("Save contact"), kIcon_solid_edit, 0, 0, 0
        , 0, [=](GUI_View* v)
    {
        this->saveContact();
    });


    withdrawChangesClicked(nullptr);
}

void ContactEdit::createWindowMenu()
{
    mMenu.push_back(addMenu(tr("Withdraw changes")));
    mMenu.push_back(addMenu(tr("Clear data")));
    mMenu.push_back(addMenu(tr("Delete contact")));
}

void ContactEdit::menuItemClicked(GUI_MenuItem* aItem)
{
    int lCounter(0);
    for(auto i : mMenu)
    {
        if((lCounter == 0) && (i == aItem))
            withdrawChangesClicked(aItem);
        else if((lCounter == 1) && (i == aItem))
            clearDataClicked(aItem);
        else if((lCounter == 2) && (i == aItem))
            deleteContactClicked(aItem);
        ++lCounter;
    }
}

void ContactEdit::withdrawChangesClicked(GUI_MenuItem* aItem)
{
    mNameEdit->setTitle(mContact->mName);
    mIconEdit->setTitle(mContact->mIcon);
    mMobileEdit->setTitle(mContact->mMobile);
    mEmailEdit->setTitle(mContact->mEmail);
    mAddressEdit->setTitle(mContact->mAddress);
    mChildCountEdit->setTitle(to_string(mContact->mChildCount));
}

void ContactEdit::clearDataClicked(GUI_MenuItem* aItem)
{
    mNameEdit->setTitle("");
    mIconEdit->setTitle("");
    mMobileEdit->setTitle("");
    mEmailEdit->setTitle("");
    mAddressEdit->setTitle("");
    mChildCountEdit->setTitle(to_string(0));
}

void ContactEdit::deleteContactClicked(GUI_MenuItem* aItem)
{
    MainWindow::instance->deleteContactClicked(aItem);
}

void ContactEdit::saveContact()
{
    mContact->mName = mNameEdit->title;
    mContact->mIcon = mIconEdit->title;
    mContact->mMobile = mMobileEdit->title;
    mContact->mEmail = mEmailEdit->title;
    mContact->mAddress = mAddressEdit->title;
    mContact->mChildCount = std::stoi(mChildCountEdit->title);
    App::instance->closeWindow(this);
    MainWindow* lWin(dynamic_cast<MainWindow*>
        (App::instance->mWindows.front()));
    if(!lWin)
        return;
    ListItem* lItem(dynamic_cast<ListItem*>(lWin->mContactList->selectedItem));
    if(!lItem)
        return;
    lItem->updateData();
}
