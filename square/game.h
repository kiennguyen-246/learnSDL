#include <iostream>
#include <sdl.h>
#include <SDL_image.h>
#include "LTexture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const double MOVE_STEP = 10.3333;

class game
{
    private:
        SDL_Window* gWindow = NULL;
        SDL_Renderer* gRenderer = NULL;
    
    public: 
        /// @brief Constructor
        game();

        /// @brief Destructor
        ~game();

        /// @brief Initialize SDL window, renderer, image
        /// @return 1 if successful, 0 otherwise
        bool init();

        /// @brief Load the images
        /// @param redSquare,blueSquare,likeEmoji Texture object of the red the blue square and the like emoji
        /// @return 1 if successful, 0 otherwise
        bool loadMedia(LTexture& redSquare, LTexture& blueSquare, LTexture& likeEmoji);

        /// @brief The main gameplay
        void play();

        /// @brief Cleanup
        void clear();
};