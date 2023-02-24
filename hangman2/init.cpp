//init.cpp
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include "init.h"

using namespace std;
using random::randInt;

int random::randInt(const int& limit)
{
    int x = rand();
    int y = rand();
    int z = rand();
    return (1ll * x * y + z) % (limit + 1);
}

int random::randInt(const int& lLimit, const int& rLimit)
{
    return randInt(rLimit - lLimit) + lLimit;
}

void random::initRand()
{
    srand(int(time(0)));
}

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

void LTexture::render(SDL_Renderer* mRenderer, const int &x, const int &y, SDL_Rect* clip)
{
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};
    if (clip != NULL)
    {
        renderQuad.h = clip->h;
        renderQuad.w = clip->w;
    }
    SDL_RenderCopy(mRenderer, mTexture, clip, &renderQuad);
}

void LTexture::render(SDL_Renderer* mRenderer, int& x, int& y, SDL_Rect* clip, const double& angle, SDL_Point* center, SDL_RendererFlip flip)
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

word::word()
{
    value = "";
    hint1 = "";
    hint2 = "";
    hint3 = "";
    len = 0;
}

word::word(const string& __value, const string& __hint1, const string& __hint2, const string& __hint3)
{
    value = __value;
    hint1 = __hint1;
    hint2 = __hint2;
    hint3 = __hint3;
    len = value.length();
}

word::~word()
{
    clear();
}

string word::getWord() const
{
    return value;
}

string word::getHint1() const
{
    return hint1;
}

string word::getHint2() const
{
    return hint2;
}

string word::getHint3() const
{
    return hint3;
}

int word::getLength() const
{
    return len;
}

void word::clear()
{
    value.clear();
    hint1.clear();
    hint2.clear();
    hint3.clear();
    len = 0;
}

void dictionary::optimize(string& line)
{
    for (auto &ch : line)
    {
        if (ch == ' ') ch = '_';
        else if (ch == ',') ch = ' ';
        else if (ch == ';') ch = ',';
    }
}

void dictionary::rollBack(string& item)
{
    for (auto &ch : item)
    {
        if (ch == '_') ch = ' ';
    }
}

void dictionary::init()
{
    ifstream fi(PATH_DICTIONARY);
    if (!fi.is_open()) return;
    string line;
    int curDifficulty = 0;
    while(getline(fi, line))
    {
        optimize(line);

        stringstream ssLine(line);
        int cnt = 0;
        string value, hint1, hint2, hint3, item;
        while (ssLine >> item)
        {
            rollBack(item);
            cnt ++;
            if (cnt == 1) value = item;
            if (cnt == 2) hint1 = item;
            if (cnt == 3) hint2 = item;
            if (cnt == 4) hint3 = item;
        }
        if (value[0] == 'e' || value[0] == 'n' || value[0] == 'd' || value[0] == 'c') 
        {
            curDifficulty++;
            continue;
        }
        if (value == "word") continue;
        vWord[curDifficulty].push_back(word(value, hint1, hint2, hint3));
        size[curDifficulty] ++;
    }
}

word dictionary::getWord(const int& difficulty) const
{
    int pos = randInt(0, vWord[difficulty].size() - 1);
    return vWord[difficulty][pos];
}

string spaced(const string& __word)
{
    string ret = "";
    for (auto i : __word) ret.push_back(i), ret.push_back(' ');
    return ret; 
}

void renderText(SDL_Renderer* renderer, LTexture& texture, const char* text, const int& x, const int& y, 
                      const int& fontSize, const char* fontPath, const SDL_Color& fontColor)
{
    TTF_Font* font = TTF_OpenFont(fontPath, fontSize);

    if (!texture.loadTexture(renderer, font, text, fontColor)) 
    {
        cout << "Cannot render given text.\n";
    }
    else
    {
        texture.render(renderer, x, y);
    }
    TTF_CloseFont(font);
}