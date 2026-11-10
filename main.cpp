#include "Game.h"
#include <exception>
#include <iostream>

int main()
{
    try
    {
        play_guess_game(1, 100, std::cin, std::cout);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}