#pragma once

#include <string>
#include <sstream>
#include <fstream>

class Logger
{
private:
    std::string logFilePath;
    std::ofstream outputStream;
    Logger(const std::string& logFilePath);

public:
    enum class LogSeverity
    {
        LOGSEVERITY_INFO,
        LOGSEVERITY_WARNING,
        LOGSEVERITY_ERROR,
        LOGSEVERITY_COUNT
    };
    static Logger& GetInstance();
    void SetLogFilePath(const std::string& newFilePath);
    void Log(const std::string& info, Logger::LogSeverity severity, bool flushToFileImmediately = true);
    void Flush();
};
