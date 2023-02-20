//square.h
#include <iostream>
#include <sdl.h>
#include <SDL_image.h>

class LTexture
{
    private:
        SDL_Texture* mTexture = NULL;
        int mWidth = 0;
        int mHeight = 0;
    
    public:
        /// @brief Constructor
        LTexture();

        /// @brief Destructor
        ~LTexture();

        /// @brief Get the width of the texture
        /// @return An integer represents the width of the texture
        int getWidth();

        /// @brief Get the height of the texture
        /// @return An integer represents the height of the texture
        int getHeight();

        /// @brief Load texture from file
        /// @param gRenderer Global SDL_Renderer
        /// @param path The directory of the file containing the texture
        bool loadTexture(SDL_Renderer* gRenderer, char* path);


        /// @brief Render the texture on the window
        /// @param gRenderer Global SDL_Renderer
        /// @param x,y 2 integers, represent the upper left position
        /// @param clip SDL_Rect, represent the part of the texture to be render. NULL means all.
        void render(SDL_Renderer* gRenderer, const int& x, const int& y, SDL_Rect* clip = NULL);

        /// @brief Destroy the LTexture object
        void clear();
        
};