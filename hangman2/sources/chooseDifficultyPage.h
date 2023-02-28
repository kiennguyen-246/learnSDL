// chooseDifficultyPage.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "startPage.h"

const int CHOOSE_DIFFICULTY_PAGE_TITLE_POS_X = 250;
const int CHOOSE_DIFFICULTY_PAGE_TITLE_POS_Y = 50;
const int CHOOSE_DIFFICULTY_PAGE_TITLE_FONT_SIZE = 60;

const int DIFFICULTY_BUTTON_POS_X[DIFFICULTY_COUNT] = {0, 550, 550, 550, 550};
const int DIFFICULTY_BUTTON_POS_Y[DIFFICULTY_COUNT] = {0, 150, 250, 350, 450};
const int DIFFICULTY_BUTTON_FONT_SIZE = 48;


class difficultyButton: public LButton
{
    private:
        /// @brief Difficulty mark for current button
        GAME_DIFFICULTY difficulty;
    
    public:
        /// @brief Sprite texture
        LTexture sprite;

        /// @brief Initialize the button
        void init(SDL_Renderer* renderer, const GAME_DIFFICULTY& __difficulty);

        /// @brief Handle the SDL_Event's
        void handleEvent(SDL_Event* event, GAME_DIFFICULTY& returnDifficulty);

        /// @brief Get the difficulty after clicking the button
        /// @param returnDifficulty See GAME_DIFFICULTY
        void trigger(GAME_DIFFICULTY& returnDifficulty);
};

class chooseDifficultyPage
{
    private:
        /// @brief The window
        SDL_Window* mWindow;

        /// @brief The renderer
        SDL_Renderer* mRenderer;
        
        /// @brief Game title
        LTexture titleTexture;

        /// @brief The 4 difficulty buttons
        difficultyButton mDifficultyButton[DIFFICULTY_COUNT];

        /// @brief The 2 intro images
        LTexture trollIntro;
    public:
        /// @brief Default constructor
        chooseDifficultyPage(/* args */);

        /// @brief Default destructor
        ~chooseDifficultyPage();

        /// @brief Load resources
        void preset(SDL_Window* window, SDL_Renderer* renderer);

        /// @brief Start the choose difficulty page
        /// @param difficulty The difficulty of the game
        void startChooseDifficultyPage(GAME_DIFFICULTY &difficulty);

        /// @brief Cleanup
        void clear();

        
};