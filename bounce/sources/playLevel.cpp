// playLevel.cpp
#include "playLevel.h"

playLevel::playLevel(SDL_Window* __Window, SDL_Renderer* __Renderer, LTexture& __Spritesheet)
{
    mWindow = __Window;
    mRenderer = __Renderer;
    mSpritesheet = __Spritesheet;
}

playLevel::~playLevel()
{

}

void playLevel::setLevelId(const int& id)
{
    levelId = id;
    mLevelMap.setMap(id);
}

void playLevel::playGame()
{
    bool quit = 0;
    SDL_Event curEvent;

    // int moveFramesLeftX = 0;

    mBall.setPosEx(160, 560, 0, 0);
    mBall.setSize(SMALL_BALL_WIDTH, SMALL_BALL_HEIGHT);
    mBall.setSpriteClip(mSpritesheet, SMALL_BALL_SPRITE_POS_x, SMALL_BALL_SPRITE_POS_Y, SMALL_BALL_WIDTH, SMALL_BALL_HEIGHT);

    while (!quit)
    {
        while (SDL_PollEvent(&curEvent) != 0)
        {
            if (curEvent.type == SDL_QUIT) quit = 1;
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) 
        {
            mBall.resetFramesPassedX();
            mBall.setVelocityX(-BALL_VELOCITY_X_DEFAULT);
            mBall.setAccelerationX(-BALL_ACCELERATION_X_DEFAULT);
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) 
        {
            mBall.resetFramesPassedX();
            mBall.setVelocityX(BALL_VELOCITY_X_DEFAULT);
            mBall.setAccelerationX(BALL_ACCELERATION_X_DEFAULT);
        }
        if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_SPACE])
        {
            if (!mBall.isAirborne())
            {
                mBall.resetFramesPassedY();
                mBall.setVelocityY(-BALL_VELOCITY_Y_DEFAULT);
                mBall.setAccelerationY(BALL_ACCELERATION_Y_DEFAULT);
            }
        }

        SDL_SetRenderDrawColor(mRenderer, SDL_COLOR_MALIBU.r, SDL_COLOR_MALIBU.g, SDL_COLOR_MALIBU.b, 255);
        SDL_RenderClear(mRenderer);

        mLevelMap.clearBrickTilesList();
        mLevelMap.render(mRenderer, mSpritesheet);

        mBall.passFrame();

        if (abs(mBall.getVelocityX()) > 1e-3)
        {
            double distance = mBall.moveX();
            bool blocked = 0;
            mBall.scaleX(mLevelMap.getFramePosX());
            for (auto curBrickTile: mLevelMap.brickTilesList())
            {
                if (collide(curBrickTile, mBall)) 
                {
                    mBall.undoMoveX();
                    mBall.scaleX(mLevelMap.getFramePosX());
                    blocked = 1;
                    // mBall.reflectX();
                }
            }
            if (!blocked) mLevelMap.moveX(distance);
        }
        
        
        mBall.moveY();
        mBall.scaleY();
        for (auto curBrickTile: mLevelMap.brickTilesList())
            if (collide(curBrickTile, mBall)) 
            {
                mBall.setCollide((mBall.getVelocityY() < 0));
                mBall.undoMoveY();
                mBall.scaleY();
                mBall.reflectY();
            }

        mBall.render(mRenderer, mSpritesheet);

        SDL_RenderPresent(mRenderer);
    }
}