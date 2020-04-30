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

protected:
    Context& context;
    common::PrefixedLogger logger;
};

}
