#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
#include "CallState.hpp"
namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
    context.timer.stopTimer();
}

void ConnectedState::handleDisconnected()
{
    logger.logInfo("Disconnected");
    context.setState<NotConnectedState>();
}

void ConnectedState::handleSendSms(common::PhoneNumber rNumber, std::string msg)
{
    context.bts.sendSms(rNumber, msg);
    context.db.saveMessageToDb(rNumber, msg, outgoing);
}

void ConnectedState::handleReceiveSms(common::PhoneNumber sNumber, std::string msg)
{
    context.db.saveMessageToDb(sNumber, msg, incoming);
    context.user.showNewSms();
}

std::vector<message> ConnectedState::retrieveMessages()
{
    return context.db.getAllMessages();
}

void ConnectedState::setMessageAsRead(int msgId)
{
    context.db.markInDbAsRead(msgId);
}

void ConnectedState::deleteAllMessages()
{
    context.db.removeAllMessages();
}

void ConnectedState::handleCallRequest(common::PhoneNumber number)
{
    using namespace std::chrono_literals;
    context.timer.startTimer(40000ms);
    context.user.showIncomingCallRequest(number);
}

void ConnectedState::callResponse(common::PhoneNumber number, Call isAcceptedOrRejected)
{
      context.bts.callResponse(number, isAcceptedOrRejected);
      if (isAcceptedOrRejected == Call::rejected)
      {
          context.user.showDefaultView();
      }
      else
      {
          context.setState<CallState>();
      }
      context.timer.stopTimer();
}

void ConnectedState::handleSendCallRequest(common::PhoneNumber rNumber)
{
    context.bts.sendCallRequest(rNumber);
    using namespace std::chrono_literals;
    context.timer.startTimer(60s);
}

void ConnectedState::handleReceiveAcceptedCall(common::PhoneNumber number)
{
    // TODO
}

void ConnectedState::handleReceiveDroppedCall(common::PhoneNumber number)
{
    // TODO
}

void ConnectedState::handleTimeout()
{
    //common::PhoneNumber rNumber = context.user.getNumber();
    context.timer.stopTimer();
    //context.user.showUserNotResponding(rNumber);
}

}
