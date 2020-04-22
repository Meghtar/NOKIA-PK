#include "ConnectedState.hpp"
#include "NotConnectedState.hpp"
namespace ue
{

ConnectedState::ConnectedState(Context &context)
    : BaseState(context, "ConnectedState")
{
    context.user.showConnected();
    context.timer.stopTimer();
    //context.db.removeAllMessages(); // AS BELOW
    //context.db.saveMessageToDb(common::PhoneNumber{100}, "XYZ", outgoing); // JUST FOR TESTING PURPOSES
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

std::vector<message> ConnectedState::retrieveMessages()
{
    return context.db.getAllMessages();
}

}
