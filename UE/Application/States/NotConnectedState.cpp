#include "NotConnectedState.hpp"
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
    context.setState<ConnectedState>(btsId);
}

}
