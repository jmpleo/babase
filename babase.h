#pragma once

#include "connconfmanager.h"
#include <memory>
#include <pqxx/connection.hxx>
#include <string>

namespace babase {

/**
 * \brief Абстрактный класс для взаимодействия с бд через pqxx.
 *
 * Класс реализует основной метод tryConnect(), позволяющий соединиться с бд
 * и обновить схему бд на используя виртуальный метод setScheme().
 */
class BABase
{
public:
    BABase(BABase const&) = delete;

    BABase()                 : connName_()                           , conn_(nullptr) {}
    BABase(std::string name) : connName_(name)                       , conn_(nullptr) {}
    BABase(BABase&& other)   : connName_(std::move(other.connName_)) , conn_(std::move(other.conn_)) {}

    virtual ~BABase() {}

public:
    //virtual bool checkScheme() = 0;
    virtual bool setScheme() = 0;
    virtual bool isConnected() { return setScheme(); }

    bool tryConnect(std::string connName = "");
    bool touch() { return isConnected() || tryConnect(); }

    std::string connName() const { return connName_; }

    static ConnConfManager config;

protected:
    std::string connName_;
    std::unique_ptr <pqxx::connection> conn_;
};

}
