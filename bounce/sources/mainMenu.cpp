// mainMenu.cpp

#include "mainMenu.h"

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
    mContinueButton.set(mRenderer, CONTINUE_BUTTON_RENDER_POS_X, CONTINUE_BUTTON_RENDER_POS_Y, CONTINUE_BUTTON_TEXT);
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

            bool buttonTriggered = 0;
            mNewGameButton.handleEvent(&curEvent, buttonTriggered);
            if (buttonTriggered)
            {
                quit = 1;
                exitStatus = MAIN_MENU_EXIT_NEW_GAME;
            }

            buttonTriggered = 0;
            mContinueButton.handleEvent(&curEvent, buttonTriggered);
            if (buttonTriggered)
            {
                quit = 1;
                exitStatus = MAIN_MENU_EXIT_CONTINUE;
            }
        }

        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);

        mBackgroundTexture.render(mRenderer, 0, 0);
        mGameLogoTexture.render(mRenderer, GAME_LOGO_RENDER_POS_X, GAME_LOGO_RENDER_POS_Y, NULL, GAME_LOGO_RENDER_STRETCH);
        
        mNewGameButton.render(mRenderer);
        mContinueButton.render(mRenderer);

        SDL_RenderPresent(mRenderer);
    }

    return exitStatus;
}