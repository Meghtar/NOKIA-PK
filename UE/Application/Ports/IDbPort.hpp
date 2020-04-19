#pragma once
#include <string>
#include <vector>

namespace ue
{

// TODO: change sender and receiverNumber type into phoneNumber
struct message
{
    int messageId;
    int senderNumber;
    int receiverNumber;
    std::string text;
    bool read;

    bool operator==(const message& m) const {
        if(this->senderNumber == m.senderNumber &&
           this->receiverNumber == m.receiverNumber &&
           this->text == m.text &&
           this->read == m.read)
            return true;
        return false;
    }
};

class IDbPort
{
public:
    virtual ~IDbPort() = default;
    virtual int saveMessageToDb(const message& msg) = 0;
    virtual std::vector<message> getAllMessages() = 0;
    virtual message getMessageById(int message_id) = 0;
    virtual void removeMessageById(int message_id) = 0;
    virtual void removeAllMessages() = 0;
};

}
