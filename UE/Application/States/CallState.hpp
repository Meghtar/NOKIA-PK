#pragma once

#include "BaseState.hpp"

namespace ue {

class CallState : public BaseState{
public:
    CallState(Context& context, common::PhoneNumber number);
    void handleCallTalk(std::string) override;
    void handleReceiveCallTalk(common::PhoneNumber, std::string) override;
private:
    common::PhoneNumber receivingNumber;
};

}
