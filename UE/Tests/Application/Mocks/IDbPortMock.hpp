#pragma once

#include <gmock/gmock.h>
#include "Ports/IDbPort.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class IDbPortMock : public IDbPort
{
public:
    IDbPortMock();
    ~IDbPortMock() override;

    MOCK_METHOD(int, saveMessageToDb, (const common::PhoneNumber phoneNumber, std::string text, bool isSender), (final));
    MOCK_METHOD(std::vector<message>, getAllMessages, (), (final));
    MOCK_METHOD(message, getMessageById, (int), (final));
    MOCK_METHOD(void, removeMessageById, (int), (final));
    MOCK_METHOD(void, removeAllMessages, (), (final));
};

}
