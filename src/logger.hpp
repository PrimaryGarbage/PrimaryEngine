#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <string>
#include <filesystem>
#include <mutex>
#include "typedefs.hpp"

namespace prim
{

class Logger
{
private:
    const char* logfileName = "primary_engine_log.txt";
    const char* logDirectoryName = "logs";
    const uint16_t logfileMaxLineCount = 1000;
    std::string buffer = std::string();

    static inline std::mutex defaultInstanceMutex;

    void writeFile();
    fs::path getAppDirPath();

public:
    enum LogLevel { NoLog, Error, Warning, Info };
    LogLevel logLevel = LogLevel::Info;

    ~Logger();

    static Logger& inst();
    void init(std::filesystem::path appPath);
    void logInfo(std::string msg, bool printAlso = true);
    void logWarning(std::string msg, bool printAlso = true);
    void logError(std::string msg, bool printAlso = true);
    void terminate();
	void print(std::string msg);
	void printLine(std::string msg);

};

} // namespace prim


#endif // __LOGGER_HPP__