#include "primary_engine.hpp"
#include "test_scenes_creator.hpp"

int main()
{
    prim::PrimaryApp primaryApp;
    primaryApp.useEditor = true;
    primaryApp.setCurrentScene(prim::TestScenesCreator::createScene3());
    return primaryApp.run();
}