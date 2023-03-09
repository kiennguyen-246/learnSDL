// levelMap.cpp
#include "levelMap.h"

brickTile::brickTile()
{
    setSize(BRICK_TILE_WIDTH, BRICK_TILE_HEIGHT);
}

brickTile::brickTile(const int& __PosX, const int& __PosY)
{
    mPosX = __PosX;
    mPosY = __PosY;
    setSize(BRICK_TILE_WIDTH, BRICK_TILE_HEIGHT);
}

void brickTile::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    spritesheet.render(renderer, mPosX, mPosY - BRICK_TILE_HEIGHT, &mSpriteClip);
}

levelMap::levelMap(/* args */)
{
}

levelMap::~levelMap()
{
}

bool levelMap::isFreeBrickTile(const int& brickTileCharPosX, const int& brickTileCharPosY)
{
    for (int curDir = 0; curDir < 4; curDir ++)
    {
        int adjBrickX = brickTileCharPosX + DIRX[curDir];
        int adjBrickY = brickTileCharPosY + DIRY[curDir];
        if (adjBrickX < 0 || adjBrickX >= charMap[brickTileCharPosY].size()) continue;
        if (adjBrickY < 0 || adjBrickY >= charMap.size()) continue;
        if (charMap[adjBrickY][adjBrickX] != BRICK_CHAR_SYMBOL) return 1;
    }
    return 0;
}

void levelMap::setMap(const int& id)
{
    charMap = LEVEL_CHAR_MAP[id];

    vCheckpoints.clear();
    for (int j = 0; j < charMap.size(); j ++)
        for (int i = 0; i < charMap[j].size(); i ++)
        {
            if (charMap[j][i] == 'C')
            {
                auto* curCheckpoint = new checkpoint;
                curCheckpoint->init(i, j);
                vCheckpoints.push_back(*curCheckpoint);
            }
            else if (charMap[j][i] == 'c')
            {
                auto* curCheckpoint = new checkpoint;
                curCheckpoint->init(i, j, CHECKPOINT_HIDDEN);
                vCheckpoints.push_back(*curCheckpoint);
            }
        }
            
}

void levelMap::moveX(const double& dist)
{
    curFramePosX += dist;
}

int levelMap::getFramePosX() const
{
    return curFramePosX;
}

void levelMap::setFramePos(const double& framePosX, const double& framePosY)
{
    curFramePosX = framePosX;
    curFramePosY = framePosY;
}

std::vector <brickTile> levelMap::brickTilesList() const
{
    return vBrickTiles;
}

std::vector <checkpoint> levelMap::checkpointsList() const
{
    return vCheckpoints;
}

void levelMap::clearBrickTilesList()
{
    vBrickTiles.clear();
}

void levelMap::updateCheckpointsList(const std::vector <checkpoint>& newCheckpointsList)
{
    vCheckpoints = newCheckpointsList;
}

void levelMap::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    int curCharPosX = curFramePosX / TILE_WIDTH;
    int curCharPosY = curFramePosY / TILE_HEIGHT;
    double remFrameX = curFramePosX - int(curFramePosX / TILE_WIDTH) * TILE_WIDTH;
    // double remFrameY = TILE_HEIGHT - curFramePosY % TILE_HEIGHT;

    int maxCharTileX = GAMEPLAY_AREA_WIDTH / TILE_WIDTH;
    int maxCharTileY = GAMEPLAY_AREA_HEIGHT / TILE_HEIGHT;

    for (int i = 0; i < maxCharTileX + 1; i ++)
    {
        for (int j = 0; j < maxCharTileY; j ++)
        {
            if (charMap[curCharPosY + j][curCharPosX + i] == BRICK_CHAR_SYMBOL)
            {
                auto* curBrick = new brickTile(BRICK_TILE_WIDTH * i - int(remFrameX), BRICK_TILE_HEIGHT * (j + 1));
                curBrick->setSpriteClip(spritesheet, BRICK_TILE_SPRITE_POS_x, BRICK_TILE_SPRITE_POS_Y, BRICK_TILE_WIDTH, BRICK_TILE_HEIGHT);
                curBrick->render(renderer, spritesheet);

                vBrickTiles.push_back(*curBrick);
            }
            else if (charMap[curCharPosY + j][curCharPosX + i] == CHECKPOINT_CHAR_SYMBOL || charMap[curCharPosY + j][curCharPosX + i] == CHECKPOINT_START_CHAR_SYMBOL)
            {
                int cnt = 0;
                for (auto &curCheckpoint: vCheckpoints)
                {
                    if (curCheckpoint.getCharmapPosX() != curCharPosX + i || curCheckpoint.getCharmapPosY() != curCharPosY + j) continue;
                    curCheckpoint.setPos(CHECKPOINT_WIDTH * i - int(remFrameX),
                                         CHECKPOINT_HEIGHT * (j + 1));
                    // if (charMap[curCharPosY + j][curCharPosX + i] == CHECKPOINT_START_CHAR_SYMBOL) 
                    //     std::cout << cnt << " " << curCheckpoint.getState() << "\n" << curCheckpoint.getPosX() << " " << curCheckpoint.getPosY() << "\n";
                    
                    curCheckpoint.render(renderer, spritesheet);
                }
            }

        }
    }
}