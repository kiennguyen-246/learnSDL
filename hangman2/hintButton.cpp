//hintButton.cpp

#include <iostream>
#include <SDL.h>
#include "hintButton.h"

hintButton::hintButton()
{
    id = 0;
}

void hintButton::setId(const int& newId)
{
    id = newId;
}

void hintButton::handleEvent(SDL_Event* event, const word& __word, std::string& hint)
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

        if (inside && event->type == SDL_MOUSEBUTTONDOWN) 
        {
            trigger(__word, hint);
        }
        
    }
}

void hintButton::trigger(const word& __word, std::string& hint)
{
    switch(id)
    {
        case 1: hint = __word.getHint1(); break;
        case 2: hint = __word.getHint2(); break;
        case 3: hint = __word.getHint3(); break;
        default: break;
    }
}