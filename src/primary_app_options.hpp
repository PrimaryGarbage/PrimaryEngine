#ifndef __PRIMARY_APP_OPTIONS_HPP__
#define __PRIMARY_APP_OPTIONS_HPP__ 

#include <string>

namespace prim
{
    struct PrimaryAppOptions
    {
        bool useEditor;
        std::string windowName;
        uint32_t windowWidth = 800u;
        uint32_t windowHeight = 800u;
    };
}

#endif // __PRIMARY_APP_OPTIONS_HPP__