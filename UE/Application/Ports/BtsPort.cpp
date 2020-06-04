#include "BtsPort.hpp"
#include "Messages/IncomingMessage.hpp"
#include "Messages/OutgoingMessage.hpp"

namespace ue
{

BtsPort::BtsPort(common::ILogger &logger, common::ITransport &transport, common::PhoneNumber phoneNumber)
    : logger(logger, "[BTS-PORT]"),
      transport(transport),
      phoneNumber(phoneNumber)
{}

void BtsPort::start(IBtsEventsHandler &handler)
{
    transport.registerMessageCallback([this](BinaryMessage msg) {handleMessage(msg);});
    transport.registerDisconnectedCallback([this] {handleDisconnected();});
    this->handler = &handler;
}

void BtsPort::stop()
{
    transport.registerMessageCallback(nullptr);
    transport.registerDisconnectedCallback(nullptr);
    handler = nullptr;
}

void BtsPort::handleMessage(BinaryMessage msg)
{
    try
    {
        common::IncomingMessage reader{msg};
        auto msgId = reader.readMessageId();
        auto from = reader.readPhoneNumber();
        auto to = reader.readPhoneNumber();

        switch (msgId)
        {
        case common::MessageId::Sib:
        {
            auto btsId = reader.readBtsId();
            handler->handleSib(btsId);
            break;
        }
        case common::MessageId::AttachResponse:
        {
            bool accept = reader.readNumber<std::uint8_t>() != 0u;
            if (accept)
                handler->handleAttachAccept();
            else
                handler->handleAttachReject();
            break;
        }
        case common::MessageId::Sms:
        {
            std::string message = reader.readRemainingText();
            logger.logDebug("receivedSms = ", from);
            logger.logDebug("message = ", message);
            handler->handleReceiveSms(from, message);
            break;
        }
        case common::MessageId::CallRequest:
        {
            logger.logDebug("CallRequest from number= ", from);
            handler->handleCallRequest(from);
            break;
        }
        case common::MessageId::CallAccepted:
        {
            logger.logDebug("Accepted call from: ", from);
            handler->handleReceiveAcceptedCall(from);
            break;
        }
        case common::MessageId::CallDropped:
        {
            logger.logDebug("Dropped call from: ", from);
            handler->handleReceiveDroppedCall(from);
            break;
        }
        case common::MessageId::CallTalk:
        {
            logger.logDebug("Received call talk from: ", from);
            auto text = reader.readRemainingText();
            handler->handleReceiveCallTalk(from, text);
            break;
        }

        default:
            logger.logError("unknow message: ", msgId, ", from: ", from);

        }
    }
    catch (std::exception const& ex)
    {
        logger.logError("handleMessage error: ", ex.what());
    }
}

void BtsPort::handleDisconnected()
{
    logger.logError("disconnected");
    handler->handleDisconnected();
}

void BtsPort::sendAttachRequest(common::BtsId btsId)
{
    logger.logDebug("sendAttachRequest: ", btsId);
    common::OutgoingMessage msg{common::MessageId::AttachRequest,
                                phoneNumber,
                                common::PhoneNumber{}};
    msg.writeBtsId(btsId);
    transport.sendMessage(msg.getMessage());

}

void BtsPort::sendSms(common::PhoneNumber rNumber, std::string msg)
{
    logger.logDebug("sendingSmsTo= ", rNumber);
    common::OutgoingMessage message { common::MessageId::Sms, phoneNumber, rNumber };
    message.writeText(msg);
    transport.sendMessage(message.getMessage());
}

void BtsPort::callResponse(common::PhoneNumber number, Call acc)
{
    logger.logDebug("CallResponse from ", number);
    common::MessageId callM;
    if (acc == Call::accepted)
    {
        callM = common::MessageId::CallAccepted;
    }
    else
    {
        callM = common::MessageId::CallDropped;
    }
    common::OutgoingMessage m {
        callM, phoneNumber, number
    };
    transport.sendMessage(m.getMessage());
}

void BtsPort::sendCallRequest(common::PhoneNumber rNumber)
{
    logger.logDebug("callRequest", rNumber);
    common::OutgoingMessage message{common::MessageId::CallRequest, phoneNumber, rNumber};
    transport.sendMessage(message.getMessage());
}

void BtsPort::sendCallTalk(std::string text, common::PhoneNumber number)
{
    logger.logDebug("Going to send ",text," to ", number);
    common::OutgoingMessage message{common::MessageId::CallTalk, phoneNumber, number};
    message.writeText(text);
    transport.sendMessage(message.getMessage());
}

void BtsPort::sendCallDrop(common::PhoneNumber rNumber)
{
    logger.logDebug("sendCallDrop: ", rNumber);
    common::OutgoingMessage message{common::MessageId::CallDropped, phoneNumber, rNumber};
    transport.sendMessage(message.getMessage());
}

}
