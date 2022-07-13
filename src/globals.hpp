#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

#include "primary_app.hpp"

namespace prim
{
    struct Globals
    {
        static inline PrimaryApp* app = nullptr;
    };
}

#endif // __GLOBALS_HPP__