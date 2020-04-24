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

void ConnectedState::handleRecevieSms(common::PhoneNumber sNumber, std::string msg)
{
    context.db.saveMessageToDb(sNumber, msg, 0);
    context.bts.recevieSms(sNumber, msg);
}

}
