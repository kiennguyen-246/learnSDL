// livesBox.cpp
#include <iostream>
#include <vector>
#include <string>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "livesBox.h"

livesBox::livesBox()
{
    livesLeft = LIVES_COUNT_DEFAULT;
    livesConsumed = 0;
}
livesBox::~livesBox()
{
    clear();
}

void livesBox::set(const int& difficulty)
{
    livesLeft = (difficulty != DIFFICULTY_CHINA ? LIVES_COUNT_DEFAULT: LIVES_COUNT_CHINA);
}

bool livesBox::defeat() const
{
    return (livesLeft == 0);
}

void livesBox::consumeLife()
{
    livesLeft --;
    livesConsumed ++;
}

int livesBox::getLivesConsumed() const
{
    return livesConsumed;
}

void livesBox::render(SDL_Renderer* renderer)
{
    string livesLeftInfo = "Lives left: " + std::to_string(livesLeft);
    renderText(renderer, livesLeftBoxTexture, &livesLeftInfo[0], LIVES_LEFT_BOX_POS_X, LIVES_LEFT_BOX_POS_Y, LIVES_LEFT_BOX_FONT_SIZE);
}

void livesBox::clear()
{
    livesLeftBoxTexture.clear();
}