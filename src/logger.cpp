#include "logger.hpp"
#include "prim_exception.hpp"
#include "utils.hpp"
#include "typedefs.hpp"
#include <fstream>
#include <iostream>


#ifdef _WIN32
#include <windows.h>
#endif


namespace fs = std::filesystem;

namespace prim
{

void Logger::writeFile()
{
    fs::path logfileDir = getAppDirPath() / logDirectoryName;
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

Logger& Logger::inst()
{
    std::lock_guard lock(defaultInstanceMutex);
    static Logger logger;
    return logger;
}

void Logger::logInfo(std::string msg, bool printAlso)
{
    if(logLevel < LogLevel::Info) return;

    static const char* prefix = " <INFO>: ";

    std::string message = Utils::currentDateTimeString("[%d-%m-%Y %H:%M:%S]") + prefix + msg;

    buffer.append(message);
    buffer.push_back('\n');
	if(printAlso) printLine(message);
}

void Logger::logWarning(std::string msg, bool printAlso) 
{
    if(logLevel < LogLevel::Warning) return;

    static const char* prefix = " <WARNING>: ";

    std::string message = Utils::currentDateTimeString("[%d-%m-%Y %H:%M:%S]") + prefix + msg;

    buffer.append(message);
    buffer.push_back('\n');
	if(printAlso) printLine(message);
}

void Logger::logError(std::string msg, bool printAlso) 
{
    if(logLevel < LogLevel::Error) return;

    static const char* prefix = " <ERROR>: ";

    std::string message = Utils::currentDateTimeString("[%d-%m-%Y %H:%M:%S]") + prefix + msg;

    buffer.append(message);
    buffer.push_back('\n');
	if(printAlso) printLine(message);
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

fs::path Logger::getAppDirPath()
{
    static fs::path path;
    if(path.empty())
    {
        #ifdef _WIN32
            #ifdef UNICODE
                WCHAR pathArr[400];
                GetModuleFileName(NULL, pathArr, (sizeof(pathArr)));
                path = fs::path(std::wstring(pathArr));
            #else
                CHAR pathArr[400];
                GetModuleFileName(NULL, pathArr, (sizeof(pathArr)));
                path = fs::path(std::string(pathArr));
            #endif // ifdef UNICODE
        #else
            path = fs::canonical("/proc/self/exe");
        #endif

        path = path.parent_path();
    }
    return path;
}

} // namespace prim

