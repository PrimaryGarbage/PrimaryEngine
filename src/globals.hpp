#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__

#include "primary_app.hpp"
#include "app_status.hpp"

namespace prim
{
    struct Globals
    {
        static inline PrimaryApp* app = nullptr;
        static inline Renderer* mainRenderer = nullptr;
        static inline SceneEditor* sceneEditor = nullptr;
        static inline SceneManager* sceneManager = nullptr;
        static inline Logger* logger = nullptr;
        static inline AppStatus appStatus {};
    };
}

#endif // __GLOBALS_HPP__