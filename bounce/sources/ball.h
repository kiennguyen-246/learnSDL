// ball.h

/**
 * @brief This file controls the balls types of the game
 * 
 */

#ifndef BALL_GUARD
#define BALL_GUARD

#include <iostream>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "base.h"

const int SMALL_BALL_SPRITE_POS_x = 198;   //position in spritesheet
const int SMALL_BALL_SPRITE_POS_Y = 0;
const int SMALL_BALL_WIDTH = 80;
const int SMALL_BALL_HEIGHT = 80;
const int BALL_VELOCITY_X_DEFAULT = 10;

class ball: public gameObject
{
private:
    /// @brief The velocity of the ball, by X and Y
    int mVelocityX, mVelocityY;

    /// @brief The real position of the ball (in comparison with the game)
    int mRealPosX, mRealPosY;

    /// @brief The acceleration of the ball by X. Assumed that aY = 0
    int mAccelerationX;

public:
    ball(/* args */);
    ~ball();

    /// @brief Set the position for the ball with some more additional settings
    void setPosEx(const int& x, const int& y, const int& framePosX, const int& frameposY);

    void setVelocityX(const int& v);

    void setVelocityY(const int& v);

    int getVelocityX() const;

    /// @brief Move the ball horizontally
    /// @param dir Direction of the ball. 1 for positive direction (forward), -1 otherwise (backward)
    void moveX(const int& dir);

    /// @brief Scale the ball to fit in the screen
    void scaleX(const int& framePosX);

    /// @brief Move the ball vertically
    // void moveY();

    void render(SDL_Renderer* renderer, LTexture& spritesheet);
};

#endif