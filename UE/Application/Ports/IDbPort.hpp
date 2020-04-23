#pragma once
#include <string>
#include <vector>
#include "Messages/PhoneNumber.hpp"

namespace ue
{

enum isSender {
    incoming,
    outgoing
};

struct message
{
    int messageId;
    int senderNumber;
    int receiverNumber;
    std::string text;
    bool read;

    bool operator==(const message& m) const {
        if(this->senderNumber == m.senderNumber &&
           this->text == m.text)
                return true;
        return false;
    }
};

class IDbPort
{
public:
    virtual ~IDbPort() = default;
    virtual int saveMessageToDb(const common::PhoneNumber, std::string, isSender) = 0;
    virtual std::vector<message> getAllMessages() = 0;
    virtual message getMessageById(int message_id) = 0;
    virtual void removeMessageById(int message_id) = 0;
    virtual void removeAllMessages() = 0;
    virtual void markInDbAsReaded(int)=0;
};

}
