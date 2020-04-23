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

std::vector<message> BaseState::retrieveMessages()
{
    logger.logError("Unexpected: retrieveMessages");
}

void BaseState::setMessageAsRead(int msgId)
{
    logger.logError("Unexpected: setMessageAsRead", msgId);
}

}
