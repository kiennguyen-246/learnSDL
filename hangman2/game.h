//game.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "hintBox.h"

const int GUESS_WORD_POSITION_X = 500;
const int GUESS_WORD_POSITION_Y = 100;
const int GUESS_WORD_FONT_SIZE = 60;

const int LIVES_LEFT_BOX_POS_X = 500;
const int LIVES_LEFT_BOX_POS_Y = 320;
const int LIVES_LEFT_BOX_FONT_SIZE = 24;

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

        /// @brief Hintboxes object, including 3 hint buttons and the hint text in the bottom of the screen
        hintBox mHintBox;

        /// @brief Keyboard object
        keyboard mKeyboard;
        
        /// @brief Texture object for the images
        LTexture sunglasses;
        LTexture likeEmoji;
        LTexture whitherAway;
        LTexture yellowSad;
        LTexture shack;
        LTexture trollHangman[HANGMAN_STATES_COUNT];

        /// @brief Sound object for the soundtracks
        Mix_Music* yeahSound;
        Mix_Music* wrongAnswer;
        Mix_Music* spectre;
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

        /// @brief Load the images
        /// @return 1 if successful, 0 otherwise
        bool loadImages();

        /// @brief Load the sounds
        /// @return 1 if successful, 0 otherwise
        bool loadSound();

        /// @brief Settings before playing the game
        void preset();

        /// @brief The main gameplay
        void play();

        /// @brief Cleanup
        void clear();
};