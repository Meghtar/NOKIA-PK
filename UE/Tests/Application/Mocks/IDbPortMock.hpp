#pragma once

#include <gmock/gmock.h>
#include "Ports/IDbPort.hpp"

namespace ue
{

class IDbPortMock : public IDbPort
{
public:
    IDbPortMock();
    ~IDbPortMock() override;

    MOCK_METHOD(int, saveMessageToDb, (const message&), (final));
    MOCK_METHOD(std::vector<message>, getAllMessages, (), (final));
    MOCK_METHOD(message, getMessageById, (int), (final));
    MOCK_METHOD(void, removeMessageById, (int), (final));
    MOCK_METHOD(void, removeAllMessages, (), (final));
};

}
