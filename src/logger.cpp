#include "logger.hpp"
#include "prim_exception.hpp"
#include <fstream>
#include <iostream>


namespace fs = std::filesystem;

const char* Logger::logfileName = "primary_engine_log.txt";
const char* Logger::logDirectoryName = "logs";
std::filesystem::path Logger::logfilePath;
std::string Logger::buffer;

void Logger::writeFile()
{
    std::ofstream fs(logfilePath.string());
    if(!fs.good()) throw PRIM_EXCEPTION("Failed to open log file for writing. Path: " + logfilePath.string());
    fs << buffer;
    buffer.clear();
    fs.close();
}

void Logger::init(std::filesystem::path appDirPath)
{
    fs::path logDirPath = appDirPath / logDirectoryName;
    if(!fs::exists(logDirPath))
    {
        if(!fs::create_directory(logDirPath)) throw PRIM_EXCEPTION("Failed to create log directory. Path: " + logDirPath.string());
    }
    logfilePath = logDirPath / logfileName;
}

void Logger::log(std::string msg, bool printAlso)
{
    buffer.append(msg);
    buffer.push_back('\n');
	if(printAlso) printLine(msg);
}

void Logger::terminate()
{
    writeFile();
}

void Logger::print(std::string msg)
{
	std::cout << msg;
	std::cout.flush();
}

void Logger::printLine(std::string msg)
{
	std::cout << msg << std::endl;
}
