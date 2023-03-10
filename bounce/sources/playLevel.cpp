// playLevel.cpp
#include "playLevel.h"

void statusArea::init(SDL_Renderer* renderer, LTexture& spritesheet)
{
    mRenderer = renderer;
    mSpritesheet = spritesheet;
    mContainer = {STATUS_AREA_POS_X, STATUS_AREA_POS_Y, STATUS_AREA_WIDTH, STATUS_AREA_HEIGHT};
    mBallSpriteClip = {SMALL_BALL_SPRITE_POS_x, SMALL_BALL_SPRITE_POS_Y, SMALL_BALL_WIDTH / 2, SMALL_BALL_HEIGHT / 2};
}

void statusArea::renderScore(const int& score)
{
    LTexture curTextTexture;
    std::string scoreStr = std::to_string(score);
    while (scoreStr.size() < 8) scoreStr = "0" + scoreStr;
    renderText(mRenderer, curTextTexture, &scoreStr[0], SCORE_RENDER_POS_X, SCORE_RENDER_POS_Y, 
                STATUS_AREA_FONT_SIZE, &CALIBRI_FONT_PATH[0], SDL_COLOR_WHITE);
}

void statusArea::renderLivesLeft(const int& livesLeft)
{
    LTexture curTextTexture;
    std::string livesInfo = std::to_string(livesLeft) + "x";
    renderText(mRenderer, curTextTexture, &livesInfo[0], LIVES_INFO_TEXT_RENDER_POS_X, LIVES_INFO_BALL_SPRITE_RENDER_POS_Y, 
                STATUS_AREA_FONT_SIZE, &CALIBRI_FONT_PATH[0], SDL_COLOR_WHITE);
    mSpritesheet.render(mRenderer, LIVES_INFO_BALL_SPRITE_RENDER_POS_X, LIVES_INFO_BALL_SPRITE_RENDER_POS_Y, &mBallSpriteClip, 2);
}

void statusArea::render(const int& livesLeft, const int& portalsLeft, const int& score)
{
    SDL_SetRenderDrawColor(mRenderer, SDL_COLOR_BLACK.r, SDL_COLOR_BLACK.g, SDL_COLOR_BLACK.b, 255);
    SDL_RenderFillRect(mRenderer, &mContainer);

    renderLivesLeft(livesLeft);
    renderScore(score);
}

playLevel::playLevel(SDL_Window* __Window, SDL_Renderer* __Renderer, LTexture& __Spritesheet)
{
    mWindow = __Window;
    mRenderer = __Renderer;
    mSpritesheet = __Spritesheet;
}

playLevel::~playLevel()
{

}

int playLevel::getLivesLeft() const
{
    return livesLeft;
}

void playLevel::setScore(const int& __score)
{
    score = __score;
}

int playLevel::getScore() const
{
    return score;
}

void playLevel::setLevelId(const int& id)
{
    levelId = id;
    mLevelMap.setMap(id);
    livesLeft = 3;
}

void playLevel::playGame()
{
    bool quit = 0;
    SDL_Event curEvent;

    mBall.setSize(SMALL_BALL_WIDTH, SMALL_BALL_HEIGHT);
    mBall.setSpriteClip(mSpritesheet, SMALL_BALL_SPRITE_POS_x, SMALL_BALL_SPRITE_POS_Y, SMALL_BALL_WIDTH / 2, SMALL_BALL_HEIGHT / 2);

    mStatusArea.init(mRenderer, mSpritesheet);

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
            SDL_Delay(200);
            checkpointsList[lastCheckpointIndex].spawnBall(mBall);
            mLevelMap.setFramePos(checkpointsList[lastCheckpointIndex].getFramePosX(), 
                                checkpointsList[lastCheckpointIndex].getFramePosY());
            respawn = 0;
        }

        mBall.passFrame();

        //Try moving by X
        if (abs(mBall.getVelocityX()) > 1e-3)
        {
            double distance = mBall.moveX();
            bool blocked = 0;
            mBall.scaleX(mLevelMap.getFramePosX());
            for (auto &curBrickTile: mLevelMap.brickTilesList())
            {
                //Hit the wall
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
        
        //Try moving by Y
        mBall.moveY();
        mBall.scaleY();
        for (auto &curBrickTile: mLevelMap.brickTilesList())
        {
            //Hit the wall
            if (collide(curBrickTile, mBall)) 
            {
                mBall.setCollide((mBall.getVelocityY() < 0));
                mBall.undoMoveY();
                mBall.scaleY();
                mBall.reflectY();
            }
        }
    
        mBall.render(mRenderer, mSpritesheet);
        
        //Hit a spike
        for (auto &curSpike: mLevelMap.spikesList())
            if (collide(curSpike, mBall))
            {
                mBall.renderPopAnimation(mRenderer, mSpritesheet);
                livesLeft --;
                respawn = 1;
            }
        
        //Collect a checkpoint
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
                        score += 20;
                        
                        break;
                    default:
                        break;
                }
            }
        }
         
        mLevelMap.updateCheckpointsList(checkpointsList);

        //Render the status area
        mStatusArea.render(livesLeft, 0, score);


        SDL_RenderPresent(mRenderer);
    }
}