#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/TimerPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{
using namespace ::testing;

class TimerPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<ITimerEventsHandlerMock> handlerMock;


    TimerPort objectUnderTest{loggerMock};

    TimerPortTestSuite()
    {
        objectUnderTest.start(handlerMock);
    }
    ~TimerPortTestSuite()
    {
        objectUnderTest.stop();
    }
};

TEST_F(TimerPortTestSuite, shallStart)
{
}

TEST_F(TimerPortTestSuite, shallTiemout)
{
    EXPECT_CALL(handlerMock, handleTimeout());

    auto tmp = std::chrono::duration<int>(1);
    objectUnderTest.startTimer(tmp);
    std::this_thread::sleep_for(2 * tmp);
}

TEST_F(TimerPortTestSuite, shallNotTiemoutAfterStopingTimer)
{
    EXPECT_CALL(handlerMock, handleTimeout()).Times(0);

    auto tmp = std::chrono::duration<int>(1);
    objectUnderTest.startTimer(tmp);
    objectUnderTest.stopTimer();
    std::this_thread::sleep_for(2 * tmp);
}

}
