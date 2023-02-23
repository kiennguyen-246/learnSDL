//charButton.cpp
#include <iostream>
#include <sdl.h>
#include "charButton.h"

bool charButton::isUsed()
{
    return __isUsed;
}

void charButton::useKey()
{
    __isUsed = 1;
}

void charButton::updateSymbol(char newSym)
{
    symbol = newSym;
}

void charButton::handleEvent(SDL_Event* event, const word& key, string& curWord, bool& isTriggered, bool& isIn)
{
    SDL_Point pos = getPos();
    int w = getWidth();
    int h = getHeight();

    
    if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEBUTTONDOWN)
    {
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);

        bool inside = 1;

        if (x < pos.x || x > pos.x + w || y < pos.y || y > pos.y + h) inside = 0;

        if (!isUsed() && inside && event->type == SDL_MOUSEBUTTONDOWN) 
        {
            trigger(key, curWord, isIn);
            isTriggered = 1;
        }
        
    }
}

void charButton::trigger(const word& key, string& curWord, bool& isIn)
{
    useKey();
    for (int i = 0; i < key.getLength(); i ++)
        if (key.getWord()[i] == symbol)
        {
            curWord[i] = symbol;
            isIn = 1;
        } 
    return;
}