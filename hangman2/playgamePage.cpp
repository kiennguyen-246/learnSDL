// playgamePage.cpp
#include <iostream>
#include <vector>
#include <string>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "playgamePage.h"

using std::cout;
using random::randInt;

const int ENDGAME_RENDER_POS_X[3] = {1000, 1000, 500};
const int ENDGAME_RENDER_POS_Y[3] = {400, 150, 400};

playgamePage::playgamePage()
{
    mWindow = NULL;
    mRenderer = NULL;
}

playgamePage::~playgamePage()
{
    clear();
}

bool playgamePage::loadSound()
{
    yeahSound = Mix_LoadMUS(&PATH_YEAHSOUND_SOUND[0]);
    if (yeahSound == NULL)
    {
        cout << "Failed to open media.\n";
        return 0;
    }

    wrongAnswer = Mix_LoadMUS(&PATH_WRONGANSWER_SOUND[0]);
    if (yeahSound == NULL)
    {
        cout << "Failed to open media.\n";
        return 0;
    }

    spectre = Mix_LoadMUS(&PATH_SPECTRE_SOUND[0]);
    if (yeahSound == NULL)
    {
        cout << "Failed to open media.\n";
        return 0;
    }

    alarm = Mix_LoadMUS(&PATH_ALARM_SOUND[0]);
    if (yeahSound == NULL)
    {
        cout << "Failed to open media.\n";
        return 0;
    }
    return 1;
}

bool playgamePage::loadImages()
{
    if (!likeEmoji.loadTexture(mRenderer, &PATH_LIKE_EMOJI[0]))
    {
        cout << "Failed to load image.\n"; 
        return 0;
    }

    if (!sunglasses.loadTexture(mRenderer, &PATH_SUNGLASSES_EMOJI[0]))
    {
        cout << "Failed to load image.\n"; 
        return 0;
    }

    if (!zhongXina.loadTexture(mRenderer, &PATH_ZHONGXINA_EMOJI[0]))
    {
        cout << "Failed to load image.\n"; 
        return 0;
    }

    if (!witherAway.loadTexture(mRenderer, &PATH_WITHERAWAY_EMOJI[0]))
    {
        cout << "Failed to load image.\n"; 
        return 0;
    }

    if (!yellowSad.loadTexture(mRenderer, &PATH_YELLOWSAD_EMOJI[0]))
    {
        cout << "Failed to load image.\n"; 
        return 0;
    }

    if (!theRock.loadTexture(mRenderer, &PATH_THEROCK[0]))
    {
        cout << "Failed to load image.\n"; 
        return 0;
    }
    return 1;
}

GAME_DIFFICULTY playgamePage::setDifficulty(const int& __dif)
{
    difficulty = GAME_DIFFICULTY(__dif);
}

bool playgamePage::victory()
{
    return mGuessWord.victory();
}

bool playgamePage::defeat()
{
    return mLivesBox.defeat();
}

void playgamePage::preset(SDL_Window* window, SDL_Renderer* renderer)
{
    mWindow = window;
    mRenderer = renderer;
    
    if (!mHintBox.loadHintButtonTexture(mRenderer))
    {
        cout << "Loading hint boxes failed.\n";
        return;
    }

    if (!mKeyboard.loadCharTexture(mRenderer))
    {
        cout << "Loading alphabet characters texture failed.\n";
        return;
    }

    if (!loadImages())
    {
        cout << "Loading images failed.\n";
        return;
    }

    theRock.set(FRAME_COUNT_THEROCK, FRAME_DELAY_THEROCK);
    witherAway.set(FRAME_COUNT_WITHERAWAY_EMOJI, FRAME_DELAY_WITHERAWAY_EMOJI);

    if (!loadSound())
    {
        cout << "Loading soundtracks failed.\n";
        return;
    }

    //Hint boxes setup
    mHintBox.setButtons();

    //On-screen keyboard setup
    mKeyboard.set();

    //Setup the dictionary
    mDictionary.init();

    //Setup the hangman model
    mHangmanModel.init(mRenderer);
}

void playgamePage::startPlaygamePage()
{
    //Set the difficulty
    setDifficulty(randInt(1, 3));

    //Get the answer key
    key = mDictionary.getWord(difficulty);

    //Hidden word setup
    mGuessWord.init(key);
    
    //Set the number of lives
    mLivesBox.set(difficulty);

    bool gameOver = 0;

    bool quit = 0;
    SDL_Event curEvent;
    int frameCount = 0;

    while (!quit)
    {
        bool keyboardTriggered = 0;
        bool isIn = 0;
        
        while (SDL_PollEvent(&curEvent) != 0)
        {
            if (curEvent.type == SDL_QUIT) quit = 1;
            else 
            {
                mKeyboard.handleEvent(curEvent, key, mGuessWord, keyboardTriggered, isIn);
                mHintBox.handleEvent(curEvent, key);
            }
        }

        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);

        //Render the word need guessing
        mGuessWord.render(mRenderer);

        //Render "Lives left" box
        mLivesBox.render(mRenderer);

        //Render the hint boxes
        mHintBox.renderHintBox(mRenderer, mLivesBox.getLivesConsumed());

        //Render the keyboard
        mKeyboard.render(mRenderer);

        //Render the hint text
        mHintBox.renderHintText(mRenderer);

        mHangmanModel.render(mRenderer, mLivesBox.getLivesConsumed());

        //A button on the keyboard is pressed
        if (keyboardTriggered)
        {
            //Correct letter provided
            if (isIn) 
            {
                Mix_PlayMusic(yeahSound, 0);
            }
            //Incorrect letter provided
            else 
            {
                mLivesBox.consumeLife();
                Mix_PlayMusic(wrongAnswer, 0);
            }
            //Game ends when no lives left
            if (this->defeat()) 
            {
                gameOver = 1;  
                Mix_PlayMusic(alarm, -1);
            }
            //Game ends when the word is successfully guessed
            if (this->victory()) 
            {
                gameOver = 1;
                Mix_PlayMusic(spectre, -1);
            }
        }

        if (gameOver)
        {
            //Victory
            if (this->victory())
            {
                likeEmoji.render(mRenderer, ENDGAME_RENDER_POS_X[0], ENDGAME_RENDER_POS_Y[0]);
                sunglasses.render(mRenderer, ENDGAME_RENDER_POS_X[1], ENDGAME_RENDER_POS_Y[1]);
                theRock.render(mRenderer, ENDGAME_RENDER_POS_X[2], ENDGAME_RENDER_POS_Y[2], 
                               &theRock.clips[(frameCount / FRAME_DELAY_THEROCK) % FRAME_COUNT_THEROCK]);
            }
            //Game over
            if (this->defeat())
            {
                zhongXina.render(mRenderer, ENDGAME_RENDER_POS_X[0], ENDGAME_RENDER_POS_Y[0]);
                yellowSad.render(mRenderer, ENDGAME_RENDER_POS_X[1], ENDGAME_RENDER_POS_Y[1]);
                witherAway.render(mRenderer, ENDGAME_RENDER_POS_X[2], ENDGAME_RENDER_POS_Y[2], 
                               &witherAway.clips[(frameCount / FRAME_DELAY_WITHERAWAY_EMOJI) % FRAME_COUNT_WITHERAWAY_EMOJI]);
            } 
        }
        
        SDL_RenderPresent(mRenderer); 

        ++frameCount;
        if (frameCount > 1e8) frameCount = 0;
    }
    clear();
}

void playgamePage::clear()
{
    likeEmoji.clear();
    sunglasses.clear();
    witherAway.clear();
    yellowSad.clear();
    theRock.clear();
    zhongXina.clear();

    mKeyboard.clear();
    
    Mix_FreeMusic(yeahSound);
    Mix_FreeMusic(wrongAnswer);
    Mix_FreeMusic(spectre);
    Mix_FreeMusic(alarm);

    SDL_DestroyRenderer(mRenderer); mRenderer = NULL;
    SDL_DestroyWindow(mWindow); mWindow = NULL;

    IMG_Quit();
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}