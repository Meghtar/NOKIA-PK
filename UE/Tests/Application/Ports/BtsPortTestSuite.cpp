#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/BtsPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IBtsPortMock.hpp"
#include "Messages/PhoneNumber.hpp"
#include "Mocks/ITransportMock.hpp"
#include "Messages/OutgoingMessage.hpp"
#include "Messages/IncomingMessage.hpp"

namespace ue
{
using namespace ::testing;

class BtsPortTestSuite : public Test
{
protected:
    const common::PhoneNumber senderNumber {111};
    const common::PhoneNumber receiverNumber {110};
    const common::PhoneNumber PHONE_NUMBER{112};
    const common::BtsId BTS_ID{13121981ll};
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IBtsEventsHandlerMock> handlerMock;
    StrictMock<common::ITransportMock> transportMock;
    common::ITransport::MessageCallback messageCallback;
    common::ITransport::DisconnectedCallback disconnectedCallback;

    BtsPort objectUnderTest{loggerMock, transportMock, PHONE_NUMBER};

    BtsPortTestSuite()
    {
        EXPECT_CALL(transportMock, registerMessageCallback(_))
                .WillOnce(SaveArg<0>(&messageCallback));
        EXPECT_CALL(transportMock, registerDisconnectedCallback(_))
                .WillOnce(SaveArg<0>(&disconnectedCallback));
        objectUnderTest.start(handlerMock);
    }
    ~BtsPortTestSuite()
    {

        EXPECT_CALL(transportMock, registerMessageCallback(IsNull()));
        EXPECT_CALL(transportMock, registerDisconnectedCallback(IsNull()));
        objectUnderTest.stop();
    }
};

TEST_F(BtsPortTestSuite, shallRegisterHandlersBetweenStartStop)
{
}

TEST_F(BtsPortTestSuite, shallHandleCallRequest)
{
    EXPECT_CALL(handlerMock, handleCallRequest(receiverNumber));
    common::OutgoingMessage msg{common::MessageId::CallRequest,
                                    receiverNumber,
                                    PHONE_NUMBER};
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallAcceptCall)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));

    objectUnderTest.callResponse(receiverNumber, Call::accepted);

    common::IncomingMessage readmsg(msg);
    EXPECT_EQ(common::MessageId::CallAccepted, readmsg.readMessageId());
    EXPECT_EQ(PHONE_NUMBER, readmsg.readPhoneNumber());
    EXPECT_EQ(receiverNumber, readmsg.readPhoneNumber());
    ASSERT_NO_THROW(readmsg.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallRejectCall)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));

    objectUnderTest.callResponse(receiverNumber, Call::rejected);

    common::IncomingMessage readmsg(msg);
    EXPECT_EQ(common::MessageId::CallDropped, readmsg.readMessageId());
    EXPECT_EQ(PHONE_NUMBER, readmsg.readPhoneNumber());
    EXPECT_EQ(receiverNumber, readmsg.readPhoneNumber());
    ASSERT_NO_THROW(readmsg.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallRejectCallResponse)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));

    objectUnderTest.callResponse(receiverNumber, Call::rejected);

    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallDropped, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(receiverNumber, reader.readPhoneNumber()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallSendSmsToBts)
{
    auto testMessage = "example";
    common::BinaryMessage msg;

    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));

    objectUnderTest.sendSms(receiverNumber, testMessage);

    common::IncomingMessage sms(msg);
    EXPECT_EQ(common::MessageId::Sms, sms.readMessageId());
    EXPECT_EQ(PHONE_NUMBER, sms.readPhoneNumber());
    EXPECT_EQ(receiverNumber, sms.readPhoneNumber());
    EXPECT_EQ(testMessage, sms.readRemainingText());
    ASSERT_NO_THROW(sms.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallHandleReceivedSms)
{
    EXPECT_CALL(handlerMock, handleReceiveSms);
    common::OutgoingMessage msg{common::MessageId::Sms, common::PhoneNumber{}, PHONE_NUMBER};
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleDisconnected)
{
    EXPECT_CALL(handlerMock, handleDisconnected());
    disconnectedCallback();
}


TEST_F(BtsPortTestSuite, shallIgnoreWrongMessage)
{
    common::OutgoingMessage wrongMsg{};
    wrongMsg.writeBtsId(BTS_ID);
    messageCallback(wrongMsg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleSib)
{
    EXPECT_CALL(handlerMock, handleSib(BTS_ID));
    common::OutgoingMessage msg{common::MessageId::Sib,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeBtsId(BTS_ID);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleAttachAccept)
{
    EXPECT_CALL(handlerMock, handleAttachAccept());
    common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(true);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallHandleAttachReject)
{
    EXPECT_CALL(handlerMock, handleAttachReject());
    common::OutgoingMessage msg{common::MessageId::AttachResponse,
                                common::PhoneNumber{},
                                PHONE_NUMBER};
    msg.writeNumber(false);
    messageCallback(msg.getMessage());
}

TEST_F(BtsPortTestSuite, shallSendAttachRequest)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));
    objectUnderTest.sendAttachRequest(BTS_ID);
    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::AttachRequest, reader.readMessageId()) );
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(common::PhoneNumber{}, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(BTS_ID, reader.readBtsId()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallSendCallTalk)
{
    common::BinaryMessage msg;
    EXPECT_CALL(transportMock, sendMessage(_)).WillOnce(SaveArg<0>(&msg));

    auto testMessage = "example";
    objectUnderTest.sendCallTalk(testMessage,receiverNumber);

    common::IncomingMessage reader(msg);
    ASSERT_NO_THROW(EXPECT_EQ(common::MessageId::CallTalk, reader.readMessageId()));
    ASSERT_NO_THROW(EXPECT_EQ(PHONE_NUMBER, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(receiverNumber, reader.readPhoneNumber()));
    ASSERT_NO_THROW(EXPECT_EQ(testMessage, reader.readRemainingText()));
    ASSERT_NO_THROW(reader.checkEndOfMessage());
}

TEST_F(BtsPortTestSuite, shallReceiveCallTalk)
{
    auto testMessage = "example";
    EXPECT_CALL(handlerMock, handleReceiveCallTalk(receiverNumber, testMessage));
    common::OutgoingMessage msg{common::MessageId::CallTalk,
                                receiverNumber,
                                PHONE_NUMBER};
    msg.writeText(testMessage);
    messageCallback(msg.getMessage());
}

}
