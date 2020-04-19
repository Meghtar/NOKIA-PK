#include "DbPort.hpp"

namespace ue
{

DbPort::DbPort(int number) : _number(number)
{
    this->_dbPath = std::to_string(number) + "-ue.db";
    _db = std::make_unique<dbStorage>(create_storage(this->_dbPath));
    _db->sync_schema(true);
}

int DbPort::saveMessageToDb(const message& msg)
{
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

};
