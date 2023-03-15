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
#include "spider.h"
#include "levelMap.h"

const int LIVES_INFO_TEXT_RENDER_POS_X = 60;
const int LIVES_INFO_TEXT_RENDER_POS_Y = 680;
const int LIVES_INFO_BALL_SPRITE_RENDER_POS_X = 160;
const int LIVES_INFO_BALL_SPRITE_RENDER_POS_Y = 680;
const int PORTALS_LEFT_TEXT_RENDER_POS_X = 360;
const int PORTALS_LEFT_TEXT_RENDER_POS_Y = 680;
const int PORTALS_LEFT_SPRITE_RENDER_POS_X = 480;
const int PORTALS_LEFT_SPRITE_RENDER_POS_Y = 680;
const int SCORE_RENDER_POS_X = 960;
const int SCORE_RENDER_POS_Y = 680;
const int EXTRA_RENDER_POS_X = 720;
const int EXTRA_RENDER_POS_Y = 680;
const int LEVEL_INFO_TEXT_RENDER_POS_X = 500;
const int LEVEL_INFO_TEXT_RENDER_POS_Y = 680;
const int LEVEL_INFO_TEXT_FONT_SIZE = 90;
const int LEVEL_INFO_TEXT_RENDER_TIME = 180;

/// @brief Obejects that can block the move of the ball
enum BLOCK_OBJECT
{
    NOT_BLOCKED,
    BRICK_TILE,
    TRAMPOLINE_TILE,
    SMALL_PORTAL_TILES,
    CLOSED_FINISH_LINE_TILES,
    PUMP_TILE,
    SHRINKER_TILE,
    ACCELERATOR_TILE,
    JUMP_BOOSTER_TILE,
    GRAVITY_REVERSER_TILE
};

class statusArea
{
private:
    SDL_Renderer* mRenderer;

    LTexture mSpritesheet;
    
    SDL_Rect mContainer;

    SDL_Rect mBallSpriteClip;

    SDL_Rect mPortalSpriteClip;

    SDL_Rect mAcceleratorSpriteClip;
public:
    void init(SDL_Renderer* renderer, LTexture& spritesheet);

    void renderLivesLeft(const int& livesLeft);

    void renderPortalsLeft(const int& portalsLeft);

    void renderScore(const int& score);

    void renderLevelInfo(const int& levelId);

    void render(const int& livesLeft, const int& portalsLeft, const int& score, const bool& acceleratorActivated);
};

class playLevel
{
private:
    SDL_Window* mWindow;

    SDL_Renderer* mRenderer;

    /// @brief The index of the current level
    int levelId;

    /// @brief The spritesheet of the game
    LTexture mSpritesheet;

    /// @brief The object for the ball
    ball mBall;

    /// @brief List of spiders in the level
    std::vector <spider> mSpiderList;

    /// @brief The map of the level
    levelMap mLevelMap;

    statusArea mStatusArea;

    BLOCK_OBJECT mCurBlockObjectX;
    BLOCK_OBJECT mCurBlockObjectY;

    int frameCount;

    bool yFrameChanged;

    bool acceleratorActivated;

    bool ballSpawnSize;

    int livesLeft;

    int portalsLeft;

    int score;

public:
    /// @brief Default constructor
    playLevel(SDL_Window* __Window, SDL_Renderer* __Renderer, LTexture& __Spritesheet);

    ~playLevel();

    void initSpiders(const std::vector <spiderInfo>& allLevelSpidersInfo);

    void setLivesLeft(const int& LivesLeft);

    /// @brief Set the number of lives left for the ball
    int getLivesLeft() const;

    void setScore(const int& score);

    /// @brief Set the score for the game
    int getScore() const;

    /// @brief Set up the index for the level
    void setLevelId(const int& id, const vector_2d_string& allLevelCharMap, const vector_2d_spiderInfo& allLevelSpidersInfo, const std::vector <int>& allLevelBallSpawnSize);

    /// @brief Check if the ball move is blocked by something
    BLOCK_OBJECT getBlockObject() const;

    /// @brief Try moving ball
    void tryMoveX(double& ballPosXBeforeMove, double& ballPosXAfterMove);
    void tryMoveY();

    bool checkBallIsAirborne();

    bool checkBallIsInsideWater();

    /// @brief Render the game
    /// @return 1 if the player successfully pass the level
    bool playGame();
};

#endif