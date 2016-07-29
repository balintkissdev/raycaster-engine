#include <iostream>
#include <stdexcept>

#include "Game.h"

int main()
{
    Game game;
    try
    {
        game.init();
        game.run();
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

