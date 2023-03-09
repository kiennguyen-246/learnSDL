// ball.h

/**
 * @brief This file controls the balls types of the game
 * Moving logic:
 * - After the control key is pressed, the ball moves horizontally with an accelerator of -5/64 pixel/(frames)^2
 *   Each time a key is pressed, the ball starts with a velocity of 5 pixel/frames
 *   The moving equation is X = X0 - 5 * t - (5/128) * (t^2)  (pixel)
 * 
 * - After the control key is pressed, the ball moves vertically with an accelerator of 1/5 pixel/(frames)^2
 *   Each time a key is pressed, the ball starts with a velocity of 12 pixel/frames
 *   The moving equation is Y = Y0 - 12 * t + (1/10) * (t^2)  (pixel)
 * 
 * - While moving, if the ball collides with any kind of bricks, it moves in the opposite direction
 *   with the velocity being decreased by 70%
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
const double BALL_VELOCITY_X_DEFAULT = 5;     //unit: pixel/frame
const double BALL_VELOCITY_Y_DEFAULT = 12;     //unit: pixel/frame
const double BALL_ACCELERATION_X_DEFAULT = -5.0 / 64;     //unit: pixel/(frame)^2
const double BALL_ACCELERATION_Y_DEFAULT = 1.0 / 5;     //unit: pixel/(frame)^2
const int BALL_BOUNCE_LEVEL = 50;      //new velocity multiplier after a bounce (percent)
// const int BALL_SINGLE_MOVE_FRAME_COUNT_X_DEFAULT = 999999;  

class ball: public gameObject
{
private:
    /// @brief The velocity of the ball, by X and Y
    double mVelocityX;
    double mVelocityY;

    /// @brief The real position of the ball (in comparison with the game)
    double mRealPosX;
    double mRealPosY;

    /// @brief The acceleration of the ball
    double mAccelerationX;
    double mAccelerationY;

    /// @brief Number of frames has passed since start moving upward
    int framePassedX;
    int framePassedY;

    /// @brief Check if the ball is moving vertically
    bool __isAirborne;

    /// @brief Check if the last collide was the ground(0) or the ceiling(1)
    bool __lastCollideY;

public:
    ball(/* args */);
    ~ball();

    /// @brief Set the position for the ball with some more additional settings
    void setPosEx(const int& x, const int& y, const int& framePosX, const int& frameposY);

    void setAccelerationX(const double& a);

    void setAccelerationY(const double& a);

    void setVelocityX(const int& v);

    void setVelocityY(const double& v);

    /// @brief Get the current velocity of the ball
    double getVelocityX() const;

    /// @brief Get the current velocity of the ball
    double getVelocityY() const;

    /// @brief Get the number of frames passed since move starts
    int getFramesPassedX() const;

    /// @brief Get the number of frames passed since move starts
    int getFramesPassedY() const;

    /// @brief Pass 1 frame
    void passFrame();

    /// @brief Reset the number of frames passed since move starts to 0
    void resetFramesPassedX();

    /// @brief Reset the number of frames passed since move starts to 0
    void resetFramesPassedY();

    /// @brief Check if the ball is moving vertically
    bool isAirborne() const;

    /// @brief Set the type of the last collide time, ground(0) or ceiling(1)
    void setCollide(const int& collideType);

    /// @brief Check if the last collide was the ground(0) or the ceiling(1)
    bool lastCollideY() const;

    /// @brief Move the ball horizontally
    /// @return The moved distance
    double moveX();

    /// @brief Undo move horizontally
    void undoMoveX();

    /// @brief Reflect the ball when hitting another block
    void reflectX();

    /// @brief Scale the ball to be rendered in the screen
    void scaleX(const int& framePosX);

    /// @brief Move the ball vertically
    void moveY();

    /// @brief Undo move vertically
    void undoMoveY();

    /// @brief Reflect the ball when hitting another block
    void reflectY();

    /// @brief Scale the ball to be rendered in the screen
    void scaleY();

    void render(SDL_Renderer* renderer, LTexture& spritesheet);
};

#endif