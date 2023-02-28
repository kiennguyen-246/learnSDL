// guessWord.cpp
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "guessWord.h"

const int GUESS_WORD_POSITION_X = 500;
const int GUESS_WORD_POSITION_Y = 100;
const int GUESS_WORD_FONT_SIZE = 60;

guessWord::guessWord()
{
    
}

guessWord::~guessWord()
{
    clear();
}

std::string guessWord::getWord() const
{
    return value;
}

void guessWord::init(const word& key)
{
    for (int i = 0; i < key.getLength(); i ++) value.push_back('_'); 
}

void guessWord::updateChar(const int& pos, const char& symbol)
{
    value[pos] = symbol;
}

bool guessWord::victory() const
{
    return (value.find('_') == value.npos);
}

void guessWord::render(SDL_Renderer* renderer)
{
    string spacedGuessWord = spaced(value);
    int curRenderPosX = GUESS_WORD_POSITION_X;
    int curRenderPosY = GUESS_WORD_POSITION_Y;
    std::string curRenderText = "";
    for (auto &ch: spacedGuessWord)
    {
        curRenderText.push_back(ch);
        if (curRenderText.length() == GUESS_WORD_LINE_LENGTH_LIMIT)
        {
            renderText(renderer, guessWordTexture, &curRenderText[0], curRenderPosX, curRenderPosY, GUESS_WORD_FONT_SIZE);
            curRenderPosY += guessWordTexture.getHeight();
            curRenderText.clear();
        }
    }
    renderText(renderer, guessWordTexture, &curRenderText[0], curRenderPosX, curRenderPosY, GUESS_WORD_FONT_SIZE);
}

void guessWord::clear()
{
    value.clear();
    guessWordTexture.clear();
}