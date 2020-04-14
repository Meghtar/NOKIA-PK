#include "NotConnectedState.hpp"
#include "ConnectingState.hpp"
#include "ConnectedState.hpp"
namespace ue
{

NotConnectedState::NotConnectedState(Context &context)
    : BaseState(context, "NotConnectedState")
{
    context.user.showNotConnected();
}

void NotConnectedState::handleSib(common::BtsId btsId)
{
    context.logger.logDebug("Received sib: ", btsId);
    context.setState<ConnectingState>(btsId);
}
}
