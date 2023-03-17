//game.cpp
#include "game.h"

using std::cout;

game::game()
{
    mWindow = NULL;
    mRenderer = NULL;
}

game::~game()
{
    clear();
}

bool game::initSDL()
{   
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed to initialize SDL. Error: " << SDL_GetError() << ".\n";
        return 0;
    }

    mWindow = SDL_CreateWindow("bounce", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == NULL)
    {
        cout << "Failed to initialize SDL window. Error: " << SDL_GetError() << ".\n";
        return 0;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (mRenderer == NULL)
    {
        cout << "Failed to initialize SDL renderer. Error: " << SDL_GetError() << ".\n";
        return 0;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        cout << "Unable to start SDL_Img. Error: "  << IMG_GetError() << ".\n";
        return 0;
    }

    if (TTF_Init() == -1)
    {
        cout << "Unable to start SDL_TTF. Error: " << TTF_GetError() << ".\n";
        return 0;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 || Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) < 0) ///wav or mp3
    {
        cout << "Unable to start SDL_Mixer. Error: " << Mix_GetError() << ".\n";
        return 0;
    }

    return 1;
}

void game::initMapConfig()
{
    std::ifstream fi(MAPCONFIG_PATH);
    if (fi.is_open())
    {
        int levelCount;
        fi >> levelCount;
        allLevelCharMap.resize(levelCount + 1);
        allLevelSpidersInfo.resize(levelCount + 1);
        allLevelBallSpawnSize.resize(levelCount + 1);

        for (int levelId = 1; levelId <= levelCount; levelId ++)
        {
            int mapHeight;
            fi >> mapHeight;
            allLevelCharMap[levelId].resize(mapHeight);

            std::string nul = "";
            getline(fi, nul);
            for (int rowId = 0; rowId < mapHeight; rowId ++)
            {
                getline(fi, allLevelCharMap[levelId][rowId]);
            }

            fi >> allLevelBallSpawnSize[levelId];
                
            int spiderCount;
            fi >> spiderCount;
            allLevelSpidersInfo[levelId].resize(spiderCount + 1);

            // getline(fi, nul);
            for (int spiderId = 0; spiderId < spiderCount; spiderId ++)
            {
                auto& curSpider = allLevelSpidersInfo[levelId][spiderId];
                fi >> curSpider.x1 >> curSpider.y1;
                fi >> curSpider.x2 >> curSpider.y2;
                fi >> curSpider.startPos;
            }
        }
    }
    else
    {
        std::cout << "Cannot open config file.";
    }
    

}

void game::preset()
{
    if (!initSDL())
    {
        cout << "SDL initialization failed.\n";
        return;
    }

    initMapConfig();

    mSpritesheet.loadTexture(mRenderer, &SPRITESHEET_PATH[0], SDL_COLOR_MALIBU);
}

void game::play()
{
    auto* curLevel = new playLevel(mWindow, mRenderer, mSpritesheet);
    curLevel->setLivesLeft(LIVES_LEFT_DEFAULT);
    curLevel->setScore(0);
    for (int i = 4; i < allLevelCharMap.size(); i ++)
    {
        curLevel->setLevelId(i, allLevelCharMap, allLevelSpidersInfo, allLevelBallSpawnSize);
        
        if (!curLevel->playGame()) break;       //Only continue if cleared level
    }
    
}

void game::clear()
{
    SDL_DestroyRenderer(mRenderer); mRenderer = NULL;
    SDL_DestroyWindow(mWindow); mWindow = NULL;

    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}