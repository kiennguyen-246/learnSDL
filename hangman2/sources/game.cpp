//game.cpp
#include <iostream>
#include <vector>
#include <string>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "game.h"

using std::cout;
using random::randInt;

game::game()
{
    mWindow = NULL;
    mRenderer = NULL;
}

game::~game()
{
    clear();
}

bool game::initSDL()
{   
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed to initialize SDL. Error: " << SDL_GetError() << ".\n";
        return 0;
    }

    mWindow = SDL_CreateWindow("hangman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == NULL)
    {
        cout << "Failed to initialize SDL window. Error: " << SDL_GetError() << ".\n";
        return 0;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (mRenderer == NULL)
    {
        cout << "Failed to initialize SDL renderer. Error: " << SDL_GetError() << ".\n";
        return 0;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "Unable to start SDL_Img. Error: "  << IMG_GetError() << ".\n";
        return 0;
    }

    if (TTF_Init() == -1)
    {
        cout << "Unable to start SDL_TTF. Error: " << TTF_GetError() << ".\n";
        return 0;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 || Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) < 0) ///wav or mp3
    {
        cout << "Unable to start SDL_Mixer. Error: " << Mix_GetError() << ".\n";
        return 0;
    }

    return 1;
}

void game::preset()
{
    random::initRand();
    
    if (!initSDL())
    {
        cout << "SDL initialization failed.\n";
        return;
    }

    mStartPage.preset(mWindow, mRenderer);
    mChooseDifficultyPage.preset(mWindow, mRenderer);
    mPlaygamePage.preset(mWindow, mRenderer);
}

void game::play()
{
    bool isRestarted = 1;
    while (isRestarted)
    {
        mStartPage.preset(mWindow, mRenderer);
        mChooseDifficultyPage.preset(mWindow, mRenderer);
        mPlaygamePage.preset(mWindow, mRenderer);
        
        START_PAGE_STATUS status = START_PAGE_NULL;
        mStartPage.startStartPage(status);
        switch(status)
        {
            case START_PAGE_PLAY:
            {
                GAME_DIFFICULTY difficulty = DIFFICULTY_NULL;
                mChooseDifficultyPage.startChooseDifficultyPage(difficulty);
                mPlaygamePage.startPlaygamePage(difficulty);
                break;
            }
            default:
                break;
        }

        isRestarted = mPlaygamePage.playAgain();
    }
    clear();
}

void game::clear()
{
    SDL_DestroyRenderer(mRenderer); mRenderer = NULL;
    SDL_DestroyWindow(mWindow); mWindow = NULL;

    mStartPage.clear();
    mChooseDifficultyPage.clear();
    mPlaygamePage.clear();

    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}