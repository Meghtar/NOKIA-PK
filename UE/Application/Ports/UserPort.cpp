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
        case View::Home:
            {
                auto option = (currentList->getCurrentItemIndex()).second;
                if (option == /*compose*/0) // TODO: into enum
                {
                    showComposeSms();
                }
                else if (option == /*show sms*/1)
                {
                    showSmsList();
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
        /*case View::ListSms:
            // get current sms
            auto currentSmsId = (currentList->getCurrentItemIndex()).second;
            //showSms(currentSmsId);
        break;*/
    }
}

void UserPort::handleRejectClick()
{
    showHome();
}

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
    currentView = View::Home;
    showHome();
}

void UserPort::showComposeSms()
{
    currentView = View::ComposeSms;
    IUeGui::ISmsComposeMode& smsView = gui.setSmsComposeMode();
    //handler->handleSendSms();
}

void UserPort::showHome()
{
    currentView = View::Home;
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    currentList = &menu;
    //menu.
}

void UserPort::showSmsList() {
    currentView = View::ListSms;
    IUeGui::IListViewMode& smsList = gui.setListViewMode();
    auto listOfMessages = handler->retrieveMessages();
    smsList.clearSelectionList();
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
}

void UserPort::showSms(unsigned id) {
    // this approach requires two calls to db, but i'm not sure
    // if we should store all messages all the time in userport
    currentView = View::ReadSms;
    IUeGui::ITextMode& readSms = gui.setViewTextMode();
    auto listOfMessages = handler->retrieveMessages();
    std::string text = listOfMessages[id].text;
    //handler->setMessageAsRead(); TODO: will need database update method
    readSms.setText(text);
}

}
