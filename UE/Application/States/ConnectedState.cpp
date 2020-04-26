#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
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
}

std::vector<message> ConnectedState::retrieveMessages()
{
    return context.db.getAllMessages();
}

void ConnectedState::setMessageAsRead(int msgId)
{
    context.db.markInDbAsRead(msgId);
}

}
