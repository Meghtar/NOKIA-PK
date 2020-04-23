#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Ports/DbPort.hpp"
#include "Mocks/ILoggerMock.hpp"
#include "Mocks/IDbPortMock.hpp"

namespace ue
{
using namespace ::testing;

class DbPortTestSuite : public Test
{
protected:
    NiceMock<common::ILoggerMock> loggerMock;
    StrictMock<IDbPortMock> dbPortMock;

    common::PhoneNumber phoneNumber {123};
    DbPort objectUnderTest{phoneNumber.value};

    int messageId = 100;
    common::PhoneNumber senderNumber {111};
    common::PhoneNumber receiverNumber = phoneNumber;
    std::string messageText = "example";
    bool messageRead = false;

    DbPortTestSuite()
    {}
    ~DbPortTestSuite()
    {
        objectUnderTest.removeAllMessages();
    }
};

TEST_F(DbPortTestSuite, shallEditMessage)
{
    objectUnderTest.saveMessageToDb(phoneNumber, messageText, incoming);
    auto allMessages = objectUnderTest.getAllMessages();
    EXPECT_EQ(allMessages.size(), 1);
    objectUnderTest.markInDbAsReaded(allMessages[0].messageId);
    allMessages = objectUnderTest.getAllMessages();
    ASSERT_TRUE(allMessages[0].read == true);
}

TEST_F(DbPortTestSuite, shallSaveOneMessage)
{
    objectUnderTest.saveMessageToDb(phoneNumber, messageText, outgoing);

    auto allMessages = objectUnderTest.getAllMessages();
    EXPECT_EQ(allMessages.size(), 1);
    message testMessage {1, phoneNumber.value, 0, messageText, false};
    ASSERT_TRUE(allMessages[0] == testMessage);
}

TEST_F(DbPortTestSuite, shallSaveOutgoingMessage)
{
    objectUnderTest.saveMessageToDb(senderNumber, messageText, outgoing);

    auto allMessages = objectUnderTest.getAllMessages();
    EXPECT_EQ(allMessages.size(), 1);
    message testMessage {1, receiverNumber.value, senderNumber.value, messageText, false};
    ASSERT_TRUE(allMessages[0] == testMessage);
}

TEST_F(DbPortTestSuite, shallSaveIncomingMessage)
{
    objectUnderTest.saveMessageToDb(senderNumber, messageText, incoming);

    auto allMessages = objectUnderTest.getAllMessages();
    EXPECT_EQ(allMessages.size(), 1);
    message testMessage {1, senderNumber.value, receiverNumber.value, messageText, false};
    ASSERT_TRUE(allMessages[0] == testMessage);
}

TEST_F(DbPortTestSuite, shallSaveMultipleMessages)
{
    int amountOfMessages = 10;
    for (int i = 0; i < amountOfMessages; ++i)
        objectUnderTest.saveMessageToDb(phoneNumber, messageText, outgoing);

    auto allMessages = objectUnderTest.getAllMessages();

    EXPECT_EQ(allMessages.size(), amountOfMessages);
}

TEST_F(DbPortTestSuite, shallDeleteSingleMessage)
{
    int id = objectUnderTest.saveMessageToDb(phoneNumber, messageText, outgoing);

    objectUnderTest.removeMessageById(id);

    auto allMessages = objectUnderTest.getAllMessages();

    EXPECT_EQ(allMessages.size(), 0);
}

TEST_F(DbPortTestSuite, shallDeleteAllMessages)
{
    int amountOfMessages = 10;
    for (int i = 0; i < amountOfMessages; ++i)
        objectUnderTest.saveMessageToDb(phoneNumber, messageText, outgoing);

    objectUnderTest.removeAllMessages();

    auto allMessages = objectUnderTest.getAllMessages();

    EXPECT_EQ(allMessages.size(), 0);
}
}
