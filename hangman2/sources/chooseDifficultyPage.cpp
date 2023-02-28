// chooseDifficultyPage.cpp
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "chooseDifficultyPage.h"

void difficultyButton::init(SDL_Renderer* renderer, const GAME_DIFFICULTY& __difficulty)
{
    difficulty = __difficulty;
    
    TTF_Font* font = TTF_OpenFont(&PATH_COMIC_FONT[0], DIFFICULTY_BUTTON_FONT_SIZE);

    std::string difficultyText = "";
    switch(difficulty)
    {
        case DIFFICULTY_EASY: 
            difficultyText = "Easy"; 
            break;
        case DIFFICULTY_NORMAL: 
            difficultyText = "Normal"; 
            break;
        case DIFFICULTY_DIFFICULT: 
            difficultyText = "Difficult"; 
            break;
        case DIFFICULTY_CHINA: 
            difficultyText = "China"; 
            break;
        default: break;
    }

    SDL_Color textColor = SDL_COLOR_BLACK;
    if (difficulty == DIFFICULTY_CHINA)  textColor = SDL_COLOR_RED;

    if (!sprite.loadTexture(renderer, font, &difficultyText[0], textColor))
    {
        cout << "Cannot render given text.\n";
    }

    TTF_CloseFont(font);

    set(DIFFICULTY_BUTTON_POS_X[difficulty], DIFFICULTY_BUTTON_POS_Y[difficulty], sprite.getWidth(), sprite.getHeight());
}

void difficultyButton::handleEvent(SDL_Event* event, GAME_DIFFICULTY& returnDifficulty)
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
            trigger(returnDifficulty);
        }
        
    }
}

void difficultyButton::trigger(GAME_DIFFICULTY& returnDifficulty)
{
    returnDifficulty = difficulty;
}

chooseDifficultyPage::chooseDifficultyPage()
{

}

chooseDifficultyPage::~chooseDifficultyPage()
{
    clear();
}

void chooseDifficultyPage::preset(SDL_Window* window, SDL_Renderer* renderer)
{
    mWindow = window;
    mRenderer = renderer;

    for (int dif = DIFFICULTY_EASY; dif <= DIFFICULTY_CHINA; dif ++)
        mDifficultyButton[dif].init(mRenderer, GAME_DIFFICULTY(dif));

    trollIntro.loadTexture(mRenderer, &PATH_TROLL_INTRO[0]);
}

void chooseDifficultyPage::startChooseDifficultyPage(GAME_DIFFICULTY& difficulty)
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
                difficulty = DIFFICULTY_NULL;
            } 
            else 
            {
                for (int dif = DIFFICULTY_EASY; dif <= DIFFICULTY_CHINA; dif ++)
                    mDifficultyButton[dif].handleEvent(&curEvent, difficulty);
            }
        }

        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);

        renderText(mRenderer, titleTexture, "CHOOSE DIFFICULTY", CHOOSE_DIFFICULTY_PAGE_TITLE_POS_X, CHOOSE_DIFFICULTY_PAGE_TITLE_POS_Y, CHOOSE_DIFFICULTY_PAGE_TITLE_FONT_SIZE, &PATH_COMIC_FONT[0]);
        
        for (int dif = DIFFICULTY_EASY; dif <= DIFFICULTY_CHINA; dif ++)
                    mDifficultyButton[dif].render(mRenderer, mDifficultyButton[dif].sprite);

        trollIntro.render(mRenderer, TROLL_INTRO_1_POS_X, TROLL_INTRO_1_POS_y);
        trollIntro.render(mRenderer, TROLL_INTRO_2_POS_X, TROLL_INTRO_2_POS_y, NULL, 0, NULL, SDL_FLIP_HORIZONTAL);

        SDL_RenderPresent(mRenderer); 

        ++frameCount;
        if (frameCount > 1e8) frameCount = 0;

        if (difficulty != DIFFICULTY_NULL) quit = 1;
    }
    clear();
}

void chooseDifficultyPage::clear()
{

}