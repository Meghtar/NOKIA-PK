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

    DbPortTestSuite()
    {}
    ~DbPortTestSuite()
    {
        objectUnderTest.removeAllMessages();
    }
};

TEST_F(DbPortTestSuite, shallSaveOneMessage)
{
    int messageId = 100;
    int senderNumber = 111;
    int receiverNumber = phoneNumber;
    std::string messageText = "example";
    bool messageRead = false;
    message msg{messageId, senderNumber, receiverNumber, messageText, messageRead};
    objectUnderTest.saveMessageToDb(msg);

    auto allMessages = objectUnderTest.getAllMessages();

    EXPECT_EQ(allMessages.size(), 1);
    ASSERT_TRUE(allMessages[0] == msg);
}
}
