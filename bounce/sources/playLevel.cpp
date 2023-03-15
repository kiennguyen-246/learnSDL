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

void playLevel::setLevelId(const int& id, const vector_2d_string& allLevelCharMap, const vector_2d_spiderInfo& allLevelSpidersInfo, const std::vector <int>& allLevelBallSpawnSize)
{
    levelId = id;
    mLevelMap.setMap(id, allLevelCharMap);
    ballSpawnSize = allLevelBallSpawnSize[id];
    initSpiders(allLevelSpidersInfo[id]);
}

void playLevel::initSpiders(const std::vector <spiderInfo>& allLevelSpidersInfo)
{
    mSpiderList.resize(allLevelSpidersInfo.size());
    for (int i = 0; i < allLevelSpidersInfo.size(); i ++)
    {
        mSpiderList[i].init(allLevelSpidersInfo[i], mSpritesheet);
    }
}

BLOCK_OBJECT playLevel::getBlockObject() const
{
    auto brickTilesList = mLevelMap.brickTilesList();
    auto trampolineTilesList = mLevelMap.trampolineTilesList();
    auto portalsList = mLevelMap.portalsList();
    auto curFinishLine = mLevelMap.getFinishLine();
    auto pumpsList = mLevelMap.pumpsList();
    auto shrinkersList = mLevelMap.shrinkersList();
    
    BLOCK_OBJECT blockObject = NOT_BLOCKED;

    for (auto &curBrickTile: brickTilesList)
        if (collide(curBrickTile, mBall)) blockObject = BRICK_TILE;

    for (auto &curTrampolineTile: trampolineTilesList)
        if (collide(curTrampolineTile, mBall)) blockObject = TRAMPOLINE_TILE;

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

void playLevel::tryMoveX(double& ballPosXBeforeMove, double& ballPosXAfterMove)
{
    if (abs(mBall.getVelocityX()) < 1e-3) return;
    
    ballPosXBeforeMove = mBall.getRealPosX();
    mBall.moveX();

    mCurBlockObjectX = getBlockObject();
    // std::cout << "[playLevel.cpp] Object that blocks X's code: " << mCurBlockObjectX << "\n";

    mBall.scaleX(mLevelMap.getFramePosX());

    if (getBlockObject() != NOT_BLOCKED) 
    {
        mBall.undoMoveX();
        mBall.scaleX(mLevelMap.getFramePosX());
        if (mBall.getVelocityX() > 0)
        {
            while (getBlockObject() == NOT_BLOCKED)
            {
                mBall.setPosEx(mBall.getRealPosX() + 1, mBall.getRealPosY(), mLevelMap.getFramePosX(), mLevelMap.getFramePosY());
                mBall.scaleX(mLevelMap.getFramePosX());
            }
            // std::cout << "[playLevel.cpp] Exit loop successfullly.\n";
            mBall.setPosEx(mBall.getRealPosX() - 1, mBall.getRealPosY(), mLevelMap.getFramePosX(), mLevelMap.getFramePosY());
            mBall.scaleX(mLevelMap.getFramePosX());
        } 
        if (mBall.getVelocityX() < 0)
        {
            while (getBlockObject() == NOT_BLOCKED)
            {
                mBall.setPosEx(mBall.getRealPosX() - 1, mBall.getRealPosY(), mLevelMap.getFramePosX(), mLevelMap.getFramePosY());
                mBall.scaleX(mLevelMap.getFramePosX());
            }
            // std::cout << "[playLevel.cpp] Exit loop successfullly.\n";
            mBall.setPosEx(mBall.getRealPosX() + 1, mBall.getRealPosY(), mLevelMap.getFramePosX(), mLevelMap.getFramePosY());
            mBall.scaleX(mLevelMap.getFramePosX());
        } 
        mBall.reflectX();
    }

    ballPosXAfterMove = mBall.getRealPosX();

    mLevelMap.moveX(ballPosXAfterMove - ballPosXBeforeMove);
    mBall.scaleX(mLevelMap.getFramePosX());
}

void playLevel::tryMoveY()
{
    double ballPosYBeforeMove = mBall.getRealPosY();
    mBall.moveY();
    mBall.scaleY(mLevelMap.getFramePosY());

    mCurBlockObjectY = getBlockObject();

    if (getBlockObject() != NOT_BLOCKED) 
    {
        mBall.undoMoveY();
        mBall.scaleY(mLevelMap.getFramePosY());
        if (mBall.getVelocityY() > 0)
        {
            while (getBlockObject() == NOT_BLOCKED)
            {
                mBall.setPosEx(mBall.getRealPosX(), mBall.getRealPosY() + 1, mLevelMap.getFramePosX(), mLevelMap.getFramePosY());
                mBall.scaleY(mLevelMap.getFramePosY());
            }
            // std::cout << "[playLevel.cpp] Exit loop successfullly.\n";
            mBall.setPosEx(mBall.getRealPosX(), mBall.getRealPosY() - 1, mLevelMap.getFramePosX(), mLevelMap.getFramePosY());
            mBall.scaleY(mLevelMap.getFramePosY());
        } 
        
        mBall.reflectY();
    }

    if (mBall.getPosY() < 0)
    {
        mBall.undoMoveY();
        mLevelMap.moveY(-7 * TILE_WIDTH);
        mBall.scaleY(mLevelMap.getFramePosY());
        yFrameChanged = 1;
    }
    if (mBall.getPosY() > 8 * TILE_WIDTH)
    {
        mBall.undoMoveY();
        mLevelMap.moveY(7 * TILE_WIDTH);
        mBall.scaleY(mLevelMap.getFramePosY());
        yFrameChanged = 1;
    }

    // std::cout << "[playLevel.cpp] Ball position before and after move: " << ballPosYBeforeMove << " " << ballPosYAfterMove << "\n";
    //Only move frame vertically when the ball is out of it
    

}

bool playLevel::checkBallIsAirborne()
{
    int charMapPosX = (mBall.getRealPosX()) / TILE_WIDTH;
    int charMapPosY = (mBall.getRealPosY() + 5) / TILE_HEIGHT;
    switch(mLevelMap.getMap()[charMapPosY][charMapPosX])
    {
        case BRICK_CHAR_SYMBOL:
        case TRAMPOLINE_CHAR_SYMBOL:
        case PUMP_CHAR_SYMBOL:
        case SHRINKER_CHAR_SYMBOL:
            return 0;
        case PORTAL_VERTICAL_SMALL_CHAR_SYMBOL:
        case PORTAL_HORIZONTAL_SMALL_CHAR_SYMBOL:
            if (mBall.checkIsLargeBall()) return 0;
            break;
        default:
            break;
    }

    if (mBall.getRealPosX() + (mBall.checkIsLargeBall() ? LARGE_BALL_WIDTH : SMALL_BALL_WIDTH) - 5 >= (charMapPosX + 1) * TILE_WIDTH)
    {
        switch(mLevelMap.getMap()[charMapPosY][charMapPosX + 1])
        {
            case BRICK_CHAR_SYMBOL:
            case TRAMPOLINE_CHAR_SYMBOL:
            case PUMP_CHAR_SYMBOL:
            case SHRINKER_CHAR_SYMBOL:
                return 0;
            case PORTAL_VERTICAL_SMALL_CHAR_SYMBOL:
            case PORTAL_HORIZONTAL_SMALL_CHAR_SYMBOL:
                if (mBall.checkIsLargeBall()) return 0;
                break;
            default:
                break;
        }
    } 
    
    return 1;
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
    
    double lastTrampolineVelocity = -SMALL_BALL_VELOCITY_Y_DEFAULT;

    int tmpCnt = 0;

    while (!quit)
    {   
        // SDL_Delay(100);
        
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
            if (!checkBallIsAirborne())
            {
                mBall.resetFramesPassedY();
                
                int charMapPosX = mBall.getRealPosX() / TILE_WIDTH;
                int charMapPosY = (mBall.getRealPosY() + 10) / TILE_HEIGHT;
                if (mLevelMap.getMap()[charMapPosY][charMapPosX] == TRAMPOLINE_CHAR_SYMBOL || mLevelMap.getMap()[charMapPosY][charMapPosX + 1] == TRAMPOLINE_CHAR_SYMBOL)
                {
                    if (abs(mBall.getVelocityY()) < 1e-7) mBall.setVelocityY(-velocityYVal * (100 + TRAMPOLINE_SPEED_BOOST_LEVEL) / 100);
                    else mBall.setVelocityY(lastTrampolineVelocity * (100 + TRAMPOLINE_SPEED_BOOST_LEVEL) / 100);
                    if (mBall.getVelocityY() < -BALL_MAX_VELOCITY_Y) mBall.setVelocityY(-BALL_MAX_VELOCITY_Y); 
                    lastTrampolineVelocity = mBall.getVelocityY();
                    // std::cout << "[playLevel.cpp] Velocity: " << mBall.getVelocityY() << "\n";
                }
                else 
                {
                    mBall.setVelocityY(-velocityYVal);
                    lastTrampolineVelocity = mBall.getVelocityY();
                }
                mBall.setAccelerationY(accelerationYVal);
            }
        }
        else lastTrampolineVelocity = -velocityYVal;

        // std::cout << "[playLevel.cpp] Get keyboard input successfully.\n";

        SDL_SetRenderDrawColor(mRenderer, SDL_COLOR_MALIBU.r, SDL_COLOR_MALIBU.g, SDL_COLOR_MALIBU.b, 255);
        SDL_RenderClear(mRenderer);

        mLevelMap.render(mRenderer, mSpritesheet);

        for (auto &curSpider: mSpiderList) 
        {
            curSpider.move();
            curSpider.scaleX(mLevelMap.getFramePosX());
            curSpider.scaleY(mLevelMap.getFramePosY());
        }

        for (auto &curSpider: mSpiderList)
        {
            if (curSpider.getPosX() < -SPIDER_WIDTH || curSpider.getPosY() > GAMEPLAY_AREA_WIDTH) curSpider.deleteFromRenderer();
            if (curSpider.getPosY() < -SPIDER_HEIGHT || curSpider.getPosY() > GAMEPLAY_AREA_HEIGHT) curSpider.deleteFromRenderer();
            curSpider.render(mRenderer, mSpritesheet);
        } 

        checkpointsList = mLevelMap.checkpointsList();
        lifeBallsList = mLevelMap.lifeBallsList();
        portalsList = mLevelMap.portalsList();
        curFinishLine = mLevelMap.getFinishLine();

        // std::cout << "[playLevel.cpp] Ball render position: " << mBall.getPosX() << " " << mBall.getPosY() << "\n";

        // std::cout << "[playLevel.cpp] Render ball successfully.\n";

        // Hit a trampoline tile
        if (mCurBlockObjectY == TRAMPOLINE_TILE) 
        {
            mBall.setVelocityY(mBall.getVelocityY() * (100 + TRAMPOLINE_SPEED_BOOST_LEVEL) / 100);
        }
        
        //Hit a spider
        for (auto &curSpider: mSpiderList)
        {
            if (collide(curSpider, mBall))
            {
                mBall.renderPopAnimation(mRenderer, mSpritesheet);
                mBall.setVelocityX(0);
                mBall.setVelocityY(0);
                livesLeft --;
                respawn = 1;
                break;
            }
        } 

        //Hit a spike
        for (auto &curSpike: mLevelMap.spikesList())
        {
            if (collide(curSpike, mBall))
            {
                // std::cout << "[playLevel.cpp] Hit spike at position " << curSpike.getPosX() << " " << curSpike.getPosY() << ".\n";
                // std::cout << "[playLevel.cpp] Ball render position is:  " << mBall.getPosX() << " " << mBall.getPosY() << ".\n";
                mBall.renderPopAnimation(mRenderer, mSpritesheet);
                mBall.setVelocityX(0);
                mBall.setVelocityY(0);
                livesLeft --;
                respawn = 1;
                break;
            }
            
        }   

        //Hit a pump
        if (mCurBlockObjectX == PUMP_TILE || mCurBlockObjectY == PUMP_TILE)
        {
            mBall.setBallSize(1, mSpritesheet);
            double ballPosXBeforeMove = mBall.getRealPosX();
            for (auto curPump: mLevelMap.pumpsList())
                if (collide(curPump, mBall))
                {
                    mBall.setPosEx(mBall.getRealPosX() - 20, mBall.getRealPosY(), mLevelMap.getFramePosX(), mLevelMap.getFramePosY());
                }
            double ballPosXAfterMove = mBall.getRealPosX();
            mLevelMap.moveX(ballPosXAfterMove - ballPosXBeforeMove);

            for (auto curPump: mLevelMap.pumpsList())
                if (collide(curPump, mBall))
                {
                    mBall.setPosEx(mBall.getRealPosX(), mBall.getRealPosY() - 20, mLevelMap.getFramePosX(), mLevelMap.getFramePosY());
                }
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
                    if (livesLeft > 5) livesLeft = 5;
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

        if (respawn)
        {
            SDL_Delay(200);
            mLevelMap.setFramePos(checkpointsList[lastCheckpointIndex].getFramePosX(), 
                                checkpointsList[lastCheckpointIndex].getFramePosY());
            checkpointsList[lastCheckpointIndex].spawnBall(mBall, mSpritesheet);
            respawn = 0;
        }

        // std::cout << "[playLevel.cpp] Render map successfully.\n";

        // SDL_Delay(100);

        mBall.passFrame();

        mCurBlockObjectX = NOT_BLOCKED;
        mCurBlockObjectY = NOT_BLOCKED;
        yFrameChanged = 0;

        double ballPosXBeforeMove = 0;
        double ballPosXAfterMove = 0;

        // std::cout << "[levelMap.cpp] " << tmpCnt++ << "\n";

        //Try moving by X
        tryMoveX(ballPosXBeforeMove, ballPosXAfterMove);

        //Try moving by Y
        tryMoveY();

        if (yFrameChanged && ballPosXBeforeMove != ballPosXAfterMove)
        {
            mBall.undoMoveX();
            mLevelMap.moveX(ballPosXBeforeMove - ballPosXAfterMove);
            mBall.scaleX(mLevelMap.getFramePosX());
        }

        mBall.render(mRenderer, mSpritesheet);

        // std::cout << "[playLevel.cpp] Render status area successfully.\n";


        SDL_RenderPresent(mRenderer);

        // std::cout << "[playLevel.cpp] Present renderer successfully.\n";
    }
    return levelCleared;
}