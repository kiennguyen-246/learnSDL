// startPage.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "hangmanModel.h"

enum START_PAGE_STATUS
{
    START_PAGE_NULL,
    START_PAGE_PLAY,
    START_PAGE_INSTRUCTION,
    START_PAGE_ABOUT,
    START_PAGE_QUIT
};

const int START_PAGE_TITLE_RENDER_POS_X = 400;
const int START_PAGE_TITLE_RENDER_POS_Y = 100;
const int START_PAGE_TITLE_FONT_SIZE = 90;

const int START_BUTTON_POS_X = 550;
const int START_BUTTON_POS_Y = 300;
const int START_BUTTON_FONT_SIZE = 60;

class startButton: public LButton
{
    public:
        /// @brief Texture of the START button
        LTexture sprite;

        /// @brief Setup the button
        void init(SDL_Renderer* renderer);

        /// @brief Handle the SDL events
        void handleEvent(SDL_Event* event, START_PAGE_STATUS& returnStatus);

        /// @brief Change the return status of the page to determine what the next page is
        /// @param returnStatus See the enum START_PAGE_STATUS
        void trigger(START_PAGE_STATUS& returnStatus);
};

class startPage
{
    private:
        /// @brief The window
        SDL_Window* mWindow;

        /// @brief The renderer
        SDL_Renderer* mRenderer;
        
        /// @brief Game title
        LTexture titleTexture;

        /// @brief The START button
        startButton mStartButton;

    public:
        /// @brief Default constructor
        startPage(/* args */);

        /// @brief Default destructor
        ~startPage();

        /// @brief Load resources
        void preset(SDL_Window* window, SDL_Renderer* renderer);

        /// @brief Start the start page
        void startStartPage(START_PAGE_STATUS& returnStatus);

        /// @brief Cleanup
        void clear();
};


