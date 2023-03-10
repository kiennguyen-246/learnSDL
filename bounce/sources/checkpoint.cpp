// checkpoint.cpp
#include "checkpoint.h"

checkpoint::checkpoint(/* args */)
{
    mState = CHECKPOINT_DEFAULT;
    setPos(-100, -100);
    setSize(CHECKPOINT_WIDTH, CHECKPOINT_HEIGHT);
}

checkpoint::~checkpoint()
{
}

void checkpoint::init(const int& __charmapPosX, const int& __charmapPosY, const CHECKPOINT_STATE& __state)
{
    mState = __state;
    charmapPosX = __charmapPosX;
    charmapPosY = __charmapPosY;
    framePosX = (charmapPosX - 7) * CHECKPOINT_WIDTH;
    framePosY = 0;
}

int checkpoint::getCharmapPosX() const
{
    return charmapPosX;
}

int checkpoint::getCharmapPosY() const
{
    return charmapPosY;
}

double checkpoint::getFramePosX() const
{
    return framePosX;
}

double checkpoint::getFramePosY() const
{
    return framePosY;
}

void checkpoint::spawnBall(ball& __ball)
{
    __ball.setPosEx(charmapPosX * CHECKPOINT_WIDTH, (charmapPosY + 1) * CHECKPOINT_HEIGHT, framePosX, framePosY);
    // std::cout << mPosX + framePosX << " " << mPosY + framePosY << "\n";
    __ball.setVelocityX(0);
    __ball.setVelocityY(CHECKPOINT_DROP_VELOCITY);
    __ball.setAccelerationY(BALL_ACCELERATION_Y_DEFAULT);
}

void checkpoint::changeState(const CHECKPOINT_STATE& newState)
{
    mState = newState;
}

CHECKPOINT_STATE checkpoint::getState() const
{
    return mState;
}

void checkpoint::resetMapFrame(double& __framePosX, double& __framePosY)
{
    __framePosX = framePosX;
    __framePosY = framePosY;
}

void checkpoint::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    if (mState == CHECKPOINT_DEFAULT)
    {
        setSpriteClip(spritesheet, CHECKPOINT_DEFAULT_SPRITE_POS_x, CHECKPOINT_DEFAULT_SPRITE_POS_Y, CHECKPOINT_WIDTH, CHECKPOINT_HEIGHT);
    } 
    else if (mState == CHECKPOINT_COLLECTED)
    {
        setSpriteClip(spritesheet, CHECKPOINT_COLLECTED_SPRITE_POS_x, CHECKPOINT_COLLECTED_SPRITE_POS_Y, CHECKPOINT_WIDTH, CHECKPOINT_HEIGHT);
    } 
    else return;
    spritesheet.render(renderer, mPosX, mPosY - CHECKPOINT_HEIGHT, &mSpriteClip);
}