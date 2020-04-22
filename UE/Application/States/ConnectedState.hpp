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
    std::vector<message> retrieveMessages() override;
};

}
