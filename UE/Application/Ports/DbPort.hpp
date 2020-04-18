#pragma once
#include "IDbPort.hpp"
//#include "sqlite_orm/sqlite_orm.h"
#include "../sqlite_orm/sqlite_orm.h"

namespace ue
{
class DbPort : public IDbPort
{
public:
    DbPort(int number);
    int saveMessageToDb(const message& msg);
private:
    std::string _dbPath;
    const int _number;
};
};
