//LButton.cpp
#include <iostream>
#include <sdl.h>
#include "LButton.h"

LButton::LButton()
{
    mPos = {0, 0};
}

void LButton::set(const int& x, const int& y, const int& __w, const int& __h)
{
    mPos = {x, y};
    w = __w;
    h = __h;
}

SDL_Point LButton::getPos()
{
    return mPos;
}

int LButton::getWidth()
{
    return w;
}

int LButton::getHeight()
{
    return h;
}

void LButton::render(SDL_Renderer* renderer, LTexture& texture, SDL_Rect* clip)
{
    texture.render(renderer, mPos.x, mPos.y, clip);
}