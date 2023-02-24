// hintbox.cpp
#include <iostream>
#include <vector>
#include <string>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "hintBox.h"

hintBox::hintBox()
{

}

hintBox::~hintBox()
{
    clear();
}

bool hintBox::loadHintButtonTexture(SDL_Renderer* renderer)
{
    //Hint box texture
    TTF_Font* mFont = TTF_OpenFont(&PATH_COMIC_FONT[0], HINT_BOX_FONT_SIZE);
	if (mFont == NULL)
	{
		cout << "Failed to load the font. Error: " << TTF_GetError() << ".\n";
		return 0;
	}

    for (int curHint = 1; curHint <= 3; curHint ++)
    {
        string text = "Hint " + std::to_string(curHint);
        if (!(hintButtonTexture[curHint].loadTexture(renderer, mFont, &text[0], SDL_COLOR_BLACK)))
        {
            cout << "Failed to render text.\n";
            return 0;
        }
    }

	TTF_CloseFont(mFont);

    return 1;
}

void hintBox::setButtons()
{
    mHintButton[1].set(HINT_BOX_POS_X[HINT_1], HINT_BOX_POS_Y, hintButtonTexture[HINT_1].getWidth(), hintButtonTexture[HINT_1].getHeight());
    mHintButton[2].set(HINT_BOX_POS_X[HINT_2], HINT_BOX_POS_Y, hintButtonTexture[HINT_2].getWidth(), hintButtonTexture[HINT_2].getHeight());
    mHintButton[3].set(HINT_BOX_POS_X[HINT_3], HINT_BOX_POS_Y, hintButtonTexture[HINT_3].getWidth(), hintButtonTexture[HINT_3].getHeight());
}

void hintBox::setText(const std::string& text)
{
    hintText = text;
}

void hintBox::handleEvent(SDL_Event& event, const word& key)
{
    for (int curHint = 1; curHint <= 3; curHint ++)
        mHintButton[curHint].handleEvent(&event, key, hintText);
}

void hintBox::renderHintBox(SDL_Renderer* renderer, const int& livesConsumed)
{
    if (livesConsumed >= 2) mHintButton[HINT_1].setId(1), mHintButton[HINT_1].render(renderer, hintButtonTexture[HINT_1]);
    if (livesConsumed >= 4) mHintButton[HINT_2].setId(2), mHintButton[HINT_2].render(renderer, hintButtonTexture[HINT_2]);
    if (livesConsumed >= 6) mHintButton[HINT_3].setId(3), mHintButton[HINT_3].render(renderer, hintButtonTexture[HINT_3]);
}

void hintBox::renderHintText(SDL_Renderer* renderer)
{
    if (!hintText.empty()) 
        renderText(renderer, hintTextTexture, &hintText[0], HINT_TEXT_POSITION_X, HINT_TEXT_POSITION_Y, HINT_TEXT_FONT_SIZE);
}

void hintBox::clear()
{
    for (int i = HINT_1; i <= HINT_3; i ++) hintButtonTexture[i].clear();
    hintTextTexture.clear();
    hintText.clear();
}