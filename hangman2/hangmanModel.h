// hangmanModel.h
#ifndef HANGMAN_MODEL_H
#define HANGMAN_MODEL_H

#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "livesBox.h"

const int HANGMAN_STATES_COUNT = 9;

const std::string PATH_RACK = "img/rack.png";
const std::string PATH_TROLL_HANGMAN[HANGMAN_STATES_COUNT] = 
{
    "img/troll0.png",
    "img/troll1.png",
    "img/troll2.png",
    "img/troll3.png",
    "img/troll4.png",
    "img/troll5.png",
    "img/troll6.png",
    "img/troll7.png",
    "img/trollIntro.png",
};

const int RACK_POS_X = 20;
const int RACK_POS_Y = 20;
const int HANGMAN_POS_X = 70;
const int HANGMAN_POS_Y = 110;

class hangmanModel
{
    private:
        /// @brief Texture object of the hangman rack
        LTexture rack;

        /// @brief Texture object of the hangman
        LTexture trollHangman[HANGMAN_STATES_COUNT];
    
    public:
        /// @brief Default constructor
        hangmanModel();

        /// @brief Default destructor
        ~hangmanModel();

        /// @brief Initialize the textures
        void init(SDL_Renderer* renderer);

        /// @brief Render the hangman
        /// @param renderer 
        void render(SDL_Renderer* renderer, const int& livesConsumed);

        /// @brief Cleanup
        void clear();

};

#endif