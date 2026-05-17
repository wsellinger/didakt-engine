#include "Engine.h"

int main(int argc, char* argvp[])
{
    Engine engine;

    bool initialize = engine.Initialize();
    if (!initialize)
        return 1;

    engine.Run();
    engine.Shutdown();

    return 0;
}