// startPage.cpp
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "startPage.h"

void startButton::init(SDL_Renderer* renderer)
{
    TTF_Font* font = TTF_OpenFont(&PATH_COMIC_FONT[0], START_BUTTON_FONT_SIZE);
    if (!sprite.loadTexture(renderer, font, "START", SDL_COLOR_BLACK))
    {
        cout << "Cannot render given text.\n";
    }
    TTF_CloseFont(font);

    set(START_BUTTON_POS_X, START_BUTTON_POS_Y, sprite.getWidth(), sprite.getHeight());
}

void startButton::handleEvent(SDL_Event* event, START_PAGE_STATUS& returnStatus)
{
    SDL_Point pos = getPos();
    int w = getWidth();
    int h = getHeight();

    
    if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEBUTTONDOWN)
    {
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);

        bool inside = 1;

        if (x < pos.x || x > pos.x + w || y < pos.y || y > pos.y + h) inside = 0;

        if (inside && event->type == SDL_MOUSEBUTTONDOWN) 
        {
            trigger(returnStatus);
        }
        
    }
}

void startButton::trigger(START_PAGE_STATUS& returnStatus)
{
    returnStatus = START_PAGE_PLAY;
}

void startButton::clear()
{
    sprite.clear();
}

startPage::startPage(/* args */)
{
}

startPage::~startPage()
{
    clear();
}

void startPage::preset(SDL_Window* window, SDL_Renderer* renderer)
{
    mWindow = window;
    mRenderer = renderer;

    mStartButton.init(mRenderer);

    trollIntro.loadTexture(mRenderer, &PATH_TROLL_INTRO[0]);
}

void startPage::startStartPage(START_PAGE_STATUS& returnStatus)
{
    bool gameOver = 0;

    bool quit = 0;
    SDL_Event curEvent;
    int frameCount = 0;

    while (!quit)
    {
        bool keyboardTriggered = 0;
        bool isIn = 0;
        
        while (SDL_PollEvent(&curEvent) != 0)
        {
            if (curEvent.type == SDL_QUIT)
            {
                quit = 1;
                returnStatus = START_PAGE_QUIT;
            } 
            else 
            {
                mStartButton.handleEvent(&curEvent, returnStatus);
            }
        }

        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);

        renderText(mRenderer, titleTexture, "HANGMAN", START_PAGE_TITLE_RENDER_POS_X, START_PAGE_TITLE_RENDER_POS_Y, START_PAGE_TITLE_FONT_SIZE, &PATH_COMIC_FONT_BOLD[0]);
        
        mStartButton.render(mRenderer, mStartButton.sprite);

        trollIntro.render(mRenderer, TROLL_INTRO_1_POS_X, TROLL_INTRO_1_POS_y);
        trollIntro.render(mRenderer, TROLL_INTRO_2_POS_X, TROLL_INTRO_2_POS_y, NULL, 0, NULL, SDL_FLIP_HORIZONTAL);

        SDL_RenderPresent(mRenderer); 

        ++frameCount;
        if (frameCount > 1e8) frameCount = 0;

        if (returnStatus != START_PAGE_NULL) quit = 1;
    }
    clear();
}

void startPage::clear()
{
    mStartButton.clear();
    titleTexture.clear();
}