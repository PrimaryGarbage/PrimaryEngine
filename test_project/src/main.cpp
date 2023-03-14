#include "primary_engine.hpp"
#include "test_scenes_creator.hpp"
#include <iostream>

int main()
{
    try
    {
        using namespace prim;

        PrimaryAppOptions options;
        options.useEditor = true;
        options.windowName = "PrimaryEngine test project";

        PrimaryApp primaryApp(options);
        primaryApp.setCurrentScene(TestScenesCreator::createScene3());
        return primaryApp.run();
    }
    catch(prim::Exception ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}