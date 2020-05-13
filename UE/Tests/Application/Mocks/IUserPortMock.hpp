#pragma once

#include <gmock/gmock.h>
#include "Ports/IUserPort.hpp"

namespace ue
{

class IUserEventsHandlerMock : public IUserEventsHandler
{
public:
    IUserEventsHandlerMock();
    ~IUserEventsHandlerMock() override;
    MOCK_METHOD(void, handleSendSms, (common::PhoneNumber, std::string), (final));
    MOCK_METHOD(std::vector<message>, retrieveMessages, (), (final));
    MOCK_METHOD(void, setMessageAsRead, (int), (final));
    MOCK_METHOD(void, deleteAllMessages, (), (final));
    MOCK_METHOD(void, callResponse, (common::PhoneNumber, Call), (final));
    MOCK_METHOD(void, handleSendCallRequest, (common::PhoneNumber), (final));

};

class IUserPortMock : public IUserPort
{
public:
    IUserPortMock();
    ~IUserPortMock() override;

    MOCK_METHOD(void, showNotConnected, (), (final));
    MOCK_METHOD(void, showConnecting, (), (final));
    MOCK_METHOD(void, showConnected, (), (final));
    MOCK_METHOD(void, showNewSms, (), (final));
    MOCK_METHOD(void, showNoNewSms, (), (final));
    MOCK_METHOD(void, showCallView, (), (final));
    MOCK_METHOD(void, showIncomingCallRequest, (common::PhoneNumber), (final));
    MOCK_METHOD(void, showDefaultView, (), (final));
};

}
