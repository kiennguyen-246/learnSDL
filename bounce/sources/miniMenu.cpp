// miniMenu.cpp

#include "miniMenu.h"

miniMenu::miniMenu(/* args */)
{
    mContainer = {MINI_MENU_RENDER_POS_X, MINI_MENU_RENDER_POS_Y, MINI_MENU_WIDTH, MINI_MENU_HEIGHT};
}

miniMenu::~miniMenu()
{
}

gameOverMenu::gameOverMenu()
{

}

gameOverMenu::~gameOverMenu()
{

}

void gameOverMenu::set(SDL_Renderer* renderer)
{
    isVictory = 0;
    mScore = 0;
    mMainMenuButton.set(renderer, MAIN_MENU_BUTTON_RENDER_POS_X, MAIN_MENU_BUTTON_RENDER_POS_Y, MAIN_MENU_BUTTON_TEXT);
    mReplayButton.set(renderer, REPLAY_BUTTON_RENDER_POS_X, REPLAY_BUTTON_RENDER_POS_Y, REPLAY_BUTTON_TEXT);

    mYellowSad.loadTexture(renderer, &YELLOW_SAD_EMOJI_PATH[0]);
    mYellowSunglasses.loadTexture(renderer, &YELLOW_SUNGLASSES_EMOJI_PATH[0]);
}

void gameOverMenu::handleEvent(SDL_Event* event, GAME_OVER_MENU_EXIT_STATUS& exitStatus)
{
    bool buttonTriggered = 0;
    mMainMenuButton.handleEvent(event, buttonTriggered);
    if (buttonTriggered) exitStatus = GAME_OVER_EXIT_MAIN_MENU;

    buttonTriggered = 0;
    mReplayButton.handleEvent(event, buttonTriggered);
    if (buttonTriggered) exitStatus = GAME_OVER_EXIT_REPLAY;
}

void gameOverMenu::setVictory(const bool& __isVictory)
{
    isVictory = __isVictory;
}

void gameOverMenu::setScore(const int& __score)
{
    mScore = __score;
}

bool gameOverMenu::checkIsVictory() const
{
    return isVictory;
}

void gameOverMenu::render(SDL_Renderer* renderer)
{   
    SDL_SetRenderDrawColor(renderer, SDL_COLOR_WHITE.r, SDL_COLOR_WHITE.g, SDL_COLOR_WHITE.b, 127);
    SDL_RenderFillRect(renderer, &mContainer);
    
    int curRenderPosY = GAME_OVER_PROMPT_RENDER_POS_Y;
    LTexture curTextTexture;
    renderText(renderer, curTextTexture, &GAME_OVER_PROMPT[isVictory][0], GAME_OVER_PROMPT_RENDER_POS_X, GAME_OVER_PROMPT_RENDER_POS_Y, GAME_OVER_PROMPT_FONT_SIZE, &CALIBRI_BOLD_FONT_PATH[0]);
    curRenderPosY += curTextTexture.getHeight() + 10;

    if (!isVictory) mYellowSad.render(renderer, GAME_OVER_PROMPT_RENDER_POS_X, curRenderPosY);
    else mYellowSunglasses.render(renderer, GAME_OVER_PROMPT_RENDER_POS_X, curRenderPosY);
    curRenderPosY += mYellowSad.getHeight() + 10;

    renderText(renderer, curTextTexture, "Anyway, your score is: ", GAME_OVER_PROMPT_RENDER_POS_X, curRenderPosY, GAME_OVER_PROMPT_FONT_SIZE, &CALIBRI_BOLD_FONT_PATH[0]);
    curRenderPosY += curTextTexture.getHeight() + 10;

    renderText(renderer, curTextTexture, &std::to_string(mScore)[0], GAME_OVER_PROMPT_RENDER_POS_X, curRenderPosY, GAME_OVER_PROMPT_FONT_SIZE_LARGE, &CALIBRI_BOLD_FONT_PATH[0]);
    curRenderPosY += curTextTexture.getHeight() + 10;

    mMainMenuButton.render(renderer);
    mReplayButton.render(renderer);
}