#include "primary_engine.hpp"
#include "test_scenes_creator.hpp"
#include <iostream>

int main()
{
    prim::PrimaryAppOptions options;
    options.useEditor = true;
    options.windowName = "PrimaryEngine test project";

    prim::PrimaryApp primaryApp(options);
    primaryApp.setCurrentScene(prim::TestScenesCreator::createScene3());
    return primaryApp.run();
}