#include "Game.h"

int main(int /*argc*/, char** /*argv*/)
{
    Game game;
    if (!game.init())
    {
        return EXIT_FAILURE;
    }

    return game.run();
}

