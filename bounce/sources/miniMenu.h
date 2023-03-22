// miniMenu.h

#ifndef MINIMENU_GUARD
#define MINIMENU_GUARD

#include <iostream>
#include <string>
#include <windows.h>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "base.h"
#include "buttons.h"

const int MINI_MENU_RENDER_POS_X = 160;
const int MINI_MENU_RENDER_POS_Y = 40;
const int MINI_MENU_WIDTH = 960;
const int MINI_MENU_HEIGHT = 560;

const std::string GAME_OVER_PROMPT[] = 
{
    "Unfortunately, you have no lives left. :(",
    "Astounding! You cleared all the levels! ^_^"
};

enum GAME_OVER_MENU_EXIT_STATUS
{
    GAME_OVER_EXIT_NULL,
    GAME_OVER_EXIT_REPLAY,
    GAME_OVER_EXIT_MAIN_MENU,
};
const int GAME_OVER_PROMPT_RENDER_POS_X = -1e6;
const int GAME_OVER_PROMPT_RENDER_POS_Y = 70;
const int GAME_OVER_PROMPT_FONT_SIZE = 36;
const int GAME_OVER_PROMPT_FONT_SIZE_LARGE = 48;
const std::string YELLOW_SAD_EMOJI_PATH = "./img/yellowSad.png";
const std::string YELLOW_SUNGLASSES_EMOJI_PATH = "./img/yellowSunglasses.png";
const std::string MAIN_MENU_BUTTON_TEXT = "MAIN MENU";
const int MAIN_MENU_BUTTON_RENDER_POS_X = 317;
const int MAIN_MENU_BUTTON_RENDER_POS_Y = 480;
const std::string REPLAY_BUTTON_TEXT = "REPLAY LEVEL";
const int REPLAY_BUTTON_RENDER_POS_X = 740;
const int REPLAY_BUTTON_RENDER_POS_Y = 480;

class miniMenu
{
protected:
    SDL_Rect mContainer;
public:
    miniMenu(/* args */);
    ~miniMenu();

    virtual void render(SDL_Renderer* renderer) = 0;
};

class gameOverMenu: public miniMenu
{
private:
    bool isVictory;

    LTexture mYellowSad;
    LTexture mYellowSunglasses;

    int mScore;

    redButton mMainMenuButton, mReplayButton;
public:
    gameOverMenu();
    ~gameOverMenu();

    bool checkIsVictory() const;

    void set(SDL_Renderer* renderer);

    void handleEvent(SDL_Event* event, GAME_OVER_MENU_EXIT_STATUS& exitStatus);

    void setVictory(const bool& __isVivtory);

    void setScore(const int& __score);

    void render(SDL_Renderer* renderer);
};


#endif