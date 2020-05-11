#pragma once

#include "BaseState.hpp"

namespace ue
{

class ConnectedState : public BaseState
{
public:
    ConnectedState(Context& context);
    void handleDisconnected() override;
    void handleSendSms(common::PhoneNumber, std::string msg) override;
    void handleReceiveSms(common::PhoneNumber, std::string msg) override;
    std::vector<message> retrieveMessages() override;
    void setMessageAsRead(int) override;
    void deleteAllMessages() override;
    void handleCallRequest(common::PhoneNumber) override;
    void callResponse(common::PhoneNumber, bool) override;
};

}
