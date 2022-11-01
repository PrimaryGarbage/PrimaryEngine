#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <string>
#include <filesystem>

namespace prim
{

class Logger
{
private:
    const char* logfileName = "primary_engine_log.txt";
    const char* logDirectoryName = "logs";
    const uint16_t logfileMaxLineCount = 1000;
    std::string buffer = std::string();

    void writeFile();

public:
    ~Logger();

    void init(std::filesystem::path appPath);
    void log(std::string msg, bool printAlso = false);
    void terminate();
	void print(std::string msg);
	void printLine(std::string msg);
};

} // namespace prim


#endif // __LOGGER_HPP__