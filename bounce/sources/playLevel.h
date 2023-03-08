// playLevel.h

/**
 * @brief This file controls a single level of the game
 * 
 */

#ifndef PLAYLEVEL_GUARD
#define PLAYLEVEL_GUARD

#include <iostream>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "base.h"
#include "ball.h"
#include "levelMap.h"



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

public:
    /// @brief Default constructor
    playLevel(SDL_Window* __Window, SDL_Renderer* __Renderer, LTexture& __Spritesheet);

    ~playLevel();

    /// @brief Set up the index for the level
    void setLevelId(const int& id);

    /// @brief Render the game
    void playGame();
};

#endif