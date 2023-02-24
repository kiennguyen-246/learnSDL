// livesBox.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "hintBox.h"

const int LIVES_LEFT_BOX_POS_X = 500;
const int LIVES_LEFT_BOX_POS_Y = 320;
const int LIVES_LEFT_BOX_FONT_SIZE = 24;

/// @brief Class object for the "Lives left: x" box
class livesBox
{
    private:
        /// @brief The texture object of the life box
        LTexture livesLeftBoxTexture;

        /// @brief The number of lives left
        int livesLeft;

        /// @brief The number of lives consumed
        int livesConsumed;

    public:
        /// @brief Default constructor
        livesBox();

        /// @brief Default destructor
        ~livesBox();

        /// @brief Set up maximum lives based on difficulty
        void set(const int& difficulty);

        /// @brief Check if the game ends when no life left or not
        /// @return 1 if game ends, 0 otherwise
        bool defeat() const;

        /// @brief Consume 1 life
        void consumeLife();

        /// @brief Get the number of lives consumed
        int getLivesConsumed() const;

        /// @brief Render the lives left box
        void render(SDL_Renderer* renderer);
        
        /// @brief Cleanup
        void clear();
};