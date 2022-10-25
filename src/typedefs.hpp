#ifndef __TYPEDEFS_HPP__
#define __TYPEDEFS_HPP__

#include <string>
#include <unordered_map>
#include <filesystem>
#include <memory>

namespace prim
{
    namespace fs = std::filesystem;

    typedef unsigned int uint;   
    typedef std::unordered_map<std::string, std::string> FieldValues;

    template<class T>
    using shptr = std::shared_ptr<T>;
}


#endif // __TYPEDEFS_HPP__