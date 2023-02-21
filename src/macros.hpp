#ifndef __MACROS_HPP__
#define __MACROS_HPP__

#include "globals.hpp"
#include "prim_exception.hpp"

#define ASSERT_GLFW_NOT_TERMINATED\
    if(Globals::appStatus.rendererTerminated)\
    {\
        Globals::logger->logError("Trying to manipulate GLFW dependent entities after GLFW termination!");\
        throw PRIM_EXCEPTION("Trying to manipulate GLFW dependent entities after GLFW termination!");\
    }

#define ASSERT_APP_INITIALIZED\
    if(!Globals::appStatus.appInitialized)\
    {\
        Globals::logger->logError("Calling Primary App functions before PrimaryApp::init() is forbidden");\
        throw PRIM_EXCEPTION("Calling Primary App functions before PrimaryApp::init() is forbidden");\
    }

#endif // __MACROS_HPP__