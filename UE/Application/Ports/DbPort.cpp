#include "DbPort.hpp"

namespace ue
{

DbPort::DbPort(int number) : _number(number)
{
    this->_dbPath = std::to_string(number) + "-ue.db";

}
int DbPort::saveMessageToDb(const message &msg)
{

}

// TODO: add example action using sqlite_orm
};
