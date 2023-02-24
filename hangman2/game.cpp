//game.cpp
#include <iostream>
#include <vector>
#include <string>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "game.h"

using std::cout;
using random::randInt;

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

    mWindow = SDL_CreateWindow("hangman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

bool game::loadSound()
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

bool game::loadImages()
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

    if (!whitherAway.loadTexture(mRenderer, &PATH_WHITHERAWAY_EMOJI[0]))
    {
        cout << "Failed to load image.\n"; 
        return 0;
    }

    if (!yellowSad.loadTexture(mRenderer, &PATH_YELLOWSAD_EMOJI[0]))
    {
        cout << "Failed to load image.\n"; 
        return 0;
    }
    return 1;
}

GAME_DIFFICULTY game::setDifficulty(const int& __dif)
{
    difficulty = GAME_DIFFICULTY(__dif);
}

bool game::victory()
{
    return mGuessWord.victory();
}

bool game::defeat()
{
    return mLivesBox.defeat();
}

void game::preset()
{
    random::initRand();
    
    if (!initSDL())
    {
        cout << "SDL initialization failed.\n";
        return;
    }

    if (!loadImages())
    {
        cout << "Loading images failed.\n";
        return;
    }

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

void game::play()
{
    preset();
    
    bool quit = 0;
    SDL_Event curEvent;

    //Set the difficulty
    setDifficulty(randInt(1, 3));

    //Get the answer key
    key = mDictionary.getWord(difficulty);

    //Hidden word setup
    mGuessWord.init(key);
    
    //Set the number of lives
    mLivesBox.set(difficulty);

    bool gameOver = 0;

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
            }
            //Game over
            if (this->defeat())
            {
                whitherAway.render(mRenderer, ENDGAME_RENDER_POS_X[0], ENDGAME_RENDER_POS_Y[0]);
                yellowSad.render(mRenderer, ENDGAME_RENDER_POS_X[1], ENDGAME_RENDER_POS_Y[1]);
            } 
        }
        

        SDL_RenderPresent(mRenderer); 
    }
    clear();
}

void game::clear()
{
    likeEmoji.clear();
    sunglasses.clear();
    whitherAway.clear();
    yellowSad.clear();
    
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