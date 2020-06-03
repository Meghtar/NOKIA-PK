#include "BaseState.hpp"

namespace ue
{

BaseState::BaseState(Context &context, const std::string &name)
    : context(context),
      logger(context.logger, "[" + name + "]")
{
    logger.logDebug("entry");
}

BaseState::~BaseState()
{
    logger.logDebug("exit");
}

void BaseState::handleTimeout()
{
    logger.logError("Uexpected: handleTimeout");
}

void BaseState::handleDisconnected()
{
    logger.logError("Uexpected: handleDisconnected");
}

void BaseState::handleSib(common::BtsId btsId)
{
    logger.logError("Uexpected: handleSib: ", btsId);
}

void BaseState::handleAttachAccept()
{
    logger.logError("Uexpected: handleAttachAccept");
}

void BaseState::handleAttachReject()
{
    logger.logError("Uexpected: handleAttachReject");
}

void BaseState::handleSendSms(common::PhoneNumber rNumber, std::string msg)
{
    logger.logError("Unexpected: handleSendSms number: ", rNumber, " content: ", msg);
}

void BaseState::handleReceiveSms(common::PhoneNumber sNumber, std::string msg)
{
    logger.logError("Unexpected: handleReceiveSms number: ", sNumber, " content: ", msg);
}

std::vector<message> BaseState::retrieveMessages()
{
    logger.logError("Unexpected: retrieveMessages");
}

void BaseState::setMessageAsRead(int msgId)
{
    logger.logError("Unexpected: setMessageAsRead", msgId);
}

void BaseState::deleteAllMessages()
{
    logger.logError("Unexpected: deleteAllMessages");
}

void BaseState::handleCallRequest(common::PhoneNumber number)
{
    logger.logError("Unexpected: handleCallRequest ", number);
}

void BaseState::callResponse(common::PhoneNumber number, Call a)
{
    logger.logError("Unexpected: callResponse ", number);
}

void BaseState::handleSendCallRequest(common::PhoneNumber rNumber)
{
    logger.logError("Unexpected: handleCallReguest", rNumber);
}

void BaseState::handleReceiveAcceptedCall(common::PhoneNumber number)
{
    logger.logError("Unexpected: handleReceiveAcceptedCall ", number);
}

void BaseState::handleReceiveDroppedCall(common::PhoneNumber number)
{
    logger.logError("Unexpected: handleReceiveDroppedCall ", number);
}

//void BaseState::handleSendCallDrop(common::PhoneNumber rNumber)
//{
//    logger.logError("Unexpected: handleCallDrop", rNumber);
//}

void BaseState::handleCallTalk(std::string message)
{
    logger.logError("Unexpected: handleCallTalk");
}

void BaseState::handleReceiveCallTalk(common::PhoneNumber number, std::string message)
{
    logger.logError("Unexpected: handleReceiveCallTalk from", number);
}
}
