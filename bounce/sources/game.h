//game.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "base.h"
#include "playLevel.h"

/**
 * @brief This file will control the working logic of the game, like starting page, levels
 * 
 */

class game
{
    private:
        /// @brief The window
        SDL_Window* mWindow;

        /// @brief The renderer
        SDL_Renderer* mRenderer;

        /// @brief The spritesheet of the game
        LTexture mSpritesheet;
    
    public: 
        /// @brief Constructor
        game();

        /// @brief Destructor
        ~game();

        /// @brief Initialize SDL window, renderer, image
        /// @return 1 if successful, 0 otherwise
        bool initSDL();

        /// @brief Settings before playing the game
        void preset();

        /// @brief The main gameplay
        void play();

        /// @brief Cleanup
        void clear();
};