#ifndef __APP_STATUS_HPP__
#define __APP_STATUS_HPP__

namespace prim
{
    struct AppStatus
    {
        bool appInitialized {};
        bool appTerminated {};
        bool rendererInitialized {};
        bool rendererTerminated {};
        bool sceneEditorInitialized {};
        bool sceneEditorTerminated {};
    };
}

#endif // __APP_STATUS_HPP__