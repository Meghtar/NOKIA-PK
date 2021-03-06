#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Application.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Mocks/ITimerPortMock.hpp"
#include "Mocks/IDbPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include <memory>

namespace ue
{
using namespace ::testing;

class ApplicationTestSuite : public Test
{
protected:
    const common::PhoneNumber receiverNumber {111};
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{203};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsPortMock> btsPortMock;
    StrictMock<IUserPortMock> userPortMock;
    StrictMock<ITimerPortMock> timerPortMock;
    StrictMock<IDbPortMock> dbPortMock;

    Expectation notConnectedExpectation = EXPECT_CALL(userPortMock, showNotConnected());

    Application objectUnderTest{PHONE_NUMBER,
                                loggerMock,
                                btsPortMock,
                                userPortMock,
                                timerPortMock,
                                dbPortMock};
};

struct ApplicationNotConnectedTestSuite : ApplicationTestSuite
{};

TEST_F(ApplicationNotConnectedTestSuite, shallSetNotConnectedStateAtStartup)
{

}

struct ApplicationConnectingTestSuite : ApplicationNotConnectedTestSuite
{
    ApplicationConnectingTestSuite()
    {
        using namespace std::chrono_literals;
        EXPECT_CALL(btsPortMock, sendAttachRequest(BTS_ID));
        EXPECT_CALL(timerPortMock, startTimer(500ms));
        EXPECT_CALL(userPortMock, showConnecting());
        objectUnderTest.handleSib(BTS_ID);
    }
};


TEST_F(ApplicationConnectingTestSuite, shallSendAttachRequestUponReceivingSIB)
{
    // implemented in Constructor of TestSuite
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectingOnDisconnect)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleDisconnected();
}


TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnAttachReject)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    EXPECT_CALL(timerPortMock, stopTimer());
    objectUnderTest.handleAttachReject();
}

TEST_F(ApplicationConnectingTestSuite, shallShowNotConnectedOnTimeout)
{
    EXPECT_CALL(userPortMock, showNotConnected());
    objectUnderTest.handleTimeout();
}

struct ApplicationConnectedTestSuite : ApplicationConnectingTestSuite
{
    ApplicationConnectedTestSuite()
    {
        EXPECT_CALL(userPortMock, showConnected());
        EXPECT_CALL(timerPortMock, stopTimer());
        objectUnderTest.handleAttachAccept();
    }
};

TEST_F(ApplicationConnectedTestSuite, shallHandleSendSms)
{
    auto msg = "example";
    EXPECT_CALL(btsPortMock, sendSms(receiverNumber, msg));
    EXPECT_CALL(dbPortMock, saveMessageToDb(receiverNumber, msg, outgoing));
    objectUnderTest.handleSendSms(receiverNumber, msg);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleReceiveSms)
{
    auto message = "hello world";;
    EXPECT_CALL(dbPortMock, saveMessageToDb(receiverNumber, message, incoming));
    EXPECT_CALL(userPortMock, showNewSms());

     objectUnderTest.handleReceiveSms(receiverNumber, message);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallRequest)
{
    using namespace std::chrono_literals;
    EXPECT_CALL(userPortMock, showIncomingCallRequest(PHONE_NUMBER));
    EXPECT_CALL(timerPortMock, startTimer(40000ms));
    objectUnderTest.handleCallRequest(PHONE_NUMBER);
}

TEST_F(ApplicationConnectedTestSuite, shallHandleCallReject)
{
    EXPECT_CALL(btsPortMock, callResponse(PHONE_NUMBER, Call::rejected));
    EXPECT_CALL(timerPortMock, stopTimer());
    EXPECT_CALL(userPortMock, showDefaultView());
    objectUnderTest.callResponse(PHONE_NUMBER, Call::rejected);
}

TEST_F(ApplicationConnectedTestSuite, shallShowConnectedOnAttachAccept)
{
    // implemented in constructor of test-suite
}

}
