#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <format>

class Logger
{
private:
    std::string logFilePath;
    std::ofstream outputStream;
    Logger(const std::string& logFilePath);
    
public:
    Logger(const Logger&) = delete;
    enum class LogSeverity
    {
        LOGSEVERITY_INFO,
        LOGSEVERITY_WARNING,
        LOGSEVERITY_ERROR
    };
    static void SetLogFilePath(const std::string& newFilePath);
    static void Log(const std::string& info, Logger::LogSeverity severity = Logger::LogSeverity::LOGSEVERITY_INFO, bool flushToFileImmediately = true);
    static void Flush();
private:
    static Logger& GetInstance();
    void SetLogFilePathInternal(const std::string& newFilePath);
    void LogInternal(const std::string& info, Logger::LogSeverity severity, bool flushToFileImmediately = true);
    void FlushInternal();
};
