//square.cpp
#include <iostream>
#include <sdl.h>
#include <SDL_image.h>
#include "LTexture.h"

using std::cout;
using std::string;

LTexture::LTexture()
{
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    clear();
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool LTexture::loadTexture(SDL_Renderer* gRenderer, char* path)
{
    clear();
    
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        cout << "Unable to load " << path << ". Error: " << IMG_GetError() << ".\n";
        return 0;
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));
        
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            cout << "Unable to create texture from " << path << ". Error: " << SDL_GetError()<< "\n";
            return 0;
        }
        else
        {
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }
    
    mTexture = newTexture;
    return 1;
}

void LTexture::render(SDL_Renderer* gRenderer, const int &x, const int &y, SDL_Rect* clip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if (clip != NULL)
    {
        renderQuad.h = clip->h;
        renderQuad.w = clip->w;
    }
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

void LTexture::clear()
{
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = mHeight = 0;
    }
}