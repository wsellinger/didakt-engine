#include "Game.h"

int main(int argc, char* argvp[])
{
    Game game;

    bool initialize = game.Initialize();
    if (!initialize)
        return 1;

    game.Run();
    game.Shutdown();

    return 0;
}