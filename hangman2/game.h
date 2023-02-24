//game.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "hangmanModel.h"

const int ENDGAME_RENDER_POS_X[3] = {1000, 1000, 500};
const int ENDGAME_RENDER_POS_Y[3] = {400, 150, 400};

class game
{
    private:
        /// @brief The window
        SDL_Window* mWindow;

        /// @brief The renderer
        SDL_Renderer* mRenderer;

        /// @brief Hidden word object
        guessWord mGuessWord;

        /// @brief Lives boxes object, including lives counter
        livesBox mLivesBox;

        /// @brief Hint boxes object, including 3 hint buttons and the hint text in the bottom of the screen
        hintBox mHintBox;

        /// @brief Keyboard object
        keyboard mKeyboard;

        /// @brief The hangman model object
        hangmanModel mHangmanModel;
        
        /// @brief Texture object for the images
        PNG_Image sunglasses;
        PNG_Image likeEmoji;
        PNG_Image zhongXina;
        PNG_Image yellowSad;
        GIF_Image witherAway;
        GIF_Image theRock;

        /// @brief Sound object for the soundtracks
        Mix_Music* yeahSound;
        Mix_Music* wrongAnswer;
        Mix_Music* spectre;
        Mix_Music* alarm;

        /// @brief The dictionary of the game, contains words from different difficulties
        dictionary mDictionary;

        /// @brief The difficulty of the game
        GAME_DIFFICULTY difficulty;

        /// @brief Answer key of the word need guessing
        word key;
    
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

        /// @brief Set the difficulty of the game
        GAME_DIFFICULTY setDifficulty(const int& __dif);

        /// @brief Game over when no spaces left
        bool victory();

        /// @brief Game over when no lives left
        bool defeat();

        /// @brief Settings before playing the game
        void preset();

        /// @brief The main gameplay
        void play();

        /// @brief Cleanup
        void clear();
};