// playLevel.h

/**
 * @brief This file controls a single level of the game
 * 
 */

#ifndef PLAYLEVEL_GUARD
#define PLAYLEVEL_GUARD

#include <iostream>
#include <string>
#include <windows.h>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "base.h"
#include "ball.h"
#include "levelMap.h"

const int LIVES_INFO_TEXT_RENDER_POS_X = 60;
const int LIVES_INFO_TEXT_RENDER_POS_Y = 680;
const int LIVES_INFO_BALL_SPRITE_RENDER_POS_X = 160;
const int LIVES_INFO_BALL_SPRITE_RENDER_POS_Y = 680;
const int SCORE_RENDER_POS_X = 800;
const int SCORE_RENDER_POS_Y = 680;

class playLevel
{
private:
    SDL_Window* mWindow;

    SDL_Renderer* mRenderer;

    /// @brief The index of the current level
    int levelId;
    
    /// @brief The object for the ball
    ball mBall;

    /// @brief The spritesheet of the game
    LTexture mSpritesheet;

    /// @brief The map of the level
    levelMap mLevelMap;

    /// @brief Set the number of lives left for the ball
    int livesLeft;

public:
    /// @brief Default constructor
    playLevel(SDL_Window* __Window, SDL_Renderer* __Renderer, LTexture& __Spritesheet);

    ~playLevel();

    int getLivesLeft() const;

    /// @brief Set up the index for the level
    void setLevelId(const int& id);

    /// @brief Render the game
    void playGame();
};

#endif