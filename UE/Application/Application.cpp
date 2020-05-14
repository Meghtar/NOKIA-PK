#include "Application.hpp"
#include "States/NotConnectedState.hpp"

namespace ue
{

Application::Application(common::PhoneNumber phoneNumber,
                         common::ILogger &iLogger,
                         IBtsPort &bts,
                         IUserPort &user,
                         ITimerPort &timer,
                         IDbPort &db)
    : context{iLogger, bts, user, timer, db},
      logger(iLogger, "[APP] ")
{
    logger.logInfo("Started");
    context.setState<NotConnectedState>();
}

Application::~Application()
{
    logger.logInfo("Stopped");
}

void Application::handleTimeout()
{
    context.state->handleTimeout();
}

void Application::handleDisconnected()
{
    context.state->handleDisconnected();
}

void Application::handleSib(common::BtsId btsId)
{
    context.state->handleSib(btsId);
}

void Application::handleAttachAccept()
{
    context.state->handleAttachAccept();
}

void Application::handleAttachReject()
{
    context.state->handleAttachReject();
}

void Application::handleSendSms(common::PhoneNumber rNumber, std::string message)
{
    context.state->handleSendSms(rNumber, message);
}


void Application::handleReceiveSms(common::PhoneNumber sNumber, std::string message)
{
    context.state->handleReceiveSms(sNumber, message);
}

std::vector<message> Application::retrieveMessages()
{
    return context.state->retrieveMessages();
}

void Application::setMessageAsRead(int msgId)
{
    context.state->setMessageAsRead(msgId);
}

void Application::deleteAllMessages()
{
    context.state->deleteAllMessages();
}

void Application::handleCallRequest(common::PhoneNumber number)
{
    context.state->handleCallRequest(number);
}

void Application::callResponse(common::PhoneNumber number, Call isAcceptedOrRejected)
{
    context.state->callResponse(number, isAcceptedOrRejected);
}

void Application::handleSendCallRequest(common::PhoneNumber rNumber)
{
    context.state->handleSendCallRequest(rNumber);
}

//void Application::handleSendCallDrop(common::PhoneNumber rNumber)
//{
//    context.state->handleSendCallDrop(rNumber);
//}
}
