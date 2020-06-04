#pragma once

#include "BaseState.hpp"
#include "ConnectedState.hpp"

namespace ue {

class CallState : public BaseState{
public:
    CallState(Context& context, common::PhoneNumber number);
    void handleCallTalk(std::string) override;
    void handleReceiveCallTalk(common::PhoneNumber, std::string) override;
    void handleSendCallDrop(common::PhoneNumber) override;
    void handleReceiveDroppedCall(common::PhoneNumber) override;
private:
    common::PhoneNumber receivingNumber;
};

}
