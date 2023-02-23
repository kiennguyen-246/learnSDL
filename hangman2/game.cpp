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

bool game::loadCharTexture()
{
    mFont = TTF_OpenFont(&PATH_COMIC_FONT[0], KEYBOARD_CHARACTER_FONT_SIZE);
	if (mFont == NULL)
	{
		cout << "Failed to load the font. Error: " << TTF_GetError() << ".\n";
		return 0;
	}
    string charList = KEYBOARD_ROWS[0] + KEYBOARD_ROWS[1] + KEYBOARD_ROWS[2] + "_ ";

    for (char curChar : charList)
    {
        string curChar_str = "";
        curChar_str.push_back(curChar);
        if ((!(charTexture[int(curChar)].loadTexture(mRenderer, mFont, &curChar_str[0], SDL_COLOR_BLACK))) || 
            (!usedCharTexture[int(curChar)].loadTexture(mRenderer, mFont, &curChar_str[0], SDL_COLOR_GRAY)))
        {
            cout << "Failed to render text.\n";
            return 0;
        }
    }

	TTF_CloseFont(mFont);

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

bool game::loadLikeEmoji()
{
    likeEmoji.loadTexture(mRenderer, "img/likeEmoji.png");
    return 1;
}

void game::renderText(LTexture& texture, const char* text, const int& x, const int& y, 
                      const int& fontSize, const char* fontPath, const SDL_Color& fontColor)
{
    mFont = TTF_OpenFont(fontPath, fontSize);

    if (!texture.loadTexture(mRenderer, mFont, text, fontColor)) 
    {
        cout << "Cannot render given text.\n";
    }
    else
    {
        texture.render(mRenderer, x, y);
    }
    TTF_CloseFont(mFont);
}

void game::preset()
{
    initRand();
    initWords(dictionary);
    
    if (!initSDL())
    {
        cout << "Initialization failed.\n";
        return;
    }

    if (!loadCharTexture())
    {
        cout << "Loading alphabet characters texture failed.\n";
        return;
    }

    if (!loadSound())
    {
        cout << "Loading soundtracks failed.\n";
        return;
    }

    loadLikeEmoji();

    int charHeight = charTexture['A'].getHeight();
    cout << charHeight << "\n";
    int curXPos = KEYBOARD_POSITION_X;
    int curYpos = KEYBOARD_POSITION_Y;

    for (int curRow = 0; curRow < 3; curRow ++)
    {
        curXPos = KEYBOARD_POSITION_X;
        for (auto ch : KEYBOARD_ROWS[curRow])
        {
            int ch_int = ch;
            button[ch_int].set(curXPos, curYpos, charTexture[ch_int].getWidth(), charTexture[ch_int].getHeight());
            curXPos += charTexture[ch_int].getWidth() + 2 * charTexture[int(' ')].getWidth();

            button[ch_int].updateSymbol(ch);
        }
        curYpos += charHeight;
    }
}

void game::play()
{
    preset();
    
    bool quit = 0;
    SDL_Event curEvent;

    word key = dictionary[DIFFICULTY_EASY][randInt(0, dictionary[DIFFICULTY_EASY].size() - 1)];
    string guessWord = "";
    for (int i = 0; i < key.getLength(); i ++) guessWord.push_back('_'); 
    bool isTriggered = 0;
    bool isIn = 0;
    int livesLeft = LIVES_COUNT_DEFAULT;
    int livesConsumed = 0;
    bool gameOver = 0;
    bool correct = 0;
    bool incorrect = 0;
    bool victory = 0;
    bool defeat = 0;

    while (!quit)
    {
        isTriggered = 0;
        isIn = 0;

        if (!gameOver)
        {
            correct = 0;
            incorrect = 0;
            victory = 0;
            defeat = 0;
        }
        
        while (SDL_PollEvent(&curEvent) != 0)
        {
            if (curEvent.type == SDL_QUIT) quit = 1;
            else 
            {
                for (int buttonId = 'A'; buttonId <= 'Z'; buttonId ++)
                    button[buttonId].handleEvent(&curEvent, key, guessWord, isTriggered, isIn);
            }
        }

        SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
        SDL_RenderClear(mRenderer);

        //A button is pressed
        if (isTriggered)
        {
            if (isIn) 
            {
                correct = 1;
                Mix_PlayMusic(yeahSound, 0);
            }
            else 
            {
                incorrect = 1;
                livesLeft --;
                livesConsumed ++;
                Mix_PlayMusic(wrongAnswer, 0);
            }
            if (livesLeft == 0) 
            {
                gameOver = 1;  
                defeat = 1;
                Mix_PlayMusic(alarm, -1);
            }
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
        }
        //Game over
        if (defeat)
        {
            
        } 

        string livesLeftInfo = "Lives left: " + std::to_string(livesLeft);
        renderText(livesLeftBoxTexture, &livesLeftInfo[0], LIVES_LEFT_BOX_POS_X, LIVES_LEFT_BOX_POS_Y, 24);

        string spacedGuessWord = spaced(guessWord);
        renderText(guessWordTexture, &spacedGuessWord[0], GUESS_WORD_POSITION_X, GUESS_WORD_POSITION_Y, 60);

        for (int curRow = 0; curRow < 3; curRow ++)
        {
            for (auto ch : KEYBOARD_ROWS[curRow])
            {
                int ch_int = ch;
                if (!button[ch_int].isUsed()) button[ch_int].render(mRenderer, charTexture[ch_int]);
                else button[ch_int].render(mRenderer, usedCharTexture[ch_int]);
            }
        }

        SDL_RenderPresent(mRenderer);
        
        
    }

    
    clear();
}

void game::clear()
{
    for (int i = 'A'; i <= 'Z'; i ++) charTexture[i].clear();
    charTexture[int(' ')].clear();
    charTexture[int('_')].clear();
    
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