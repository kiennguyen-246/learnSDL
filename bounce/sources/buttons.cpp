// buttons.cpp

#include "buttons.h"

LButton::LButton()
{
    mPos = {0, 0};
    mSpriteClipPtr = NULL;
}

void LButton::set(const int& x, const int& y, const int& __w, const int& __h)
{
    mPos = {x, y};
    mWidth = __w;
    mHeight = __h;
}

void LButton::set(const int& x, const int& y, const LTexture& __ButtonTexture)
{
    setTexture(__ButtonTexture);
    set(x, y, __ButtonTexture.getWidth(), __ButtonTexture.getHeight());
}

SDL_Point LButton::getPos()
{
    return mPos;
}

int LButton::getWidth()
{
    return mWidth;
}

int LButton::getHeight()
{
    return mHeight;
}

void LButton::setTexture(const LTexture& __ButtonTexture)
{
    mButtonTexture = __ButtonTexture;
    mSpriteClipPtr = NULL;
}

void LButton::setText(SDL_Renderer* renderer, const std::string& __Text, const int& fontSize, const char* fontPath, const SDL_Color& fontColor)
{
    mText = __Text;

    TTF_Font* font = TTF_OpenFont(fontPath, fontSize);

    if (!mTextTexture.loadTexture(renderer, font, &mText[0], fontColor)) 
    {
        std::cout << "Cannot load button text.\n";
    }
    else textIsSetUp = 1;
    TTF_CloseFont(font);
}

void LButton::render(SDL_Renderer* renderer)
{
    mButtonTexture.render(renderer, mPos.x, mPos.y, mSpriteClipPtr);

    if (textIsSetUp)
    {
        int textRenderPosX = mPos.x + (mWidth - mTextTexture.getWidth()) / 2;  
        int textRenderPosY = mPos.y + (mHeight - mTextTexture.getHeight()) / 2; 
        mTextTexture.render(renderer, textRenderPosX, textRenderPosY);
    }
    
}

redButton::redButton()
{

}

redButton::~redButton()
{

}

void redButton::set(SDL_Renderer* renderer, const int& x, const int& y, const std::string& text)
{
    mButtonTexture.loadTexture(renderer, &RED_BUTTON_PATH[0]);
    LButton::set(x, y, RED_BUTTON_WIDTH, RED_BUTTON_HEIGHT);
    setText(renderer, text, RED_BUTTON_TEXT_FONT_SIZE, &CALIBRI_FONT_PATH[0], SDL_COLOR_WHITE);
}