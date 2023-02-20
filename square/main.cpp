//main.cpp
#include <iostream>
#include <sdl.h>
#include <SDL_image.h>
#include "game.h"

using std::cout;

int main(int argc, char* argv[])
{
    auto* __game = new game;
    __game->play();
    
}