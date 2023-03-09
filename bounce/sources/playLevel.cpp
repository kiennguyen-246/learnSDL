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

    mBall.setPosEx(160, 560, 0, 0);
    mBall.setSize(SMALL_BALL_WIDTH, SMALL_BALL_HEIGHT);
    mBall.setVelocityX(BALL_VELOCITY_X_DEFAULT);
    mBall.setAccelerationY(BALL_ACCELERATION_Y_DEFAULT);
    mBall.setSpriteClip(mSpritesheet, SMALL_BALL_SPRITE_POS_x, SMALL_BALL_SPRITE_POS_Y, SMALL_BALL_WIDTH, SMALL_BALL_HEIGHT);

    while (!quit)
    {
        while (SDL_PollEvent(&curEvent) != 0)
        {
            if (curEvent.type == SDL_QUIT) quit = 1;
            const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

            if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) 
            {
                // Check if the ball touch a brick
                bool blocked = 0;
                for (auto &curBrick: mLevelMap.brickTilesList())
                {
                    if (collide(curBrick, mBall) && curBrick.getPosX() < mBall.getPosX()) blocked = 1;
                }
                if (!blocked)
                {
                    mLevelMap.moveX(-1, mBall.getVelocityX());
                    mBall.moveX(-1);
                    mBall.scaleX(mLevelMap.getFramePosX());
                }
            }
            if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) 
            {
                // Check if the ball touch a brick
                bool blocked = 0;
                for (auto &curBrick: mLevelMap.brickTilesList())
                {
                    if (collide(curBrick, mBall) && curBrick.getPosX() > mBall.getPosX()) blocked = 1;
                }
                if (!blocked)
                {
                    mLevelMap.moveX(1, mBall.getVelocityX());
                    mBall.moveX(1);
                    mBall.scaleX(mLevelMap.getFramePosX());
                }
            }
            if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_SPACE])
            {
                if (!mBall.isAirborne())
                {
                    mBall.resetFramesPassed();
                    mBall.setVelocityY(-BALL_VELOCITY_Y_DEFAULT);
                }
                
            }

            // for (int i = 0; i < 20; i ++) mBall.moveX(1);
            // mBall.scaleX(0);

            //  std::cout << mBall.getPosX() << " " << mBall.getPosY() << "\n";
        }

        SDL_SetRenderDrawColor(mRenderer, SDL_COLOR_MALIBU.r, SDL_COLOR_MALIBU.g, SDL_COLOR_MALIBU.b, 255);
        SDL_RenderClear(mRenderer);

        mLevelMap.clearBrickTilesList();
        mLevelMap.render(mRenderer, mSpritesheet);

        mBall.passFrame();
        mBall.moveY();
        for (auto curBrickTile: mLevelMap.brickTilesList())
            if (collide(curBrickTile, mBall)) 
            {
                mBall.undoMoveY();
                mBall.reflectY();
            }

        mBall.render(mRenderer, mSpritesheet);

        SDL_RenderPresent(mRenderer);
    }
}