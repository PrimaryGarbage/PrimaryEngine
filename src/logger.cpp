#include "logger.hpp"
#include "prim_exception.hpp"
#include "utils.hpp"
#include "typedefs.hpp"
#include <fstream>
#include <iostream>


namespace fs = std::filesystem;

namespace prim
{

void Logger::writeFile()
{
    fs::path logfileDir = Utils::getAppDirPath() / logDirectoryName;
    fs::create_directories(logfileDir);
    fs::path logfilePath = logfileDir / logfileName;
    std::ofstream fs(logfilePath.string());
    if(!fs.good()) throw PRIM_EXCEPTION("Failed to open log file for writing. Path: " + logfilePath.string());
    fs << buffer;
    buffer.clear();
    fs.close();
}

Logger::~Logger() 
{
    terminate();
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

} // namespace prim

