//LButton.h
#include <iostream>
#include <sdl.h>
#include "LTexture.h"
#include "init.h"

class LButton
{
    private:
        /// @brief Upper left position of the button
        SDL_Point mPos;
        int w;
        int h;

    public:
        /// @brief Constructor
        LButton();

        /// @brief Set the upper left position, width and height of the button
        void set(const int& x, const int& y, const int& __w, const int& __h);

        /// @brief Get the upper left position of the button
        SDL_Point getPos();

        /// @brief Get the width of the button
        int getWidth();

        /// @brief Get the height of the button
        int getHeight();

        /// @brief Render the button
        void render(SDL_Renderer* renderer, LTexture& texture, SDL_Rect* clip = NULL);
};