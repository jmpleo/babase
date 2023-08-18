#pragma once


#include "logger.h"
#include "config.h"
#include <map>
#include <nlohmann/detail/iterators/iteration_proxy.hpp>
#include <nlohmann/detail/meta/type_traits.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>

namespace babase {

class ConnConfManager
{
    using mapped_param = std::map <std::string, std::string>;

public:
    ConnConfManager(const ConnConfManager&) = delete;
    ConnConfManager& operator=(const ConnConfManager&) = delete;

    ConnConfManager(std::string defaultConfigPath);
    ~ConnConfManager();

    void        setConnectionOptions(std::string connName, std::string paramsLine);
    void        setDevice           (std::string connName, std::string id);
    void        removeConnection    (std::string connName);
    std::string getConnectionOptions(std::string connName);
    std::string getDevice           (std::string connName);
    std::string rollupOptions       (mapped_param);
    bool        connectionExists    (std::string connName)   { return !getConnectionOptions(connName).empty(); }
    void        setConfigPath       (std::string configPath) { configPath_ = configPath; }

    mapped_param getSplitedConnectionOptions(std::string connName) { return splitOptions(getConnectionOptions(connName)); }
    mapped_param splitOptions               (std::string optionsLine);

    std::vector<std::string> getConnectionsList();

private:
    void updateState();
    void updateConfig();

private:
    std::string configPath_;
    nlohmann::json jconfigState_;
};

inline std::string ConnConfManager::rollupOptions(std::map<std::string, std::string> splitedOpt)
{
    std::string paramLine;
    for (auto &opt : splitedOpt) { paramLine += (" " + opt.first + "=" + opt.second); }
    return paramLine;
}

}
