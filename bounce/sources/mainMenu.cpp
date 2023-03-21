// mainMenu.cpp

#include "mainMenu.h"

newGameButton::newGameButton()
{

}

newGameButton::~newGameButton()
{

}

void newGameButton::handleEvent(SDL_Event* event, bool& isTriggered)
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
            isTriggered = 1;
        }
        
    }
}

mainMenu::mainMenu()
{

}

mainMenu::~mainMenu()
{

}

mainMenu::mainMenu(SDL_Renderer* __Renderer)
{
    init(__Renderer);
}

void mainMenu::init(SDL_Renderer* __Renderer)
{
    mRenderer = __Renderer;
    
    mBackgroundTexture.loadTexture(mRenderer, &BACKGROUND_PATH[0]);

    mGameLogoTexture.loadTexture(mRenderer, &GAME_LOGO_PATH[0]);

    mNewGameButton.set(mRenderer, NEW_GAME_BUTTON_RENDER_POS_X, NEW_GAME_BUTTON_RENDER_POS_Y, NEW_GAME_BUTTON_TEXT);
}

MAIN_MENU_EXIT_STATUS mainMenu::render()
{
    bool quit = 0;
    SDL_Event curEvent;
    MAIN_MENU_EXIT_STATUS exitStatus = MAIN_MENU_EXIT_NULL;

    while (!quit)
    {
        while (SDL_PollEvent(&curEvent) != 0)
        {
            if (curEvent.type == SDL_QUIT) quit = 1;

            bool newGameButtonTriggered = 0;
            mNewGameButton.handleEvent(&curEvent, newGameButtonTriggered);
            if (newGameButtonTriggered)
            {
                quit = 1;
                exitStatus = MAIN_MENU_EXIT_NEW_GAME;
            }
        }

        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);

        mBackgroundTexture.render(mRenderer, 0, 0);
        mGameLogoTexture.render(mRenderer, GAME_LOGO_RENDER_POS_X, GAME_LOGO_RENDER_POS_Y, NULL, GAME_LOGO_RENDER_STRETCH);
        
        mNewGameButton.render(mRenderer);

        SDL_RenderPresent(mRenderer);
    }

    return exitStatus;
}