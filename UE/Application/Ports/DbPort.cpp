#include "DbPort.hpp"

namespace ue
{

DbPort::DbPort(int number) : _number(number)
{
    this->_dbPath = std::to_string(number) + "-ue.db";
    _db = std::make_unique<dbStorage>(create_storage(this->_dbPath));
}

int DbPort::saveMessageToDb(const message& msg)
{
    return _db->insert(msg);
}

};
