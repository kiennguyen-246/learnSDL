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

void ball::setAccelerationY(const double& a)
{
    mAcceleration = a;
}

void ball::setVelocityX(const int& v)
{
    mVelocityX = v;
}

void ball::setVelocityY(const double& v)
{
    mVelocityY = v;
}

int ball::getVelocityX() const
{
    return mVelocityX;
}

int ball::getFramesPassed() const
{
    return framePassed;
}

void ball::passFrame()
{
    framePassed ++;
}

void ball::resetFramesPassed()
{
    framePassed = 0;
}

bool ball::isAirborne() const
{
    // Δx = v0 * t + a * (t^2) / 2 = (v - a * (t^2) / 2) * t
    double distance = (mVelocityY - mAcceleration * framePassed / 2) * framePassed;
    std::cout << distance << "\n";
    return abs(distance) > 20;
}

double ball::getVelocityY() const
{
    return mVelocityY;
}

void ball::moveX(const int& dir)
{
    mRealPosX = int((mRealPosX + 1) / mVelocityX) * mVelocityX;
    mRealPosX += dir;
    mRealPosX += dir * mVelocityX;
}

void ball::scaleX(const int& framePos)
{
    mPosX = mRealPosX - framePos;
}

void ball::moveY()
{
    //x(t) = x(t - 1) + v(t) - a / 2; v(t) = v(t - 1) + a
    mVelocityY += mAcceleration;
    mRealPosY += mVelocityY - mAcceleration / 2;
    scaleY();
}

void ball::undoMoveY()
{
    mRealPosY -= mVelocityY - mAcceleration / 2;
    mVelocityY -= mAcceleration;
    scaleY();
    
}

void ball::reflectY()
{
    mVelocityY = -mVelocityY * 3 / 10;
    framePassed = 0;
}

void ball::scaleY()
{
    mPosY = mRealPosY;
}

void ball::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    spritesheet.render(renderer, mPosX, mPosY - SMALL_BALL_HEIGHT, &mSpriteClip);
}