#pragma once

#include "IBtsPort.hpp"
#include "Logger/PrefixedLogger.hpp"
#include "ITransport.hpp"
#include "Messages/PhoneNumber.hpp"

namespace ue
{

class BtsPort : public IBtsPort
{
public:
    BtsPort(common::ILogger& logger, common::ITransport& transport, common::PhoneNumber phoneNumber);
    void start(IBtsEventsHandler& handler);
    void stop();

    void sendAttachRequest(common::BtsId) override;
    void sendSms(common::PhoneNumber, std::string) override;
    void callResponse(common::PhoneNumber, Call) override;
    void sendCallRequest(common::PhoneNumber) override;
    void sendCallDrop(common::PhoneNumber) override;
    void sendCallTalk(std::string, common::PhoneNumber) override;
private:
    void handleMessage(BinaryMessage msg);
    void handleDisconnected();

    common::PrefixedLogger logger;
    common::ITransport& transport;
    common::PhoneNumber phoneNumber;

    IBtsEventsHandler* handler = nullptr;
};

}
