// ball.cpp

#include "ball.h"

ball::ball(/* args */)
{
    mAccelerationX = ACCELERATION;
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

void ball::setVelocityX(const int& v)
{
    mVelocityX = v;
}

void ball::setVelocityY(const int& v)
{
    mVelocityY = v;
}

int ball::getVelocityX() const
{
    return mVelocityX;
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

void ball::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    // std::cout << mPosX << " " << mPosY << "\n";
    spritesheet.render(renderer, mPosX, mPosY - SMALL_BALL_HEIGHT, &mSpriteClip);
}