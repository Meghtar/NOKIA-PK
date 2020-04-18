#pragma once
#include <string>

namespace ue
{

struct message
{
    int messageId;
    int senderNumber;
    int receiverNumber;
    std::string text;
    bool read;
};

class IDbPort
{
public:
    virtual int saveMessageToDb(const message& msg) = 0;
};

}
