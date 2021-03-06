#include "CallState.hpp"

namespace ue {
    CallState::CallState(Context& context, common::PhoneNumber number)
        : BaseState(context, "CallState"),
          receivingNumber(number)
    {
        context.user.showCallView();
    }

    void CallState::handleCallTalk(std::string message)
    {
        logger.logDebug("Going to send ",message," to ",receivingNumber);
        context.bts.sendCallTalk(message, receivingNumber);
    }

    void CallState::handleReceiveCallTalk(common::PhoneNumber number, std::string message)
    {
        logger.logDebug("Received ",message," from ", number);
        context.user.showNewCallTalk(number, message);
    }

    void CallState::handleSendCallDrop(common::PhoneNumber number)
    {
        logger.logDebug("Send drop");
        context.user.showConnected();
        context.setState<ConnectedState>();
    }

    void CallState::handleReceiveDroppedCall(common::PhoneNumber number)
    {
        logger.logDebug("Received call drop");
        context.user.showConnected();
        context.setState<ConnectedState>();
    }
}
