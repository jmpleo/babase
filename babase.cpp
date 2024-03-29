#include "babase.h"
#include "connconfmanager.h"
#include "logger.h"
#include <algorithm>
#include <pqxx/connection.hxx>

using namespace babase;

ConnConfManager BABase::config;

/**
 * \brief Попытка соединения с базой.
 *
 * Настройки соединения по имени соединения передаются от статического поля
 * config - менеджера конфигурационного файла. После соединения вызывается
 * виртуальный метод setScheme() который устанавливает весь необходимый
 * функционал в бд.
 *
 * \param otherConnName - Название соединения, с которым будет производиться
 * попытка подключения. По умолчанию с соединением переданным при
 * конструировании BABase.
 *
 * \return Cтатус соединения.
 */
bool BABase::tryConnect(std::string otherConnName)
{
    connName_ = otherConnName.empty() ? connName_ : otherConnName;

    try {
        conn_ = std::make_unique <pqxx::connection> (config.getConnectionOptions(connName_));
        //return setScheme();
        return true;
    }
    catch (const pqxx::broken_connection &e) {
        Logger::cout()
            << "Не удалось подключиться к " + connName_
            << " Подробнее: " << e.what() << std::endl;
    }
    catch (...) {
        //
    }
    return false;
}


