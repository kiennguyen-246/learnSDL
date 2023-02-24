// keyboard.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "init.h"

/// @brief Keyboard class
class keyboard
{
    private:
         /// @brief Buttons on the on-screen keyboard
        charButton keyboardButton[128];

        /// @brief The texture create from characters from A to Z
        LTexture charTexture[128];

        /// @brief The texture create from characters from A to Z, but after used
        LTexture usedCharTexture[128];

    public:
        keyboard();

        ~keyboard();

        /// @brief Load the texture for the on-screeen keyboard
        /// @return 1 if successful, 0 otherwise
        bool loadCharTexture(SDL_Renderer* mRenderer);

        /// @brief Setup the keyboard
        void set();

        /// @brief Handle SDL_Event's. If is SDL_EVENT_MOUSEBUTTONUP, change the guessword
        /// @param event The SDL_Event
        /// @param key The answer for the word need guessing, together with its hints
        /// @param guessWord The word need guessing
        /// @param isTriggered Check if the button is clicked
        /// @param isIn Check if the current button's letter is in the word
        void handleEvent(SDL_Event& event, const word& key, string& guessWord, bool& isTriggered, bool& isIn);

        /// @brief Render the keyboard
        void render(SDL_Renderer* renderer);

        void clear();
};
