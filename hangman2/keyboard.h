// keyboard.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "hintButton.h"

/// @brief Keyboard class
class keyboard
{
    public:
         /// @brief Buttons on the on-screen keyboard
        charButton keyboardButton[128];

        /// @brief The texture create from characters from A to Z
        LTexture charTexture[128];

        /// @brief The texture create from characters from A to Z, but after used
        LTexture usedCharTexture[128];

        /// @brief Load the texture for the on-screeen keyboard
        /// @return 1 if successful, 0 otherwise
        bool loadCharTexture(SDL_Renderer* mRenderer);

        /// @brief Setup the keyboard
        void set();

        /// @brief Render the keyboard
        void render(SDL_Renderer* renderer);
};
