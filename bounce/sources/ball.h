// ball.h

/**
 * @brief This file controls the balls types of the game
 * Moving logic:
 * - After the control key is pressed, the ball moves horizontally by 20 pixels
 * - After the control key is pressed, the ball moves vertically with an accelerator of 2/65 pixel/(frames)^2
 *   Each time a key is pressed, the ball starts with a velocity of 4 pixel/frames
 *   The moving equation is Y = Y0 - 12 * t + (1/10) * (t^2)
 * - While moving vertically, if the ball collides with any kind of bricks, it moves in the opposite direction
 *   with the velocity being decreased by 30%
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
const int BALL_VELOCITY_X_DEFAULT = 20;     //unit: pixel/frame
const double BALL_VELOCITY_Y_DEFAULT = 12;     //unit: pixel/frame
const double BALL_ACCELERATION_Y_DEFAULT = 1.0 / 5;     //unit: pixel/(frame)^2

class ball: public gameObject
{
private:
    /// @brief The velocity of the ball, by X and Y
    int mVelocityX;
    double mVelocityY;

    /// @brief The real position of the ball (in comparison with the game)
    int mRealPosX;
    double mRealPosY;

    /// @brief The acceleration of the ball
    double mAcceleration;

    /// @brief Number of frames has passed since start moving upward
    int framePassed;

    /// @brief Check if the ball is moving vertically
    bool movingY;

public:
    ball(/* args */);
    ~ball();

    /// @brief Set the position for the ball with some more additional settings
    void setPosEx(const int& x, const int& y, const int& framePosX, const int& frameposY);

    void setAccelerationY(const double& a);

    void setVelocityX(const int& v);

    void setVelocityY(const double& v);

    int getVelocityX() const;

    /// @brief Get the number of frames passed since move starts
    int getFramesPassed() const;

    /// @brief Pass 1 frame
    void passFrame();

    /// @brief Reset the number of frames passed since move starts to 0
    void resetFramesPassed();

    /// @brief Check if the ball is moving vertically
    bool isMovingY() const;

    /// @brief Get the current velocity of the ball
    double getVelocityY() const;

    /// @brief Move the ball horizontally. 
    /// @param dir Direction of the ball. 1 for positive direction (forward), -1 otherwise (backward)
    void moveX(const int& dir);

    /// @brief Scale the ball to be rendered in the screen
    void scaleX(const int& framePosX);

    /// @brief Move the ball vertically
    void moveY();

    /// @brief Scale the ball to be rendered in the screen
    void scaleY();

    void render(SDL_Renderer* renderer, LTexture& spritesheet);
};

#endif