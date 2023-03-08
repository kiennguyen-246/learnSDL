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

    mBall.setPosEx(160, 560);
    mBall.setVelocityX(BALL_VELOCITY_X_DEFAULT);
    mBall.setSpriteClip(mSpritesheet, SMALL_BALL_SPRITE_POS_x, SMALL_BALL_SPRITE_POS_Y, SMALL_BALL_WIDTH, SMALL_BALL_HEIGHT);

    while (!quit)
    {
        while (SDL_PollEvent(&curEvent) != 0)
        {
            if (curEvent.type == SDL_QUIT) quit = 1;
            const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

            if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) 
            {
                mLevelMap.moveX(-1, mBall.getVelocityX());
                mBall.moveX(-1);
                mBall.scaleX(mLevelMap.getFramePosX());
            
            }
            else if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) 
            {
                mLevelMap.moveX(1, mBall.getVelocityX());
                mBall.moveX(1);
                mBall.scaleX(mLevelMap.getFramePosX());
            }

            SDL_SetRenderDrawColor(mRenderer, 81, 218, 254, 255);
            SDL_RenderClear(mRenderer);

            mLevelMap.render(mRenderer, mSpritesheet);
            mBall.render(mRenderer, mSpritesheet);

            SDL_RenderPresent(mRenderer);
        }
    }
}