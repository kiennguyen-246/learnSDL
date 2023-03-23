// miniMenu.cpp

#include "miniMenu.h"

int getHighScore()
{
    std::ifstream fi(HIGH_SCORE_DATA_PATH);
    if (!fi.is_open())
    {
        std::cout << "Cannot open high score data file.\n";
    }
    int ret = 0;
    fi >> ret;
    fi.close();
    return ret;
}

void reloadHighScore(const int& newScore)
{
    std::ofstream fo(HIGH_SCORE_DATA_PATH);
    if (!fo.is_open())
    {
        std::cout << "Cannot open high score data file.\n";
    }
    fo << newScore;
    fo.close();
}

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
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) buttonTriggered = 1;
    if (buttonTriggered) exitStatus = GAME_OVER_EXIT_MAIN_MENU;

    buttonTriggered = 0;
    mReplayButton.handleEvent(event, buttonTriggered);
    if (buttonTriggered) exitStatus = GAME_OVER_EXIT_REPLAY;
}

void gameOverMenu::setVictory(const bool& __isVictory)
{
    isVictory = __isVictory;
}

bool gameOverMenu::checkIsVictory() const
{
    return isVictory;
}

void gameOverMenu::setScore(const int& __score)
{
    mScore = __score;
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

    int oldHighScore = getHighScore();
    if (mScore >= oldHighScore)
    {
        renderText(renderer, curTextTexture, "NEW HIGH SCORE!", GAME_OVER_PROMPT_RENDER_POS_X, curRenderPosY, GAME_OVER_PROMPT_FONT_SIZE, &CALIBRI_BOLD_FONT_PATH[0]);
        curRenderPosY += curTextTexture.getHeight() + 10;
        reloadHighScore(mScore);

    }

    mMainMenuButton.render(renderer);
    mReplayButton.render(renderer);
}

highScoreMenu::highScoreMenu()
{

}

highScoreMenu::~highScoreMenu()
{

}

void highScoreMenu::set(SDL_Renderer* renderer)
{
    mReturnButton.set(renderer, RETURN_BUTTON_RENDER_POS_X, RETURN_BUTTON_RENDER_POS_Y, RETURN_BUTTON_TEXT);

    mYellowSunglasses.loadTexture(renderer, &YELLOW_SUNGLASSES_EMOJI_PATH[0]);
}

void highScoreMenu::handleEvent(SDL_Event* event, HIGH_SCORE_MENU_EXIT_STATUS& exitStatus)
{
    bool buttonTriggered = 0;
    mReturnButton.handleEvent(event, buttonTriggered);
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) buttonTriggered = 1;
    if (buttonTriggered) 
    {
        exitStatus = HIGH_SCORE_EXIT_RETURN;
    }
}

void highScoreMenu::render(SDL_Renderer* renderer)
{   
    SDL_SetRenderDrawColor(renderer, SDL_COLOR_WHITE.r, SDL_COLOR_WHITE.g, SDL_COLOR_WHITE.b, 127);
    SDL_RenderFillRect(renderer, &mContainer);
    
    int curRenderPosY = HIGH_SCORE_PROMPT_RENDER_POS_Y;
    LTexture curTextTexture;
    renderText(renderer, curTextTexture, "High score: ", HIGH_SCORE_PROMPT_RENDER_POS_X, curRenderPosY, HIGH_SCORE_PROMPT_FONT_SIZE, &CALIBRI_BOLD_FONT_PATH[0]);
    curRenderPosY += curTextTexture.getHeight() + 10;

    renderText(renderer, curTextTexture, &std::to_string(getHighScore())[0], HIGH_SCORE_PROMPT_RENDER_POS_X, curRenderPosY, HIGH_SCORE_PROMPT_FONT_SIZE_LARGE, &CALIBRI_BOLD_FONT_PATH[0]);
    curRenderPosY += curTextTexture.getHeight() + 10;

    mYellowSunglasses.render(renderer, HIGH_SCORE_PROMPT_RENDER_POS_X, curRenderPosY);
    curRenderPosY += mYellowSunglasses.getHeight() + 10;

    mReturnButton.render(renderer);
}

pauseMenu::pauseMenu()
{

}

pauseMenu::~pauseMenu()
{

}

void pauseMenu::set(SDL_Renderer* renderer)
{
    mResumeButton.set(renderer, RESUME_BUTTON_RENDER_POS_X, RESUME_BUTTON_RENDER_POS_Y, RESUME_BUTTON_TEXT);
    mMainMenuButton.set(renderer, MAIN_MENU_2_BUTTON_RENDER_POS_X, MAIN_MENU_2_BUTTON_RENDER_POS_Y, MAIN_MENU_2_BUTTON_TEXT);

    mYellowNerd.loadTexture(renderer, &YELLOW_NERD_EMOJI_PATH[0]);
}

void pauseMenu::handleEvent(SDL_Event* event, PAUSE_MENU_EXIT_STATUS& exitStatus)
{
    bool buttonTriggered = 0;
    mResumeButton.handleEvent(event, buttonTriggered);
    if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) buttonTriggered = 1;
    if (buttonTriggered) 
    {
        exitStatus = PAUSE_EXIT_RESUME;
    }

    buttonTriggered = 0;
    mMainMenuButton.handleEvent(event, buttonTriggered);
    if (buttonTriggered) 
    {
        exitStatus = PAUSE_EXIT_MAIN_MENU;
    }
}

void pauseMenu::render(SDL_Renderer* renderer)
{   
    SDL_SetRenderDrawColor(renderer, SDL_COLOR_WHITE.r, SDL_COLOR_WHITE.g, SDL_COLOR_WHITE.b, 127);
    SDL_RenderFillRect(renderer, &mContainer);
    
    int curRenderPosY = PAUSE_PROMPT_RENDER_POS_Y;
    LTexture curTextTexture;
    renderText(renderer, curTextTexture, "GAME PAUSED", PAUSE_PROMPT_RENDER_POS_X, curRenderPosY, PAUSE_PROMPT_FONT_SIZE, &CALIBRI_BOLD_FONT_PATH[0]);
    curRenderPosY += curTextTexture.getHeight() + 10;

    mYellowNerd.render(renderer, PAUSE_PROMPT_RENDER_POS_X, curRenderPosY);
    curRenderPosY += mYellowNerd.getHeight() + 10;

    mResumeButton.render(renderer);
    mMainMenuButton.render(renderer);
}