#include "core/Engine.h"

#include "../Demo/DemoEngine.hpp"

#include <SDL_main.h>

int main(int  /*argc*/, char*  /*argvp*/[])
{
    //Engine engine;
    DemoEngine engine;

    bool initialize = engine.Initialize();
    if (!initialize)
        return 1;

    engine.Run();
    engine.Shutdown();

    return 0;
}