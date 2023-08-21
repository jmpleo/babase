#pragma once

#include "connconfmanager.h"
#include <memory>
#include <pqxx/connection.hxx>
#include <string>

namespace babase {

class BABase
{
public:
    BABase() = delete;
    BABase(BABase const&) = delete;

    BABase(std::string);
    BABase(BABase&&);

    virtual ~BABase();

public:
    //virtual bool checkScheme() = 0;
    virtual bool setScheme  () = 0;
    bool         isConnected()       { return setScheme(); }
    std::string  connName   () const { return connName_; }
    bool         tryConnect (std::string connName = "");

    static ConnConfManager config;

protected:
    std::string connName_;
    std::unique_ptr <pqxx::connection> conn_;
};

}
