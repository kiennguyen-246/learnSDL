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

    if (!shack.loadTexture(mRenderer, &PATH_SHACK[0]))
    {
        cout << "Failed to load image.\n"; 
        return 0;
    }

    for (int state = 0; state < HANGMAN_STATES_COUNT; state ++)
    {
        if (!trollHangman[state].loadTexture(mRenderer, &PATH_TROLL_HANGMAN[state][0]))
        {
            cout << "Failed to load image.\n"; 
            return 0;
        }
    }
    return 1;
}

void game::preset()
{
    initRand();
    initWords(dictionary);
    
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
}

void game::play()
{
    preset();
    
    bool quit = 0;
    SDL_Event curEvent;

    int difficulty = randInt(1, 3);
    word key = dictionary[difficulty][randInt(0, dictionary[difficulty].size() - 1)];
    string guessWord = "";
    string hintText = "";
    for (int i = 0; i < key.getLength(); i ++) guessWord.push_back('_'); 
    bool keyboardTriggered = 0;
    bool isIn = 0;
    int livesLeft = LIVES_COUNT_DEFAULT;
    int livesConsumed = 0;
    bool gameOver = 0;
    bool victory = 0;
    bool defeat = 0;

    while (!quit)
    {
        keyboardTriggered = 0;
        isIn = 0;

        if (!gameOver)
        {
            victory = 0;
            defeat = 0;
        }
        
        while (SDL_PollEvent(&curEvent) != 0)
        {
            if (curEvent.type == SDL_QUIT) quit = 1;
            else 
            {
                mKeyboard.handleEvent(curEvent, key, guessWord, keyboardTriggered, isIn);
                mHintBox.handleEvent(curEvent, key);
            }
        }

        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);

        //Render the word need guessing
        string spacedGuessWord = spaced(guessWord);
        renderText(mRenderer, guessWordTexture, &spacedGuessWord[0], GUESS_WORD_POSITION_X, GUESS_WORD_POSITION_Y, GUESS_WORD_FONT_SIZE);

        //Render "Lives left" box
        string livesLeftInfo = "Lives left: " + std::to_string(livesLeft);
        renderText(mRenderer, livesLeftBoxTexture, &livesLeftInfo[0], LIVES_LEFT_BOX_POS_X, LIVES_LEFT_BOX_POS_Y, LIVES_LEFT_BOX_FONT_SIZE);

        //Render the hint boxes
        mHintBox.renderHintBox(mRenderer, livesConsumed);

        //Render the keyboard
        mKeyboard.render(mRenderer);

        //Render the hint text
        mHintBox.renderHintText(mRenderer);

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
                livesLeft --;
                livesConsumed ++;
                Mix_PlayMusic(wrongAnswer, 0);
            }
            //Game ends when no lives left
            if (livesLeft == 0) 
            {
                gameOver = 1;  
                defeat = 1;
                Mix_PlayMusic(alarm, -1);
            }
            //Game ends when the word is successfully guessed
            if (guessWord.find('_') == guessWord.npos) 
            {
                victory = 1;
                gameOver = 1;
                Mix_PlayMusic(spectre, -1);
            }
        }

        //Victory
        if (victory)
        {
            likeEmoji.render(mRenderer, 1000, 400);
            sunglasses.render(mRenderer, 1000, 200);
        }
        //Game over
        if (defeat)
        {
            whitherAway.render(mRenderer, 1000, 200);
            yellowSad.render(mRenderer, 1000, 400);
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
    shack.clear();
    for (int i = 0; i < HANGMAN_STATES_COUNT; i ++) trollHangman[i].clear();
    
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