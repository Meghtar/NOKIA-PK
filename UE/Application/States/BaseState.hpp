#pragma once

#include "IEventsHandler.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "Context.hpp"

namespace ue
{

class BaseState : public IEventsHandler
{
public:
    BaseState(Context& context, const std::string& name);
    ~BaseState() override;

    // ITimerEventsHandler interface
    void handleTimeout() override;

    // IBtsEventsHandler interface
    void handleDisconnected() override;
    void handleSib(common::BtsId btsId) override;
    void handleAttachAccept() override;
    void handleAttachReject() override;
    void handleSendSms(common::PhoneNumber rNumber, std::string msg) override;
    void handleReceiveSms(common::PhoneNumber sNumber, std::string msg) override;
    std::vector<message> retrieveMessages() override;
    void setMessageAsRead(int) override;
    void deleteAllMessages() override;
    void callResponse(common::PhoneNumber, Call) override;
    void handleCallRequest(common::PhoneNumber) override;
    void handleSendCallRequest(common::PhoneNumber) override;
    void handleSendCallDrop(common::PhoneNumber) override;

    void handleReceiveAcceptedCall(common::PhoneNumber) override;
    void handleReceiveDroppedCall(common::PhoneNumber) override;
    void handleCallTalk(std::string) override;
    void handleReceiveCallTalk(common::PhoneNumber, std::string) override;

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
