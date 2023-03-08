// base.cpp
#include "base.h"

using std::cout;

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

bool LTexture::loadTexture(SDL_Renderer* gRenderer, const char* path)
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

bool LTexture::loadTexture(SDL_Renderer* mRenderer, TTF_Font* mFont, const char* text, const SDL_Color& textColor)
{
	clear();

	SDL_Texture* newTexture = NULL;
	SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, text, textColor);
    if (textSurface == NULL)
    {
        cout << "Unable to render text. Error code: " << TTF_GetError() << ".\n";
        return 0;
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
        if (newTexture == NULL)
        {
            cout << "Unable to create texture from given text. Error code: " << SDL_GetError()<< "\n";
            return 0;
        }
        else
        {
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    
    mTexture = newTexture;
    return 1;

}

void LTexture::render(SDL_Renderer* mRenderer, const int& x, const int& y, SDL_Rect* clip, const double& angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if (clip != NULL)
    {
        renderQuad.h = clip->h;
        renderQuad.w = clip->w;
    }
    SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
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

gameObject::gameObject()
{

}

gameObject::~gameObject()
{

}

void gameObject::setPos(const int& x, const int& y)
{
    mPosX = x;
    mPosY = y;
}

void gameObject::setPos(const std::pair <int, int>& __Pos)
{
    setPos(__Pos.first, __Pos.second);
}

int gameObject::getPosX() const
{
    return mPosX;
}

int gameObject::getPosY() const
{
    return mPosY;
}

void gameObject::setSpriteClip(LTexture& spritesheet, const int& x, const int& y, const int& w, const int& h) 
{
    mSpriteClip = {x, y, w, h};
}