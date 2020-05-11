#include "CallState.hpp"

namespace ue {
    CallState::CallState(Context& context)
        : BaseState(context, "CallState")
    {
        //context.user.callView TODO Show call view
    }
}
