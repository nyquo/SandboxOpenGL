#pragma once

#include <ctime>
#include <iostream>

namespace core {

class Logger
{
  public:
    enum class LogLevel
    {
        Error,
        Warning,
        Info,
        Debug,
        Trace
    };

    static void setLogLevel(LogLevel logLevel) { s_logLevel = logLevel; }

    template<typename T>
    static void logError(const T& error)
    {
        log(LogLevel::Error, error);
    }

    template<typename T>
    static void logWarning(const T& warning)
    {
        log(LogLevel::Warning, warning);
    }

    template<typename T>
    static void logInfo(const T& info)
    {
        log(LogLevel::Info, info);
    }

    template<typename T>
    static void logDebug(const T& debug)
    {
        log(LogLevel::Debug, debug);
    }

    template<typename T>
    static void logTrace(const T& trace)
    {
        log(LogLevel::Trace, trace);
    }

  private:
    template<typename T>
    static void log(LogLevel level, const T& t)
    {
        if(s_logLevel < level)
        {
            return;
        }
        std::time_t time = std::time(nullptr);
        char strTime[50];
        std::strftime(strTime, sizeof(strTime), "%H:%M:%S", std::localtime(&time));
        std::cout << logLevelToStr(level) << " " << t << std::endl;
    }

    static std::string logLevelToStr(LogLevel level)
    {
        switch(level)
        {
            case LogLevel::Error: return "[ERROR]"; break;
            case LogLevel::Warning: return "[WARNING]"; break;
            case LogLevel::Info: return "[INFO]"; break;
            case LogLevel::Debug: return "[DEBUG]"; break;
            case LogLevel::Trace: return "[TRACE]"; break;
            default: return "[UNEXISTING_LOG_LEVEL]"; break;
        }
    }

    inline static LogLevel s_logLevel{LogLevel::Error};
};

}