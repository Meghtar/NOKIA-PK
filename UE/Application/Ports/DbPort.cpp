#include "DbPort.hpp"

namespace ue
{

DbPort::DbPort(int number) : _number(number)
{
    this->_dbPath = std::to_string(number) + "-ue.db";
    _db = std::make_unique<dbStorage>(create_storage(this->_dbPath));
    _db->sync_schema(true);
}

int DbPort::saveMessageToDb(const common::PhoneNumber phoneNumber, std::string text, isSender val)
{
    bool isRead = true;
    int receiverNumber, senderNumber;
    if(val == isSender::outgoing)
    {
        senderNumber = _number;
        receiverNumber = phoneNumber.value;
    } else {
        receiverNumber = _number;
        senderNumber = phoneNumber.value;
        isRead = false;
    }
    message msg {/*Autoincrement */ -1, senderNumber, receiverNumber, text, isRead};
    return _db->insert(msg);
}

std::vector<message> DbPort::getAllMessages()
{
    return _db->get_all<message>();
}

message DbPort::getMessageById(int message_id)
{
    return _db->get<message>(message_id);
}

void DbPort::removeMessageById(int message_id)
{
    _db->remove<message>(message_id);
}

void DbPort::removeAllMessages()
{
    _db->remove_all<message>();
}

void DbPort::markInDbAsRead(int messId)
{
    auto sms = getMessageById(messId);
    sms.read = true;
    _db->update(sms);
}

}
