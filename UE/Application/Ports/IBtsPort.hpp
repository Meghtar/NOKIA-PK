#pragma once

#include "Messages/BtsId.hpp"
#include "Messages/PhoneNumber.hpp"
#include "IUserPort.hpp"

namespace ue
{

class IBtsEventsHandler
{
public:
    virtual ~IBtsEventsHandler() = default;

    virtual void handleDisconnected() = 0;

    virtual void handleSib(common::BtsId) = 0;
    virtual void handleAttachAccept() = 0;
    virtual void handleAttachReject() = 0;
    virtual void handleReceiveSms(common::PhoneNumber, std::string) = 0;
    virtual void handleCallRequest(common::PhoneNumber) = 0;

    virtual void handleReceiveAcceptedCall(common::PhoneNumber) = 0;
    virtual void handleReceiveDroppedCall(common::PhoneNumber) = 0;

    virtual void handleReceiveCallTalk(common::PhoneNumber, std::string) = 0;

};
class IBtsPort
{
public:
    virtual ~IBtsPort() = default;

    virtual void sendAttachRequest(common::BtsId) = 0;
    virtual void sendSms(common::PhoneNumber, std::string) = 0;
    virtual void callResponse(common::PhoneNumber, Call) = 0;
    virtual void sendCallRequest(common::PhoneNumber) = 0;
    virtual void sendCallDrop(common::PhoneNumber) = 0;
    virtual void sendCallTalk(std::string, common::PhoneNumber) = 0;
};
}
