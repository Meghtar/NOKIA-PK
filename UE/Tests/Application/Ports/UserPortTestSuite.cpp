#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/UserPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IUserPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/IUeGuiMock.hpp"

namespace ue
{
using namespace ::testing;

class UserPortTestSuite : public Test
{
protected:
    const common::PhoneNumber PHONE_NUMBER{112};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IUserEventsHandlerMock> handlerMock;
    StrictMock<IUeGuiMock> guiMock;
    StrictMock<IListViewModeMock> listViewModeMock;
    StrictMock<ISmsComposeModeMock> smsComposeModeMock;
    StrictMock<ICallModeMock> callViewModeMock;

    IUeGui::Callback acceptCallback;
    IUeGui::Callback rejectCallback;

    UserPort objectUnderTest{loggerMock, guiMock, PHONE_NUMBER};

    UserPortTestSuite()
    {
        EXPECT_CALL(guiMock, setTitle(HasSubstr(to_string(PHONE_NUMBER))));
        EXPECT_CALL(guiMock, setAcceptCallback(_)).WillOnce(SaveArg<0>(&acceptCallback));
        EXPECT_CALL(guiMock, setRejectCallback(_)).WillOnce(SaveArg<0>(&rejectCallback));
        objectUnderTest.start(handlerMock);
    }
    ~UserPortTestSuite()
    {
        EXPECT_CALL(guiMock, setAcceptCallback(IsNull()));
        EXPECT_CALL(guiMock, setRejectCallback(IsNull()));
        objectUnderTest.stop();
    }
};

TEST_F(UserPortTestSuite, shallStartStop)
{
}

TEST_F(UserPortTestSuite, shallShowNotConnected)
{
    EXPECT_CALL(guiMock, showNotConnected());
    objectUnderTest.showNotConnected();
}

TEST_F(UserPortTestSuite, shallShowConnecting)
{
    EXPECT_CALL(guiMock, showConnecting());
    objectUnderTest.showConnecting();
}

TEST_F(UserPortTestSuite, shallShowMenuOnConnected)
{
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));
    objectUnderTest.showConnected();
}

TEST_F(UserPortTestSuite, shallShowComposeSms)
{
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));
    EXPECT_CALL(listViewModeMock, getCurrentItemIndex()).WillOnce(Return(std::pair<bool, unsigned>(true,0)));

    EXPECT_CALL(guiMock, setSmsComposeMode()).WillOnce(ReturnRef(smsComposeModeMock));
    EXPECT_CALL(smsComposeModeMock, clearSmsText());

    objectUnderTest.showMenu();
    acceptCallback();
}

TEST_F(UserPortTestSuite, shallShowMenuAfterRejectFromComposeSms)
{
    EXPECT_CALL(guiMock, setSmsComposeMode()).WillOnce(ReturnRef(smsComposeModeMock));
    EXPECT_CALL(smsComposeModeMock, clearSmsText());

    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(listViewModeMock, addSelectionListItem(_, _)).Times(AtLeast(1));

    objectUnderTest.showComposeSms();
    rejectCallback();
}

TEST_F(UserPortTestSuite, shallShowEmptySmsList)
{
    EXPECT_CALL(guiMock, setListViewMode()).WillOnce(ReturnRef(listViewModeMock));
    EXPECT_CALL(listViewModeMock, clearSelectionList());
    EXPECT_CALL(handlerMock, retrieveMessages());
    EXPECT_CALL(guiMock, showNoNewSms());

    // No messages are retrieved so addSelectionListItem will not be called

    objectUnderTest.showSmsList();
}

TEST_F(UserPortTestSuite, shallShowSentNewCallTalk)
{
    std::string testMessage = "testMessage";
    std::string incommingMessage = "You: " + testMessage;

    EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callViewModeMock));
    EXPECT_CALL(callViewModeMock, getOutgoingText()).WillOnce(Return(testMessage));
    EXPECT_CALL(callViewModeMock, clearOutgoingText());
    EXPECT_CALL(callViewModeMock, appendIncomingText(incommingMessage));
    EXPECT_CALL(handlerMock, handleCallTalk(testMessage));

    objectUnderTest.sendCallTalk();
}

TEST_F(UserPortTestSuite, shallShowReceivedNewCallTalk)
{
    auto testMessage = "testMessage";

    EXPECT_CALL(guiMock, setCallMode()).WillOnce(ReturnRef(callViewModeMock));
    EXPECT_CALL(callViewModeMock, appendIncomingText(std::to_string(PHONE_NUMBER.value) + ": " + testMessage));

    objectUnderTest.showNewCallTalk(PHONE_NUMBER, testMessage);
}
}
