#pragma once
#include "IDbPort.hpp"
#include "sqlite_orm.h"

namespace ue
{

inline auto create_storage(const std::string& path)
{
    using namespace sqlite_orm;
    return make_storage(path,
                                make_table("messages",
                                           make_column("message_id", &message::messageId, autoincrement(), primary_key()),
                                           make_column("sender_number", &message::senderNumber),
                                           make_column("receiver_number", &message::receiverNumber),
                                           make_column("text", &message::text),
                                           make_column("read", &message::read)));
}

using dbStorage = decltype(create_storage(""));

class DbPort : public IDbPort
{
public:
    DbPort(int number);
    int saveMessageToDb(const common::PhoneNumber, std::string, bool) override;
    std::vector<message> getAllMessages() override;
    message getMessageById(int message_id) override;
    void removeMessageById(int message_id) override;
    void removeAllMessages() override;
private:
    std::string _dbPath;
    const int _number;
    std::unique_ptr<dbStorage> _db;
};
};
