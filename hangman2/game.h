//game.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "playgamePage.h"

const int STARTPAGE_TITLE_RENDER_POS_X = 400;
const int STARTPAGE_TITLE_RENDER_POS_Y = 100;
const int STARTPAGE_TITLE_FONT_SIZE = 90;

const int ENDGAME_RENDER_POS_X[3] = {1000, 1000, 500};
const int ENDGAME_RENDER_POS_Y[3] = {400, 150, 400};

class game
{
    private:
        /// @brief The window
        SDL_Window* mWindow;

        /// @brief The renderer
        SDL_Renderer* mRenderer;

        /// @brief The title of the startpage
        LTexture startPageTitle;

        /// @brief Class for the main page of the game
        playgamePage mPlaygamePage;
    
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

        /// @brief The startpage of the game
        // void startPage();

        /// @brief The main gameplay
        void play();

        /// @brief Cleanup
        void clear();
};