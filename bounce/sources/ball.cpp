// ball.cpp

#include "ball.h"

ball::ball(/* args */)
{

}

ball::~ball()
{
}

void ball::setPosEx(const int& x, const int& y, const int& framePosX, const int& framePosY)
{
    mRealPosX = x;
    mRealPosY = y;
    mPosX = mRealPosX;
    mPosY = mRealPosY;
}

void ball::setAccelerationX(const double& a)
{
    mAccelerationX = a;
}

void ball::setAccelerationY(const double& a)
{
    mAccelerationY = a;
}

void ball::setVelocityX(const int& v)
{
    mVelocityX = v;
}

void ball::setVelocityY(const double& v)
{
    mVelocityY = v;
}

double ball::getVelocityX() const
{
    return mVelocityX;
}

double ball::getVelocityY() const
{
    return mVelocityY;
}

int ball::getFramesPassedX() const
{
    return framePassedX;
}

int ball::getFramesPassedY() const
{
    return framePassedY;
}

void ball::passFrame()
{
    framePassedX ++;
    framePassedY ++;
}

void ball::resetFramesPassedX()
{
    framePassedX = 0;
}

void ball::resetFramesPassedY()
{
    framePassedY = 0;
}

bool ball::isAirborne() const
{
    // Δx = v0 * t + a * (t^2) / 2 = (v - a * (t^2) / 2) * t
    double distance = (mVelocityY - mAccelerationY * framePassedY / 2) * framePassedY;
    return (__lastCollideY == 1 || abs(distance) > 1);
}

void ball::setCollide(const int& collideType)
{
    __lastCollideY = collideType;
}

bool ball::lastCollideY() const
{
    return __lastCollideY;
}

double ball::moveX()
{
    //x(t) = x(t - 1) + v(t) - a / 2; v(t) = v(t - 1) + a
    mVelocityX += mAccelerationX;
    mRealPosX += mVelocityX - mAccelerationX / 2;
    return mVelocityX - mAccelerationX / 2;
}

void ball::undoMoveX()
{
    mRealPosX -= mVelocityX - mAccelerationX / 2;
    mVelocityX -= mAccelerationX;
}

void ball::reflectX()
{
    mVelocityX = -mVelocityX * 5 / 10;
    framePassedX = 0;
}

void ball::scaleX(const int& framePos)
{
    mPosX = mRealPosX - framePos;
    // std::cout << mPosX << " ";
}

void ball::moveY()
{
    //y(t) = y(t - 1) + v(t) - a / 2; v(t) = v(t - 1) + a
    mVelocityY += mAccelerationY;
    mRealPosY += mVelocityY - mAccelerationY / 2;
}

void ball::undoMoveY()
{
    mRealPosY -= mVelocityY - mAccelerationY / 2;
    mVelocityY -= mAccelerationY;
}

void ball::reflectY()
{
    mVelocityY = -mVelocityY * BALL_BOUNCE_LEVEL / 100;
    framePassedY = 0;
}

void ball::scaleY()
{
    mPosY = mRealPosY;
}

void ball::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    spritesheet.render(renderer, mPosX, mPosY - SMALL_BALL_HEIGHT, &mSpriteClip);
}