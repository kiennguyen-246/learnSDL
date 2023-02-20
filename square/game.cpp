//game.cpp
#include <iostream>
#include <sdl.h>
#include <SDL_image.h>
#include "game.h"

using std::cout;

game::game()
{
    gWindow = NULL;
    gRenderer = NULL;
}

game::~game()
{
    clear();
}

bool game::init()
{   
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed to initialize SDL. Error: " << SDL_GetError() << ".\n";
        return 0;
    }

    gWindow = SDL_CreateWindow("square", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        cout << "Failed to initialize SDL window. Error: " << SDL_GetError() << ".\n";
        return 0;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL)
    {
        cout << "Failed to initialize SDL renderer. Error: " << SDL_GetError() << ".\n";
        return 0;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "Unable to start SDL_Img. Error: "  << IMG_GetError() << ".\n";
        return 0;
    }

    return 1;
}

bool game::loadMedia(LTexture& redSquare, LTexture& blueSquare, LTexture& likeEmoji)
{
    if (!redSquare.loadTexture(gRenderer, "redSquare.png"))
    {
        cout << "Failed to load media.";
        return 0;
    }

    if (!blueSquare.loadTexture(gRenderer, "blueSquare.png"))
    {
        cout << "Failed to load media.";
        return 0;
    }

    if (!likeEmoji.loadTexture(gRenderer, "likeEmoji.png"))
    {
        cout << "Failed to load media.";
        return 0;
    }

    return 1;
}

void game::play()
{
    LTexture redSquare;
    LTexture blueSquare;
    LTexture likeEmoji;

    bool quit = 0;
    SDL_Event curEvent;
    
    if (!init())
    {
        cout << "Initialization failed.\n";
        return;
    }

    if (!loadMedia(redSquare, blueSquare, likeEmoji))
    {
        cout << "Loading media failed.\n";
        return;
    }

    int redPosX = 0;
    int redPosY = 0;
    int bluePosX = SCREEN_WIDTH - blueSquare.getWidth();
    int bluePosY = SCREEN_HEIGHT - blueSquare.getHeight();

    while (!quit)
    {
        while (SDL_PollEvent(&curEvent) != 0)
        {
            cout << curEvent.type << "\n";
            if (curEvent.type == SDL_QUIT) quit = 1;
            else if (curEvent.type == SDL_KEYDOWN)
            {
                switch(curEvent.key.keysym.sym)
                {
                    case SDLK_UP:
                        redPosY -= MOVE_STEP;
                        if (redPosY < 0) redPosY = 0;
                        break;
                    case SDLK_DOWN:
                        redPosY += MOVE_STEP;
                        if (redPosY + redSquare.getHeight() > SCREEN_HEIGHT) redPosY = SCREEN_HEIGHT - redSquare.getHeight();
                        break;
                    case SDLK_LEFT:
                        redPosX -= MOVE_STEP;
                        if (redPosX < 0) redPosX = 0;
                        break;
                    case SDLK_RIGHT:
                        redPosX += MOVE_STEP;
                        if (redPosX + redSquare.getWidth() > SCREEN_WIDTH) redPosX = SCREEN_WIDTH - redSquare.getWidth();
                        break;
                    default: break;
                }
            }
        }


        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        blueSquare.render(gRenderer, bluePosX, bluePosY);
        redSquare.render(gRenderer, redPosX, redPosY);
        

        if (redPosX >= bluePosX && redPosY >= bluePosY)
        {
            int likeEmojiPosX = (SCREEN_WIDTH - likeEmoji.getWidth()) / 2;
            int likeEmojiPosY = (SCREEN_HEIGHT - likeEmoji.getHeight()) / 2;

            likeEmoji.render(gRenderer, likeEmojiPosX, likeEmojiPosY);
            
        }   

        SDL_RenderPresent(gRenderer);
    }

    redSquare.clear();
    blueSquare.clear();
    likeEmoji.clear();
    clear();
}

void game::clear()
{
    SDL_DestroyRenderer(gRenderer); gRenderer = NULL;
    SDL_DestroyWindow(gWindow); gWindow = NULL;
    IMG_Quit();
    SDL_Quit();
}