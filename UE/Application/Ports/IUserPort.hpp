#pragma once
#include "Messages/PhoneNumber.hpp"
#include "IDbPort.hpp"

namespace ue
{

class IUserEventsHandler
{
public:
    virtual ~IUserEventsHandler() = default;
    virtual void handleSendSms(common::PhoneNumber, std::string) = 0;
    virtual std::vector<message> retrieveMessages() = 0;
    virtual void setMessageAsRead(int) = 0;
    virtual void deleteAllMessages() = 0;
    virtual void callResponse(common::PhoneNumber, bool) = 0;
};

class IUserPort
{
public:
    virtual ~IUserPort() = default;

    virtual void showNotConnected() = 0;
    virtual void showConnecting() = 0;
    virtual void showConnected() = 0;
    virtual void showNewSms() = 0;
    virtual void showNoNewSms() = 0;
    virtual void showIncomingCallRequest(common::PhoneNumber) = 0;
    virtual void showCallView() = 0;
};

}
