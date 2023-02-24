// hintBox.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "keyboard.h"

enum HINT_PROPERTIES
{
    HINT_NULL,
    HINT_1,
    HINT_2,
    HINT_3,
    HINT_COUNT
};

const int HINT_BOX_POS_Y = 320;
const int HINT_BOX_POS_X[HINT_COUNT] = {0, 650, 800, 950};
const int HINT_BOX_FONT_SIZE = 24;

const int HINT_TEXT_POSITION_X = 50;
const int HINT_TEXT_POSITION_Y = 600;
const int HINT_TEXT_FONT_SIZE = 24;

/// @brief Class for the hint buttons
class hintButton: public LButton
{
    private:
        /// @brief Index of the button, can be 1, 2 or 3
        int id;
    
    public:
        /// @brief Default constructor
        hintButton();

        /// @brief Set up the index for the button
        void setId(const int& newId);

        /// @brief Handke SDL_Event's
        void handleEvent(SDL_Event* event, const word& __word, std::string& hint);

        /// @brief After clicking
        void trigger(const word& __word, string& hint);
};


class hintBox
{
    private:
         /// @brief Buttons to trigger hints
        hintButton mHintButton[HINT_COUNT];

        /// @brief The texture object of the hint boxes
        LTexture hintButtonTexture[HINT_COUNT];

        /// @brief The texture object of the hint line
        LTexture hintTextTexture;

        /// @brief The hint text
        std::string hintText;
    
    public:
        /// @brief Default constructor
        hintBox();

        /// @brief Default destructor
        ~hintBox();

        /// @brief Initialize the hint box texture
        /// @return 1 if successful, 0 otherwise
        bool loadHintButtonTexture(SDL_Renderer* renderer);  

        /// @brief Setup the hint buttons
        void setButtons();

        /// @brief Setup hintText
        void setText(const std::string& text);

        /// @brief Handle an SDL_Event. If being a MOUSE_BUTTONUP, render hints form key
        /// @param key Word object
        void handleEvent(SDL_Event& event, const word& key);

        /// @brief Render the hint boxes
        void renderHintBox(SDL_Renderer* renderer, const int& liveConsumed);
        
        /// @brief Render the hint text
        void renderHintText(SDL_Renderer* renderer);

        /// @brief Cleanup
        void clear();
};