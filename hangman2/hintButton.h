///hintButton.h
#include <iostream>
#include <SDL.h>
#include "charButton.h"

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