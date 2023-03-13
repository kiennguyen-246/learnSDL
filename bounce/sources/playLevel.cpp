// playLevel.cpp
#include "playLevel.h"

void statusArea::init(SDL_Renderer* renderer, LTexture& spritesheet)
{
    mRenderer = renderer;
    mSpritesheet = spritesheet;
    mContainer = {STATUS_AREA_POS_X, STATUS_AREA_POS_Y, STATUS_AREA_WIDTH, STATUS_AREA_HEIGHT};
    mBallSpriteClip = {SMALL_BALL_SPRITE_POS_x, SMALL_BALL_SPRITE_POS_Y, SMALL_BALL_WIDTH / 2, SMALL_BALL_HEIGHT / 2};
    mPortalSpriteClip = {PORTAL_HORIZONTAL_DEFAULT_SPRITE_POS_x, PORTAL_HORIZONTAL_COLLECTED_SPRITE_POS_Y,
                         PORTAL_HORIZONTAL_WIDTH / 2, PORTAL_HORIZONTAL_HEIGHT / 2};
}

void statusArea::renderScore(const int& score)
{
    LTexture curTextTexture;
    std::string scoreStr = std::to_string(score);
    while (scoreStr.size() < 8) scoreStr = "0" + scoreStr;
    renderText(mRenderer, curTextTexture, &scoreStr[0], SCORE_RENDER_POS_X, SCORE_RENDER_POS_Y, 
                STATUS_AREA_FONT_SIZE, &CALIBRI_FONT_PATH[0], SDL_COLOR_WHITE);
}

void statusArea::renderPortalsLeft(const int& portalsLeft)
{
    LTexture curTextTexture;
    std::string portalsLeftInfo = std::to_string(portalsLeft) + "x";
    renderText(mRenderer, curTextTexture, &portalsLeftInfo[0], PORTALS_LEFT_TEXT_RENDER_POS_X, PORTALS_LEFT_TEXT_RENDER_POS_Y, 
                STATUS_AREA_FONT_SIZE, &CALIBRI_FONT_PATH[0], SDL_COLOR_WHITE);
    mSpritesheet.render(mRenderer, PORTALS_LEFT_SPRITE_RENDER_POS_X, PORTALS_LEFT_SPRITE_RENDER_POS_Y, &mPortalSpriteClip, 2);
}

void statusArea::renderLivesLeft(const int& livesLeft)
{
    LTexture curTextTexture;
    std::string livesInfo = std::to_string(livesLeft) + "x";
    renderText(mRenderer, curTextTexture, &livesInfo[0], LIVES_INFO_TEXT_RENDER_POS_X, LIVES_INFO_TEXT_RENDER_POS_Y, 
                STATUS_AREA_FONT_SIZE, &CALIBRI_FONT_PATH[0], SDL_COLOR_WHITE);
    mSpritesheet.render(mRenderer, LIVES_INFO_BALL_SPRITE_RENDER_POS_X, LIVES_INFO_BALL_SPRITE_RENDER_POS_Y, &mBallSpriteClip, 2);
}

void statusArea::render(const int& livesLeft, const int& portalsLeft, const int& score)
{
    SDL_SetRenderDrawColor(mRenderer, SDL_COLOR_BLACK.r, SDL_COLOR_BLACK.g, SDL_COLOR_BLACK.b, 255);
    SDL_RenderFillRect(mRenderer, &mContainer);

    renderLivesLeft(livesLeft);
    renderPortalsLeft(portalsLeft);
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

void playLevel::setLivesLeft(const int& __livesLeft)
{
    livesLeft = __livesLeft;
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

void playLevel::setLevelId(const int& id, const vector_2d_string& allLevelCharMap, const vector_2d_pair_of_pii& allLevelSpidersInfo, const std::vector <int>& allLevelBallSpawnSize)
{
    levelId = id;
    mLevelMap.setMap(id, allLevelCharMap, allLevelSpidersInfo);
    ballSpawnSize = allLevelBallSpawnSize[id];
    std::cout << "Starting ball spawn size: " << ballSpawnSize << "\n";
}

BLOCK_OBJECT playLevel::getBlockObject() const
{
    auto brickTilesList = mLevelMap.brickTilesList();
    auto portalsList = mLevelMap.portalsList();
    auto curFinishLine = mLevelMap.getFinishLine();
    auto pumpsList = mLevelMap.pumpsList();
    auto shrinkersList = mLevelMap.shrinkersList();
    
    BLOCK_OBJECT blockObject = NOT_BLOCKED;
    for (auto &curBrickTile: brickTilesList)
        if (collide(curBrickTile, mBall)) blockObject = BRICK_TILE;

    for (auto &curPump: pumpsList)
    {
        if (collide(curPump, mBall))
        {
            blockObject = PUMP_TILE;
        } 
    }

    for (auto &curShrinker: shrinkersList)
    {
        if (collide(curShrinker, mBall))
        {
            blockObject = SHRINKER_TILE;
        } 
    }
        
    if (collide(curFinishLine, mBall) && !curFinishLine.checkIsOpen()) blockObject = CLOSED_FINISH_LINE_TILES;

    for (auto &curPortal: portalsList)
        if (collide(curPortal, mBall) && mBall.checkIsLargeBall() && curPortal.getPortalSize() == PORTAL_SMALL) blockObject = SMALL_PORTAL_TILES;

    return blockObject;
}

void playLevel::tryMoveX()
{
    if (abs(mBall.getVelocityX()) < 1e-3) return;
    
    double ballPosXBeforeMove = mBall.getRealPosX();
    mBall.moveX();

    mCurBlockObjectX = getBlockObject();
    // std::cout << "[playLevel.cpp] Object that blocks X's code: " << mCurBlockObjectX << "\n";

    mBall.scaleX(mLevelMap.getFramePosX());

    if (getBlockObject() != NOT_BLOCKED) 
    {
        mBall.undoMoveX();
        mBall.scaleX(mLevelMap.getFramePosX());
        mBall.setVelocityX(0);
        // mBall.reflectX();
    }

    double ballPosXAfterMove = mBall.getRealPosX();

    mLevelMap.moveX(ballPosXAfterMove - ballPosXBeforeMove);
}

void playLevel::tryMoveY()
{
    double ballPosYBeforeMove = mBall.getRealPosY();
    mBall.moveY();
    mBall.scaleY(mLevelMap.getFramePosY());

    mCurBlockObjectY = getBlockObject();

    if (getBlockObject() != NOT_BLOCKED) 
    {
        mBall.setCollide((mBall.getVelocityY() < 0));
        mBall.undoMoveY();
        mBall.scaleY(mLevelMap.getFramePosY());
        mBall.reflectY();
    }

    double ballPosYAfterMove = mBall.getRealPosY();
    //Only move frame vertically when the ball is out of it
    mLevelMap.moveY((int((ballPosYAfterMove - 1) / (7 * TILE_HEIGHT)) - int((ballPosYBeforeMove - 1) / (7 * TILE_HEIGHT))) * (7 * TILE_HEIGHT));

}

bool playLevel::playGame()
{
    bool levelCleared = 0;
    
    bool quit = 0;
    SDL_Event curEvent;

    mStatusArea.init(mRenderer, mSpritesheet);

    bool respawn = 1;
    int lastCheckpointIndex = 0;
    auto checkpointsList = mLevelMap.checkpointsList();
    for (int i = 0; i < checkpointsList.size(); i ++)
    {
        if (checkpointsList[i].getState() == CHECKPOINT_HIDDEN) lastCheckpointIndex = i;
    }
    checkpointsList[lastCheckpointIndex].setBallSpawnSize(ballSpawnSize);
    mLevelMap.updateCheckpointsList(checkpointsList);
    mLevelMap.setFramePos(checkpointsList[lastCheckpointIndex].getFramePosX(), 
                        checkpointsList[lastCheckpointIndex].getFramePosY());

    auto lifeBallsList = mLevelMap.lifeBallsList();
    
    auto portalsList = mLevelMap.portalsList();
    portalsLeft = portalsList.size();

    auto curFinishLine = mLevelMap.getFinishLine();

    int moveXCount = 0;

    while (!quit)
    {
        // std::cout << "[playLevel.cpp] Start new loop successfully.\n";
        while (SDL_PollEvent(&curEvent) != 0)
        {
            if (curEvent.type == SDL_QUIT) quit = 1;
        }

        // std::cout << "[playLevel.cpp] Check event successfully.\n";

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        auto velocityXVal = (mBall.checkIsLargeBall() ? LARGE_BALL_VELOCITY_X_DEFAULT : SMALL_BALL_VELOCITY_X_DEFAULT);
        auto accelerationXVal = (mBall.checkIsLargeBall() ? LARGE_BALL_ACCELERATION_X_DEFAULT : SMALL_BALL_ACCELERATION_X_DEFAULT);
        auto velocityYVal = (mBall.checkIsLargeBall() ? LARGE_BALL_VELOCITY_Y_DEFAULT : SMALL_BALL_VELOCITY_Y_DEFAULT);
        auto accelerationYVal = (mBall.checkIsLargeBall() ? LARGE_BALL_ACCELERATION_Y_DEFAULT : SMALL_BALL_ACCELERATION_Y_DEFAULT);

        if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) 
        {
            mBall.resetFramesPassedX();
            mBall.setVelocityX(-velocityXVal);
            mBall.setAccelerationX(-accelerationXVal);
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) 
        {
            mBall.resetFramesPassedX();
            mBall.setVelocityX(velocityXVal);
            mBall.setAccelerationX(accelerationXVal);
        }
        if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_SPACE])
        {
            if (!mBall.isAirborne())
            {
                mBall.resetFramesPassedY();
                mBall.setVelocityY(-velocityYVal);
                mBall.setAccelerationY(accelerationYVal);
            }
        }

        // std::cout << "[playLevel.cpp] Get keyboard input successfully.\n";

        SDL_SetRenderDrawColor(mRenderer, SDL_COLOR_MALIBU.r, SDL_COLOR_MALIBU.g, SDL_COLOR_MALIBU.b, 255);
        SDL_RenderClear(mRenderer);

        if (respawn)
        {
            SDL_Delay(200);
            checkpointsList[lastCheckpointIndex].spawnBall(mBall, mSpritesheet);
            mLevelMap.setFramePos(checkpointsList[lastCheckpointIndex].getFramePosX(), 
                                checkpointsList[lastCheckpointIndex].getFramePosY());
            respawn = 0;
        }

        mLevelMap.render(mRenderer, mSpritesheet);

        // std::cout << "[playLevel.cpp] Render map successfully.\n";

        checkpointsList = mLevelMap.checkpointsList();
        lifeBallsList = mLevelMap.lifeBallsList();
        portalsList = mLevelMap.portalsList();
        curFinishLine = mLevelMap.getFinishLine();

        mBall.passFrame();

        mCurBlockObjectX = NOT_BLOCKED;
        mCurBlockObjectY = NOT_BLOCKED;
        
        //Try moving by X
        tryMoveX();

        //Try moving by Y
        tryMoveY();

        mBall.render(mRenderer, mSpritesheet);

        // std::cout << "[playLevel.cpp] Render ball successfully.\n";
        
        //Hit a spike
        for (auto &curSpike: mLevelMap.spikesList())
            if (collide(curSpike, mBall))
            {
                mBall.renderPopAnimation(mRenderer, mSpritesheet);
                mBall.setVelocityX(0);
                mBall.setVelocityY(0);
                livesLeft --;
                respawn = 1;
                break;
            }

        //Hit a pump
        if (mCurBlockObjectX == PUMP_TILE || mCurBlockObjectY == PUMP_TILE)
        {
            mBall.setBallSize(1, mSpritesheet);
        }

        //Hit a shrinker
        if (mCurBlockObjectX == SHRINKER_TILE || mCurBlockObjectY == SHRINKER_TILE)
        {
            mBall.setBallSize(0, mSpritesheet);
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
                        checkpointsList[curIndex].setBallSpawnSize(mBall.checkIsLargeBall());
                        lastCheckpointIndex = curIndex;
                        score += CHECKPOINT_SCORE;
                        break;
                    default:
                        break;
                }
            }
        }
        //Collect a life ball
        for (int curIndex = 0; curIndex < lifeBallsList.size(); curIndex ++)
        {
            if (collide(lifeBallsList[curIndex], mBall))
            {
                if (!lifeBallsList[curIndex].checkIsCollected())
                {
                    lifeBallsList[curIndex].collectLifeBall();
                    score += LIFE_BALL_SCORE;
                    livesLeft ++;
                }
            }
        }

        //Collect a portal
        for (int curIndex = 0; curIndex < portalsList.size(); curIndex ++)
        {
            if (collide(portalsList[curIndex], mBall))
            {
                switch(portalsList[curIndex].checkIsCollected())
                {
                    case PORTAL_DEFAULT:
                    {
                        if (portalsList[curIndex].getPortalSize() == PORTAL_LARGE ||
                            (portalsList[curIndex].getPortalSize() == PORTAL_SMALL && mBall.checkIsLargeBall() == 0))
                        {
                            portalsList[curIndex].collectPortal();
                            portalsLeft --;
                            score += PORTAL_SCORE;
                            break;
                        }
                        
                    }
                        
                    default:
                        break;
                }
            }
        }

        if (portalsLeft == 0)
        {
            if (!curFinishLine.checkIsOpen())
            {
                curFinishLine.openFinishLine();
            }
        }

        //Touch the finish line
        if (collide(curFinishLine, mBall))
        {
            std::cout << "[playLevel.cpp] " << curFinishLine.checkIsOpen() << " " << curFinishLine.getPosX() << " " << curFinishLine.getPosY() << "\n";
            if (curFinishLine.checkIsOpen())
            {
                score += FINISH_LINE_SCORE;
                levelCleared = 1;
                quit = 1;
            }
        }
         
        mLevelMap.updateCheckpointsList(checkpointsList);
        mLevelMap.updateLifeBallsList(lifeBallsList);
        mLevelMap.updatePortalsList(portalsList);
        mLevelMap.updateFinishLine(curFinishLine);

        // std::cout << "[playLevel.cpp] Update successfully.\n";

        //Render the status area
        mStatusArea.render(livesLeft, portalsLeft, score);

        // std::cout << "[playLevel.cpp] Render status area successfully.\n";


        SDL_RenderPresent(mRenderer);

        // std::cout << "[playLevel.cpp] Present renderer successfully.\n";
    }
    return levelCleared;
}