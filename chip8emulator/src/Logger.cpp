#include "Logger.h"

void ClearFileIfExists(const std::string& filePath)
{
    std::ofstream testStream = std::ofstream(filePath);
    if (testStream.good())
    {
        if (testStream.is_open())
        {
            testStream.close();
        }
    }
}

Logger::Logger(const std::string& logFilePath)
{
    SetLogFilePath(logFilePath);
}

Logger& Logger::GetInstance()
{
    static Logger logger = Logger("CHIP8_EMU_LOG.txt");
    return logger;
}

void Logger::SetLogFilePath(const std::string& newFilePath)
{
    //Don't update if the new file path is the same as the old one.
    if (newFilePath == logFilePath)
    {
        return;
    }

    //Close the output stream first
    if (outputStream.is_open())
    {
        outputStream.close();
    }

    ClearFileIfExists(newFilePath);

    //Update the log file path
    this->logFilePath = newFilePath;
    outputStream = std::ofstream(this->logFilePath, std::ios_base::app);
    if (!outputStream.is_open())
    {
        throw std::exception("Couldn't open logging file.");
    }
}

void Logger::Log(const std::string& info, Logger::LogSeverity severity, bool flushToFileImmediately)
{
    std::stringstream logText = std::stringstream();
    switch (severity)
    {
    case Logger::LogSeverity::LOGSEVERITY_INFO:
        logText << "[INFO]: ";
        break;
    case Logger::LogSeverity::LOGSEVERITY_WARNING:
        logText << "[WARNING]: ";
        break;
    case Logger::LogSeverity::LOGSEVERITY_ERROR:
        logText << "[ERROR]: ";
        break;
    default:
        break;
    }
    logText << info << "\n";
    if (outputStream.is_open())
    {
        outputStream << logText.str();
    }
    else
    {
        throw std::exception("Output stream is closed");
    }
    if (flushToFileImmediately)
    {
        Flush();
    }
}

void Logger::Flush()
{
    if (outputStream.is_open())
    {
        outputStream.flush();
    }
    else
    {
        throw std::exception("Stream isn't open. Cannot flush.");
    }
}