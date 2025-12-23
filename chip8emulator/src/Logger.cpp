#include "Logger.h"

//Utility
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

// PUBLICS
Logger::Logger(const std::string& logFilePath)
{
    SetLogFilePathInternal(logFilePath);
}

void Logger::SetLogFilePath(const std::string& newFilePath)
{
    GetInstance().SetLogFilePathInternal(newFilePath);
}

void Logger::Log(const std::string& info, Logger::LogSeverity severity, bool flushToFileImmediately)
{
    GetInstance().LogInternal(info, severity, flushToFileImmediately);
}

void Logger::Flush()
{
    GetInstance().FlushInternal();
}

//PRIVATES
Logger& Logger::GetInstance()
{
    static Logger logger = Logger("CHIP8_EMU_LOG.txt");
    return logger;
}

void Logger::SetLogFilePathInternal(const std::string& newFilePath)
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
        std::cout << "Couldn't open logging file." << std::endl;
    }
}

void Logger::LogInternal(const std::string& info, Logger::LogSeverity severity, bool flushToFileImmediately)
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
        std::cout << "Output stream is closed." << std::endl;
    }
    if (flushToFileImmediately)
    {
        Flush();
    }
}

void Logger::FlushInternal()
{
    if (outputStream.is_open())
    {
        outputStream.flush();
    }
    else
    {
        std::cout << "Stream isn't open. Cannot flush." << std::endl;
    }
}