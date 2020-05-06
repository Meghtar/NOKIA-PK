#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"
#include "UeGui/ITextMode.hpp"

#include <string>

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber),
      currentView(View::Default)
{}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
    gui.setAcceptCallback([&]() {handleAcceptClick();});
    gui.setRejectCallback([&]() {handleRejectClick();});
    // TODO: add stuff connected to home button
}

void UserPort::stop()
{
    handler = nullptr;
    gui.setAcceptCallback(nullptr);
    gui.setRejectCallback(nullptr);
}

void UserPort::handleAcceptClick()
{
    switch(currentView)
    {
        case View::Menu:
            {
                auto option = static_cast<Option>((currentList->getCurrentItemIndex()).second);
                if (option == Option::ComposeSms)
                {
                    showComposeSms();
                }
                else if (option == Option::ShowSms)
                {
                    showSmsList();
                }
                else if (option == Option::DeleteAllSms)
                {
                    // mostly a debug feature
                    handler->deleteAllMessages();
                }
                break;
            }
        case View::ListSms:
            {
                auto currentSmsId = (currentList->getCurrentItemIndex()).second;
                logger.logInfo(std::to_string(currentSmsId));
                showSms(currentSmsId);
                break;
            }
        case View::ComposeSms:
            {
                IUeGui::ISmsComposeMode& smsView = gui.setSmsComposeMode();
                auto number = smsView.getPhoneNumber();
                auto msg = smsView.getSmsText();
                handler->handleSendSms(number, msg);
                smsView.clearSmsText();
                showMenu();
                break;
            }
    }
}

void UserPort::handleRejectClick()
{
    switch(currentView)
    {
        case View::ComposeSms:
        {
            showMenu();
            break;
        }
        case View::ListSms:
        {
            showMenu();
            break;
        }
        case View::ReadSms:
        {
            showSmsList();
            break;
        }
    }
}

// void UserPort::handleHomeClick() {} TODO

void UserPort::showNotConnected()
{
    currentView = View::Default;
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    currentView = View::Default;
    gui.showConnecting();
}

void UserPort::showConnected()
{
    currentView = View::Menu;
    showMenu();
}

void UserPort::showNewSms()
{
    gui.showNewSms();
}

void UserPort::showNoNewSms()
{
    gui.showNoNewSms();
}

void UserPort::showComposeSms()
{
    currentView = View::ComposeSms;
    IUeGui::ISmsComposeMode& smsView = gui.setSmsComposeMode();
    smsView.clearSmsText();
    // TODO: this probably shouldn't be final solution, draft should be kept somewhere
}

void UserPort::showMenu()
{
    currentView = View::Menu;
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    menu.addSelectionListItem("Delete all SMS", "Not implemented yet");
    currentList = &menu;
}

void UserPort::showSmsList() {
    currentView = View::ListSms;
    IUeGui::IListViewMode& smsList = gui.setListViewMode();
    smsList.clearSelectionList();
    auto listOfMessages = handler->retrieveMessages();
    for (const auto& msg : listOfMessages)
    {
        std::string preview;
        if(msg.read == false)
            preview += "[NEW]";
        preview += msg.receiverNumber == phoneNumber.value ?
                    "[FROM:" + std::to_string(msg.senderNumber) + "]" :
                    "[TO:" + std::to_string(msg.receiverNumber) + "]";
        logger.logInfo(preview);
        smsList.addSelectionListItem(preview, "");
    }
    currentList = &smsList;
    showNoNewSms();
}

void UserPort::showSms(unsigned id) {
    // this approach requires two calls to db, but i'm not sure
    // if we should store all messages all the time in userport
    currentView = View::ReadSms;
    IUeGui::ITextMode& readSms = gui.setViewTextMode();
    auto listOfMessages = handler->retrieveMessages();
    std::string text = listOfMessages[id].text;
    handler->setMessageAsRead(listOfMessages[id].messageId);
    readSms.setText(text);
}

}
