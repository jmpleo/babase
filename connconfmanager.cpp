#include "connconfmanager.h"
#include "logger.h"
#include "config.h"
#include <fstream>
#include <nlohmann/json_fwd.hpp>
#include <sstream>
#include <string>
#include <regex>
#include <vector>

using namespace babase;

ConnConfManager::ConnConfManager(std::string defaultConfigPath)
    : jconfigState_()
    , configPath_(defaultConfigPath)
{
    updateState();
}


/**
 * \brief Обновление состояния объекта.
 *
 * Обновляется при успешном обновлении состояние объекта изменяется в
 * соответствии с файлом конфигурации производится чтение файла.
 */
void ConnConfManager::updateState()
{
    std::ifstream f(configPath_);
    if (f.is_open() == false) {
        Logger::cout() << "Не удалось открыть файл конфигурации: " << configPath_ << std::endl;
        return;
    }

    try {
        f >> jconfigState_;
    }
    catch (nlohmann::json::parse_error &ex) {
        Logger::cout() << "Не удалось распарсить конфигурационный файл: " << configPath_ << " Подробнее: " << ex.byte << std::endl;
    }
    f.close();
}


/**
 * \brief Обновление конфигурационного файла.
 *
 * Производится запись состояния объекта в конфигурационный файл.
 */
void ConnConfManager::updateConfig()
{
    std::ofstream f(configPath_);
    if (f.is_open() == false) {
        Logger::cout() << "Не удалось открыть файл конфигурации: " << configPath_ << std::endl;
        return;
    }

    f << jconfigState_.dump(4);
    f.close();
}


/**
 * \brief Удаление записи о соединении из конфигурационного файла.
 *
 * \param connName Название соединения, запись которого будет удалена.
 */
void ConnConfManager::removeConnection(std::string connName)
{
    try {
        jconfigState_.at("connections").erase(connName);
        updateConfig();
    }
    catch (nlohmann::json::type_error &ex) {
        Logger::cout() << ex.what() << std::endl;
    }
    catch (nlohmann::json::out_of_range &ex) {
        Logger::cout() << ex.what() << std::endl;
    }
}


/**
 * \brief Изменение/добавление опций соединения.
 *
 * \param connName Название соединения, по которому вноситься запись.
 * \param paramLine Опции соединения для соединения по pqxx.
 */
void ConnConfManager::setConnectionOptions(std::string connName, std::string paramsLine)
{
    jconfigState_["connections"][connName]["options"] = paramsLine;
    updateConfig();
}


/**
 * \brief Изменение/добавление id устройства, соответствующее этому соединению.
 *
 * \param connName Название соединения, по которому вноситься изменение.
 * \param device Идентификатор устройства БА, которое присуще этому
 * соединению.
 */
void ConnConfManager::setDevice(std::string connName, std::string id)
{
    jconfigState_["connections"][connName]["device"] = id;
    updateConfig();
}


/**
 * \brief Получение идентификатора устройства соединения.
 *
 * Чтение файла не происходит! Данные берутся из состояния jconfigState_.
 *
 * \param connName Название соединения
 *
 * \return Идентификатор устройства, если запись присутствует в
 * состоянии конфигурационного файла, и пустую строку иначе.
 */
std::string ConnConfManager::getDevice(std::string connName)
{
    //updateState();
    std::string id;
    try {
        id = jconfigState_
            .at("connections")
            .at(connName)
            .at("device")
            .get<std::string>();
    }
    catch (nlohmann::json::type_error &ex) {
        Logger::cout() << ex.what() << std::endl;
    }
    catch (nlohmann::json::out_of_range &ex) {
        Logger::cout() << ex.what() << std::endl;
    }
    return id;
}


/**
 * \brief Получение опций соединения.
 *
 * Чтение файла не происходит! Данные берутся из состояния jconfigState_.
 *
 * \param connName Название соединения
 *
 * \return Строка опций соединения, если запись присутствует в
 * состоянии конфигурационного файла, и пустую строку иначе.
 */
std::string ConnConfManager::getConnectionOptions(std::string connName)
{
    //updateState();
    std::string paramLine;
    try {
        paramLine = jconfigState_
            .at("connections")
            .at(connName)
            .at("options")
            .get<std::string>();
    }
    catch (nlohmann::json::type_error &ex) {
        Logger::cout() << ex.what() << std::endl;
    }
    catch (nlohmann::json::out_of_range &ex) {
        Logger::cout() << ex.what() << std::endl;
    }
    return paramLine;
}


/**
 * \brief Получение списка доступный соединений.
 *
 * Чтение файла не происходит! Данные берутся из состояния jconfigState_.
 *
 * \return Список доступных соединений в соответствии с состоянием
 * jconfigState_.
 *
 */
std::vector<std::string> ConnConfManager::getConnectionsList()
{
    //updateState();
    std::vector<std::string> connectionList;
    try {
        for (auto &conn : jconfigState_.at("connections").items()) {
            Logger::cout() << "Нашлось соединение: " << conn.key() << std::endl;
            connectionList.push_back(conn.key());
        }
    }
    catch (nlohmann::json::type_error &ex) {
        Logger::cout() << ex.what() << std::endl;
    }
    catch (nlohmann::json::out_of_range &ex) {
        Logger::cout() << ex.what() << std::endl;
    }
    return connectionList;
}


/**
 * \brief Разбить строку опций на map[параметр] = значение.
 *
 * \param paramsLine Строка с опциями соединения.
 *
 * \return Распарсенные параметры соединения.
 */
std::map <std::string, std::string> ConnConfManager::splitOptions(std::string paramsLine)
{
    std::map<std::string, std::string> params;

    std::istringstream iss(paramsLine);
    std::string key, param, value;
    std::regex pattern("\\s*(\\w+)\\s*=\\s*(\\S+)\\s*");

    std::smatch match;
    std::string::const_iterator searchStart(paramsLine.cbegin());
    while (std::regex_search(searchStart, paramsLine.cend(), match, pattern)) {
        params[match[1]] = match[2];
        //std::cout << match[1] << " = " << match[2] << std::endl;
        searchStart = match.suffix().first;
    }
    return params;
}
