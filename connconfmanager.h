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

/*
 * \brief Класс взаимодействия с файлом конфигурации.
 *
 * Реализует чтение, запись и изменение опций файла конфигурации соединений.
 * Инкапсулирует состояние конфигурационного файла используя nlohmann::json.
 */
class ConnConfManager
{
    using str = std::string;
    using mapped_param = std::map <str, str>;

public:
    ConnConfManager(const ConnConfManager&) = delete;
    ConnConfManager& operator=(const ConnConfManager&) = delete;

    ConnConfManager(str defaultConfigPath);
    ~ConnConfManager();

    bool connectionExists(str connName) { return not getConnectionOptions(connName).empty(); }

    void setConnectionOptions(str connName, str paramsLine);
    void setDevice           (str connName, str id);
    void removeConnection    (str connName);
    str  getConnectionOptions(str connName);
    str  getDevice           (str connName);
    void setConfigPath       (str configPath)
        { configPath_ = configPath; updateState(); }

    mapped_param splitOptions               (str optionsLine);
    mapped_param getSplitedConnectionOptions(str connName)
        { return splitOptions(getConnectionOptions(connName)); }

    std::vector<str> getConnectionsList();

    inline str rollupOptions(mapped_param);

private:
    void updateState();
    void updateConfig();

private:
    str configPath_;
    nlohmann::json jconfigState_;
};

inline std::string ConnConfManager::rollupOptions(mapped_param splitedOpt)
{
    str paramLine;
    for (auto &opt : splitedOpt) { paramLine += (" " + opt.first + "=" + opt.second); }
    return paramLine;
}

}
