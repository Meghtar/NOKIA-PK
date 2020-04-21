#pragma once

#include "Ports/ITimerPort.hpp"
#include "Ports/IBtsPort.hpp"
#include "Ports/IUserPort.hpp"
#include "Ports/IDbPort.hpp"


namespace ue
{

class IEventsHandler : public IBtsEventsHandler,
                       public IUserEventsHandler,
                       public ITimerEventsHandler
{};

}
