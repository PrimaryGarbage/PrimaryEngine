#include "logger.hpp"
#include <fstream>
#include "prim_exception.hpp"

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

void Logger::init()
{
    fs::path currentPath = fs::current_path();
    fs::path logDirPath = currentPath / logDirectoryName;
    if(!fs::exists(logDirPath))
    {
        if(!fs::create_directory(logDirPath)) throw PRIM_EXCEPTION("Failed to create log directory. Path: " + logDirPath.string());
    }
    logfilePath = logDirPath / logfileName;
}

void Logger::log(std::string msg)
{
    buffer.append(msg);
    buffer.push_back('\n');
}

void Logger::terminate()
{
    writeFile();
}
