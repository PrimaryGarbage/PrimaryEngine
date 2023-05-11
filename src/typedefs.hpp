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

    template<typename T>
    using Unp = std::unique_ptr<T>;
    template<typename T>
    using Shp = std::shared_ptr<T>;

    template<typename T, typename... Args>
    Unp<T> mkunq(Args&&... args) { return std::make_unique<T>(std::forward<Args>(args)...); }
    template<typename T, typename... Args>
    Shp<T> mkshr(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

}


#endif // __TYPEDEFS_HPP__