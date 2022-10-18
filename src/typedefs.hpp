#ifndef __TYPEDEFS_HPP__
#define __TYPEDEFS_HPP__

#include <string>
#include <unordered_map>
#include <filesystem>

namespace prim
{
    namespace fs = std::filesystem;

    typedef unsigned int uint;   
    typedef std::unordered_map<std::string, std::string> FieldValues;

}


#endif // __TYPEDEFS_HPP__