// hangmanModel.cpp
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "hangmanModel.h"

hangmanModel::hangmanModel()
{

}

hangmanModel::~hangmanModel()
{
    clear();
}

void hangmanModel::init(SDL_Renderer* renderer)
{
    if (!rack.loadTexture(renderer, &PATH_RACK[0]))
    {
        cout << "Failed to load image.\n"; 
        return;
    }

    for (int state = 0; state < HANGMAN_STATES_COUNT; state ++)
    {
        if (!trollHangman[state].loadTexture(renderer, &PATH_TROLL_HANGMAN[state][0]))
        {
            cout << "Failed to load image.\n"; 
            return;
        }
    }
}

void hangmanModel::render(SDL_Renderer* renderer, const int& livesConsumed)
{
    rack.render(renderer, RACK_POS_X, RACK_POS_Y);
    trollHangman[livesConsumed].render(renderer, HANGMAN_POS_X, HANGMAN_POS_Y);
}

void hangmanModel::clear()
{
    rack.clear();
    for (int i = 0; i < HANGMAN_STATES_COUNT; i ++) trollHangman[i].clear();
}