#include "TimerPort.hpp"

namespace ue
{

TimerPort::TimerPort(common::ILogger &logger)
    : logger(logger, "[TIMER PORT]")
{}

void TimerPort::start(ITimerEventsHandler &handler)
{
    logger.logDebug("Started");
    this->handler = &handler;
}

void TimerPort::stop()
{
    logger.logDebug("Stoped");
    handler = nullptr;
    if (timerThread.joinable())
        timerThread.detach();
}

void TimerPort::startTimer(Duration duration)
{
    logger.logDebug("Start timer: ", duration.count(), "ms");
    running = true;
    timerThread = std::thread {&TimerPort::waitTillTimeout, this, duration};
}

void TimerPort::stopTimer()
{
    running = false;
    logger.logDebug("Stop timer");
    if(timerThread.joinable())
        timerThread.detach();
}

void TimerPort::waitTillTimeout(Duration duration)
{
    std::this_thread::sleep_for(duration);
    if (!running)
        return;
    handler->handleTimeout();
}

}
