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

    int phoneNumber = 123;
    DbPort objectUnderTest{phoneNumber};

    int messageId = 100;
    int senderNumber = 111;
    int receiverNumber = phoneNumber;
    std::string messageText = "example";
    bool messageRead = false;

    message testMessage{
        messageId,
        senderNumber,
        receiverNumber,
        messageText,
        messageRead
    };

    DbPortTestSuite()
    {}
    ~DbPortTestSuite()
    {
        objectUnderTest.removeAllMessages();
    }
};

TEST_F(DbPortTestSuite, shallSaveOneMessage)
{
    objectUnderTest.saveMessageToDb(testMessage);

    auto allMessages = objectUnderTest.getAllMessages();

    EXPECT_EQ(allMessages.size(), 1);
    ASSERT_TRUE(allMessages[0] == testMessage);
}

TEST_F(DbPortTestSuite, shallSaveMultipleMessages)
{
    int amountOfMessages = 10;
    for (int i = 0; i < amountOfMessages; ++i)
        objectUnderTest.saveMessageToDb(testMessage);

    auto allMessages = objectUnderTest.getAllMessages();

    EXPECT_EQ(allMessages.size(), amountOfMessages);
}

TEST_F(DbPortTestSuite, shallDeleteSingleMessage)
{
    int id = objectUnderTest.saveMessageToDb(testMessage);

    objectUnderTest.removeMessageById(id);

    auto allMessages = objectUnderTest.getAllMessages();

    EXPECT_EQ(allMessages.size(), 0);
}

TEST_F(DbPortTestSuite, shallDeleteAllMessages)
{
    int amountOfMessages = 10;
    for (int i = 0; i < amountOfMessages; ++i)
        objectUnderTest.saveMessageToDb(testMessage);

    objectUnderTest.removeAllMessages();

    auto allMessages = objectUnderTest.getAllMessages();

    EXPECT_EQ(allMessages.size(), 0);
}
}
