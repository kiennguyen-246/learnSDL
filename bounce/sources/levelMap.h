// levelMap.h

/**
 * @brief This file controls the map of the game and how it is rendered
 * Logic:
 * - Horizontally, the map will move along with the ball. While playing, the ball will "stand still"
 * at some position during the game. The balls position is fixed at 7 brick tiles (560px) from the 
 * leftmost position of the game area.
 * - The map shifts up 7 brick tiles when the ball moves vertically out.
 * - The collide checking function will check colliding objects based on the position of the render map
 */

#ifndef LEVELMAP_GUARD
#define LEVELMAP_GUARD

#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "base.h"
#include "ball.h"
#include "levelMap.h"
#include "checkpoint.h"
#include "enemy.h"

/**
 * @brief 
 * - B: bricks
 * - R: vertical large portals, together with "+"
 * - r: vertical small portals
 * - E: horizontal small portals, together with "+"
 * - e: horizontal small portals
 * - T: spikes
 * - C: checkpoints
 * - L: extra lives
 * - P: pumps
 * - D: shrinkers
 * - G: Finish line, together with "#"
 * 
 */
const std::vector <std::string> LEVEL_CHAR_MAP[3] = 
{
    {""},

    {
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"          ,
        "     B    BB   BBBBBB          BBB         BB         BBB                  BBB      BB  L   BB               BB         BB"          ,
        "    BB    BBc  BBBBBB          BBB         R          BB                    BB      BB      BB               BB         BB"          ,
        "   B B    BB   BBBBBB   BB     BBB         +          BB         C          BB  BB  BB  BB  BB  BB           BB         BB"          ,
        "  B  B    BB   BBBBBB   BB     BBB  BBBB  BBBB  BBBB  BB                    BB  BB  BB  BB  BB  BB       BBE-BBE-BB     BB"          ,
        "     B    BB       R    BB          BB     BB     BB                            BB  R   BB  R   BB    BBTBB  C   BBTBB  G#"          ,
        "  BBBBBBB BB       +    BB T        BB  T     T   BB        T  T   T   T        BB  +   BB  +   BB    BBBBB      BBBBB  ##"          ,
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"          ,
    },

    {
        "                                                                                                              BBBBBBBBBBBBBBBBBBBBBBBB"
        "                                                                                                              BBC                   BB"
        "                                                                                                              BB          R         BB"
        "                                                                                                              BB          +         BB"
        "                                                                                                              BBBBBBBBBBBBBB        BB"
        "                                                                                                              BB                    G#"
        "                                                                                                              BB                    ##"
        "                                                                                                              BB   BBBBBBBBBBBBBBBBBBB"
        "                                                                                                              BB                    BB"
        "                                                                                                              BB                    BB"
        "                                                                                                              BBBB      BB  BB      BB"
        "                                                                                                              BBC      BB    BB     BB"
        "                                                                                                              BB      BB      BB    BB"
        "                                                                                                              BB     BB   T    BB   BB"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBE-BB"
        "BB                    R   BB      BB     R                        BB        C           BB         BB         R   BB         BB     BB"
        "BB       c            +   BB      BB     +                        BB                    BB         BB         +   BB         BB     BB"
        "BB             BBBBBBBBB  BB  BB         BBBBB                    BB        XZ          BB         R          BB  BB    BB   BB  BBBBB"
        "BB             BB     BB  BB  BB         BBBBB                    BB       XBBZ         BB         +          BB  BB  BBBB          BB"
        "BB             BB L           BB         BBBBB                    R       XBBBBZ        R          BB         BB      L BB          BB"
        "BB           T BB             BBP        BBBBB                    +      XBBBBBBZ       +          BB         BB        BB        T BB"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBDDDDDDDDDBBBBBBBBBBBBBBBBBBBBBBBB"
    }
};

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;

const int BRICK_TILE_SPRITE_POS_x = 170;   //position in spritesheet
const int BRICK_TILE_SPRITE_POS_Y = 330;
const int BRICK_TILE_WIDTH = 80;
const int BRICK_TILE_HEIGHT = 80;

const char BRICK_CHAR_SYMBOL = 'B';
const char CHECKPOINT_CHAR_SYMBOL = 'C';
const char CHECKPOINT_START_CHAR_SYMBOL = 'c';
const char SPIKE_CHAR_SYMBOL = 'T';
const char SPIKE_HORIZONTAL_CHAR_SYMBOL = 't';

const int DIRX[] = {1, -1, 0, 0};
const int DIRY[] = {0, 0, 1, -1};

class brickTile: public gameObject
{
private:
    
public:
    /// @brief Default constructor
    brickTile();

    /// @brief Create a brick tile that has the bottom left coordinate of (__PosX, __PosY)
    brickTile(const int& __PosX, const int& __PosY);

    void render(SDL_Renderer* renderer, LTexture& spritesheet);
};

class levelMap
{
private:
    /// @brief The encoded map of the level
    std::vector <std::string> charMap;

    /// @brief List of brick tiles
    std::vector <brickTile> vBrickTiles;

    /// @brief List of spikes
    std::vector <spike> vSpikes;

    /// @brief List of checkpoints
    std::vector <checkpoint> vCheckpoints;

    /// @brief The bottom left position of the current frame, in comparison with the full level map
    double curFramePosX, curFramePosY;

    /// @brief Check if a brick tile is free when it is not surrounded by other bricks
    bool isFreeBrickTile(const int& brickTileCharPosX, const int& brickTileCharPosY);

public:
    levelMap(/* args */);
    ~levelMap();

    /// @brief Set up the map by the level
    /// @param id The index of the current level
    void setMap(const int& id);

    /// @brief Move the map horizontally
    /// @param v Velocity
    void moveX(const double& dist);

    /// @brief Set the bottom left position of the current frame, in comparison with the full level map
    void setFramePos(const double& framePosX, const double& framePosY);

    /// @brief Get the list of brick tiles rendered on the map
    std::vector <brickTile> brickTilesList() const;

    /// @brief Get the list of checkpoints in the map
    std::vector <checkpoint> checkpointsList() const;

    /// @brief Get the list of spikes rendered on the map
    std::vector <spike> spikesList() const;

    /// @brief Update the checkpoints list
    void updateCheckpointsList(const std::vector <checkpoint>& newCheckpointsList);

    int getFramePosX() const;

    void render(SDL_Renderer* renderer, LTexture& spritesheet);

};

#endif