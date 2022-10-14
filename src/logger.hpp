#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <string>
#include <filesystem>

class Logger
{
private:
    static const char* logfileName;
    static const char* logDirectoryName;
    static const uint16_t logfileMaxLineCount = 10;
    static std::filesystem::path logfilePath;
    static std::string buffer;

    static void writeFile();

public:
    static void init(std::filesystem::path appPath);
    static void log(std::string msg, bool printAlso = false);
    static void terminate();
	static void print(std::string msg);
	static void printLine(std::string msg);
};

#endif // __LOGGER_HPP__