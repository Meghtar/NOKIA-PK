#pragma once

#include "ITimerPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include <thread>

namespace ue
{

class TimerPort : public ITimerPort
{
public:
    TimerPort(common::ILogger& logger);

    void start(ITimerEventsHandler& handler);
    void stop();

    // ITimerPort interface
    void startTimer(Duration duration) override;
    void stopTimer() override;

private:
    common::PrefixedLogger logger;
    ITimerEventsHandler* handler = nullptr;

    std::thread timerThread;
    void waitTillTimeout(Duration duration);
};

}
