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
    typedef unsigned char uchar;   

    typedef std::unordered_map<std::string, std::string> NodeValues;

    template<class T>
    using Shp = std::shared_ptr<T>;
    template<class T>
    using Unp = std::unique_ptr<T>;

}


#endif // __TYPEDEFS_HPP__