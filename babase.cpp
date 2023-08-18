#include "babase.h"
#include "connconfmanager.h"

using namespace babase;

ConnConfManager BABase::config(_DEFAULT_DB_CONFIG_PATH);

BABase::BABase(std::string name)
    : connName_(name)
    , conn_(nullptr)
{
}

BABase::BABase(BABase&& other)
    : connName_(std::move(other.connName_))
    , conn_(std::move(other.conn_))
{

}

BABase::~BABase()
{

}

bool BABase::tryConnect(std::string otherConnName)
{
    connName_ = otherConnName.empty() ? connName_ : otherConnName;

    try {
        conn_ = std::make_unique<pqxx::connection>( config.getConnectionOptions(connName_));

        //if (checkScheme() || setScheme()) {
        if (setScheme()) {
            Logger::cout() << "Успешное подключение к " + connName_ << std::endl;
            return true;
        }
        else {
            return false;
        }
    }
    catch (const pqxx::broken_connection &e) {
        Logger::cout() << "Не удалось подключиться к " + connName_ << " Подробнее: " << e.what() << std::endl;
        return false;
    }
    catch (...) {
        return false;
    }

}


