#pragma once

#include <ctime>
#include <ostream>
#include <string>
#include <iostream>


struct Logger
{
    static std::ostream& cout() { return WithTime(std::cout); }
    static std::ostream& WithTime (std::ostream& stream)
    {
        std::time_t currentTime = std::time(nullptr);
        std::string timestamp = std::asctime(std::localtime(&currentTime));
        timestamp = timestamp.substr(0, timestamp.length() - 1);
        return (stream << "[" << timestamp << "] ");
    }
    static void debug() { Logger::cout() << "[DEBUG]" << std::endl; }
};

