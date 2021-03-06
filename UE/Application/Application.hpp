#pragma once

#include "Logger/PrefixedLogger.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IEventsHandler.hpp"
#include "Context.hpp"

namespace ue
{

using common::PhoneNumber;
using common::ILogger;

class Application : public IEventsHandler
{
public:
    Application(PhoneNumber phoneNumber,
                ILogger& iLogger,
                IBtsPort& bts,
                IUserPort& user,
                ITimerPort& timer,
                IDbPort& db);
    ~Application();

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSendSms(common::PhoneNumber, std::string) override;
    void handleReceiveSms(common::PhoneNumber, std::string) override;
    std::vector<message> retrieveMessages() override;
    void setMessageAsRead(int) override;
    void deleteAllMessages() override;
    void handleCallRequest(common::PhoneNumber) override;
    void callResponse(common::PhoneNumber, Call) override;
    void handleSendCallRequest(common::PhoneNumber) override;

    void handleReceiveAcceptedCall(common::PhoneNumber) override;
    void handleReceiveDroppedCall(common::PhoneNumber) override;
    void handleSendCallDrop(common::PhoneNumber) override;
    void handleCallTalk(std::string) override;
    void handleReceiveCallTalk(common::PhoneNumber, std::string) override;

private:
    Context context;
    common::PrefixedLogger logger;

};

}
