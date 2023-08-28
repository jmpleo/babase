#include "babase.h"
#include "connconfmanager.h"

using namespace babase;

ConnConfManager BABase::config(_DEFAULT_DB_CONFIG_PATH);



/*
 * \brief Попытка соединения с базой.
 *
 * Настройки соединения по имени соединения передаются от config - менеджера
 * конфигурационного файла.
 *
 * \param otherConnName - название соединения, с которым будет производиться попытка подключения.
 * По умолчанию с соединением переданным при конструировании BABase.
 * \return false - при неудачном соединении, true - соединение можно
 * использовать
 */
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



