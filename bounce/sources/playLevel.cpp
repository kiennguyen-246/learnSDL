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

    mBall.setSize(SMALL_BALL_WIDTH, SMALL_BALL_HEIGHT);
    mBall.setSpriteClip(mSpritesheet, SMALL_BALL_SPRITE_POS_x, SMALL_BALL_SPRITE_POS_Y, SMALL_BALL_WIDTH, SMALL_BALL_HEIGHT);

    bool respawn = 1;
    int lastCheckpointIndex = 0;
    auto checkpointsList = mLevelMap.checkpointsList();
    for (int i = 0; i < checkpointsList.size(); i ++)
    {
        if (checkpointsList[i].getState() == CHECKPOINT_HIDDEN) lastCheckpointIndex = i;
    }
    mLevelMap.setFramePos(checkpointsList[lastCheckpointIndex].getFramePosX(), 
                        checkpointsList[lastCheckpointIndex].getFramePosY());

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

        mLevelMap.render(mRenderer, mSpritesheet);

        checkpointsList = mLevelMap.checkpointsList();
        
        if (respawn)
        {
            checkpointsList[lastCheckpointIndex].spawnBall(mBall);
            mLevelMap.setFramePos(checkpointsList[lastCheckpointIndex].getFramePosX(), 
                                checkpointsList[lastCheckpointIndex].getFramePosY());
            respawn = 0;
        }

        mBall.passFrame();

        if (abs(mBall.getVelocityX()) > 1e-3)
        {
            double distance = mBall.moveX();
            bool blocked = 0;
            mBall.scaleX(mLevelMap.getFramePosX());
            for (auto &curBrickTile: mLevelMap.brickTilesList())
            {
                if (collide(curBrickTile, mBall)) 
                {
                    mBall.undoMoveX();
                    mBall.scaleX(mLevelMap.getFramePosX());
                    blocked = 1;
                    mBall.setVelocityX(0);
                    // mBall.reflectX();
                }
            }
            if (!blocked) mLevelMap.moveX(distance);
        }
        
        mBall.moveY();
        mBall.scaleY();
        for (auto &curBrickTile: mLevelMap.brickTilesList())
            if (collide(curBrickTile, mBall)) 
            {
                mBall.setCollide((mBall.getVelocityY() < 0));
                mBall.undoMoveY();
                mBall.scaleY();
                mBall.reflectY();
            }
        
        for (auto &curSpike: mLevelMap.spikesList())
            if (collide(curSpike, mBall))
            {
                SDL_Rect poppedBallRenderClip = {POPPED_BALL_SPRITE_POS_x, POPPED_BALL_SPRITE_POS_Y, POPPED_BALL_WIDTH, POPPED_BALL_HEIGHT};
                mSpritesheet.render(mRenderer, mBall.getPosX(), mBall.getPosY(), &poppedBallRenderClip);
                SDL_Delay(100);
                respawn = 1;
            }
        
        for (int curIndex = 0; curIndex < checkpointsList.size(); curIndex ++)
        {
            if (collide(checkpointsList[curIndex], mBall))
            {
                switch(checkpointsList[curIndex].getState())
                {
                    case CHECKPOINT_DEFAULT:
                        checkpointsList[lastCheckpointIndex].changeState(CHECKPOINT_DELETED);
                        checkpointsList[curIndex].changeState(CHECKPOINT_COLLECTED);
                        lastCheckpointIndex = curIndex;
                        
                        break;
                    default:
                        break;
                }
            }
        }
         

        mLevelMap.updateCheckpointsList(checkpointsList);

        mBall.render(mRenderer, mSpritesheet);

        SDL_RenderPresent(mRenderer);
    }
}