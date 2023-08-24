#include "Game.h" 
#include <string>
#include <iostream>

int main()
{
    Game g(std::string("config.txt"));
    g.run();
    

    return 0;
}