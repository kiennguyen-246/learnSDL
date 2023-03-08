// ball.cpp

#include "ball.h"

ball::ball(/* args */)
{
    mAccelerationX = ACCELERATION;
}

ball::~ball()
{
}

void ball::setPosEx(const int& x, const int& y)
{
    setPos(x, y);
    mRenderPosX = mPosX;
    mRenderPosY = mPosY;
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
    mPosX += dir * mVelocityX;
}

void ball::scaleX(const int& framePos)
{
    mRenderPosX = mPosX - framePos;
}

void ball::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    std::cout << mRenderPosX << " " << mRenderPosY << "\n";
    spritesheet.render(renderer, mRenderPosX, mRenderPosY - SMALL_BALL_HEIGHT, &mSpriteClip);
}