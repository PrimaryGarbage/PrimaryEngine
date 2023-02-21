#include "primary_engine.hpp"
#include "test_scenes_creator.hpp"
#include <iostream>

int main()
{
    prim::PrimaryApp primaryApp;
    primaryApp.useEditor = true;
    try
    {
        primaryApp.init();
        primaryApp.setCurrentScene(prim::TestScenesCreator::createScene3());
    }
    catch(prim::Exception ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return primaryApp.run();
}