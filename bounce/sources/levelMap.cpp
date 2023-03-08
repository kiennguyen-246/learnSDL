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
}

void levelMap::moveX(const int& dir, const int& v)
{
    curFramePosX = int((curFramePosX + 1) / v) * v;
    curFramePosX += dir;
    curFramePosX += dir * v;
    //if (curFramePosX % TILE_WIDTH != 0) curFramePosX = int(curFramePosX / TILE_WIDTH + 1) * TILE_WIDTH;
}

int levelMap::getFramePosX() const
{
    return curFramePosX;
}

std::vector <brickTile> levelMap::brickTilesList() const
{
    return vBrickTiles;
}

void levelMap::clearBrickTilesList()
{
    vBrickTiles.clear();
}

void levelMap::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    int curCharPosX = curFramePosX / TILE_WIDTH;
    int curCharPosY = curFramePosY / TILE_HEIGHT;
    int remFrameX = curFramePosX % TILE_WIDTH;
    int remFrameY = TILE_HEIGHT - curFramePosY % TILE_HEIGHT;

    // std::cout << remFrameX << "\n";

    int maxCharTileX = GAMEPLAY_AREA_WIDTH / TILE_WIDTH;
    int maxCharTileY = GAMEPLAY_AREA_HEIGHT / TILE_HEIGHT;

    for (int i = 0; i < maxCharTileX + 1; i ++)
    {
        for (int j = 0; j < maxCharTileY; j ++)
        {
            if (charMap[curCharPosY + j][curCharPosX + i] == BRICK_CHAR_SYMBOL)
            {
                auto* curBrick = new brickTile(BRICK_TILE_WIDTH * i - remFrameX, BRICK_TILE_HEIGHT * (j + 1));
                curBrick->setSpriteClip(spritesheet, BRICK_TILE_SPRITE_POS_x, BRICK_TILE_SPRITE_POS_Y, BRICK_TILE_WIDTH, BRICK_TILE_HEIGHT);
                curBrick->render(renderer, spritesheet);

                // if (isFreeBrickTile(curCharPosX + i, curCharPosY + j)) vObject.push_back(*curBrick);
                vBrickTiles.push_back(*curBrick);
            }
        }
    }
    // std::cout << "run" << "\n";

    // auto* curBrick = new brickTile(-40, 720);
    // curBrick->setSpriteClip(spritesheet, BRICK_TILE_SPRITE_POS_x, BRICK_TILE_SPRITE_POS_Y, BRICK_TILE_WIDTH, BRICK_TILE_HEIGHT);
    // curBrick->render(renderer, spritesheet);

    // std::cout << "run here\n";
}