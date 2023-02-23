//game.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "keyboard.h"

const int GUESS_WORD_POSITION_X = 500;
const int GUESS_WORD_POSITION_Y = 100;
const int GUESS_WORD_FONT_SIZE = 60;

const int LIVES_LEFT_BOX_POS_X = 500;
const int LIVES_LEFT_BOX_POS_Y = 320;
const int LIVES_LEFT_BOX_FONT_SIZE = 24;

const int HINT_BOX_POS_Y = 320;
const int HINT_BOX_POS_X[4] = {0, 650, 800, 950};
const int HINT_BOX_FONT_SIZE = 24;

const std::string KEYBOARD_ROWS[3] = {"QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"};
const int KEYBOARD_POSITION_X = 500;
const int KEYBOARD_POSITION_Y = 400;
const int KEYBOARD_CHARACTER_FONT_SIZE = 48;

const int HINT_TEXT_POSITION_X = 50;
const int HINT_TEXT_POSITION_Y = 600;
const int HINT_TEXT_FONT_SIZE = 24;

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

        /// @brief Buttons to trigger hints
        hintButton hintBox[4];

        /// @brief The texture object of the hint boxes
        LTexture hintBoxTexture[4];

        /// @brief The texture object of the hint line
        LTexture hintTextTexture;

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

        /// @brief Initialize the hint box texture
        /// @return 1 if successful, 0 otherwise
        bool loadHintBoxTexture();

        /// @brief Load the images
        /// @return 1 if successful, 0 otherwise
        bool loadImages();

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