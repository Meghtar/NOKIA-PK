#pragma once

#include "IUserPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "IUeGui.hpp"
#include "Messages/PhoneNumber.hpp"
#include "UeGui/ISmsComposeMode.hpp"
#include "UeGui/IListViewMode.hpp"

namespace ue
{

enum class View
{
    Default, // TODO: this name should be changed but i don't have better idea now
    Home,
    ComposeSms,
    ListSms,
    ReadSms
};

class UserPort : public IUserPort
{
public:
    UserPort(common::ILogger& logger, IUeGui& gui, common::PhoneNumber phoneNumber);
    void start(IUserEventsHandler& handler);
    void stop();

    void handleAcceptClick();
    void handleRejectClick();

    void showNotConnected() override;
    void showConnecting() override;
    void showConnected() override;

    void showComposeSms();
    void showHome();
    void showSmsList();
    void showSms(unsigned id);

private:
    common::PrefixedLogger logger;
    IUeGui& gui;
    common::PhoneNumber phoneNumber;
    IUserEventsHandler* handler = nullptr;
    View currentView;
    IUeGui::IListViewMode* currentList;
};

}
