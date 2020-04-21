#include "UserPort.hpp"
#include "UeGui/IListViewMode.hpp"

namespace ue
{

UserPort::UserPort(common::ILogger &logger, IUeGui &gui, common::PhoneNumber phoneNumber)
    : logger(logger, "[USER-PORT]"),
      gui(gui),
      phoneNumber(phoneNumber)
{}

void UserPort::start(IUserEventsHandler &handler)
{
    this->handler = &handler;
    gui.setTitle("Nokia " + to_string(phoneNumber));
    //gui.setRejectCallback([&]() { showConnected();});
}

void UserPort::stop()
{
    handler = nullptr;

}

void UserPort::showNotConnected()
{
    gui.showNotConnected();
}

void UserPort::showConnecting()
{
    gui.showConnecting();
}

void UserPort::showConnected()
{
    IUeGui::IListViewMode& menu = gui.setListViewMode();
    menu.clearSelectionList();
    menu.addSelectionListItem("Compose SMS", "");
    menu.addSelectionListItem("View SMS", "");
    /*
    gui.setAcceptCallback([&]() {

        switch(menu.getCurrentItemIndex().second){
        case 0:
            handleComposeSms();
            break;
        case 1:

            handleSmsListView();
            break;

        }
    });
    */

}

void UserPort::handleComposeSms()
{
    IUeGui::ISmsComposeMode& smsView = gui.setSmsComposeMode();
    //IUeGui::ISmsComposeMode* composeSms;

    //auto rNumber = composeSms->getPhoneNumber();
    //auto msg =composeSms->getSmsText();
    //handler->handleSendSms(rNumber, msg);

}

void UserPort::handleSmsListView() {
    IUeGui::ITextMode& viewSms = gui.setViewTextMode();
}


}
