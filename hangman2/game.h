#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "charButton.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const std::string PATH_COMIC_FONT = "fonts/comic.ttf";
const std::string PATH_YEAHSOUND_SOUND = "sounds/yeahsound.mp3";
const std::string PATH_WRONGANSWER_SOUND = "sounds/wrongAnswer.mp3";
const std::string PATH_SPECTRE_SOUND = "sounds/spectre.mp3";
const std::string PATH_ALARM_SOUND = "sounds/alarm.mp3";

const SDL_Color SDL_COLOR_BLACK = {0, 0, 0};
const SDL_Color SDL_COLOR_GRAY = {192, 192, 192};

const int GUESS_WORD_POSITION_X = 500;
const int GUESS_WORD_POSITION_Y = 100;

const int LIVES_LEFT_BOX_POS_X = 500;
const int LIVES_LEFT_BOX_POS_Y = 320;

const std::string KEYBOARD_ROWS[3] = {"QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"};
const int KEYBOARD_POSITION_X = 500;
const int KEYBOARD_POSITION_Y = 400;
const int KEYBOARD_CHARACTER_FONT_SIZE = 48;

class game
{
    private:
        /// @brief The window
        SDL_Window* mWindow;

        /// @brief The renderer
        SDL_Renderer* mRenderer;

        /// @brief The font object
        TTF_Font* mFont;

        /// @brief The texture object of the word need guessing
        LTexture guessWordTexture;

        /// @brief The texture object of the life box
        LTexture livesLeftBoxTexture;

        /// @brief The texture create from characters from A to Z
        LTexture charTexture[128];

        /// @brief The texture create from characters from A to Z, but after used
        LTexture usedCharTexture[128];

        /// @brief Buttons on the on-screen keyboard
        charButton button[128];

        LTexture likeEmoji;

        /// @brief Sound object for "yeahSound.mp3"
        Mix_Music* yeahSound;

        /// @brief Sound object for "wrongAnswer.mp3";
        Mix_Music* wrongAnswer;
        
        /// @brief Sound object for "spectre.mp3"
        Mix_Music* spectre;

        /// @brief Sound object for "alarm.mp3"
        Mix_Music* alarm;

        /// @brief The dictionary of the game, contains words from different difficulties
        vector <word> dictionary[DIFFICULTY_COUNT];
    
    public: 
        /// @brief Constructor
        game();

        /// @brief Destructor
        ~game();

        /// @brief Initialize SDL window, renderer, image
        /// @return 1 if successful, 0 otherwise
        bool initSDL();

        /// @brief Load the texture for the on-screeen keyboard
        /// @return 1 if successful, 0 otherwise
        bool loadCharTexture();

        bool loadLikeEmoji();

        /// @brief Load the sounds
        /// @return 1 if successful, 0 otherwise
        bool loadSound();

        /// @brief Render some text to the screen
        /// @param texture The LTexture object
        /// @param x,y The render position
        /// @param text The text being rendered, in char* form
        /// @param fontPath Path to the font (ttf) file
        /// @param fontSize Font size
        /// @param textColor Text color, in SDL_Color form
        void renderText(LTexture& texture, const char* text, const int& x, const int& y, const int& fontSize = 48, const char* fontPath = &PATH_COMIC_FONT[0], const SDL_Color& fontColor = SDL_COLOR_BLACK);

        /// @brief Settings before playing the game
        void preset();

        /// @brief The main gameplay
        void play();

        /// @brief Cleanup
        void clear();
};