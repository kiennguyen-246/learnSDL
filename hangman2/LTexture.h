//LTexture.h
#include <iostream>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

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
        /// @param mRenderer SDL_Renderer
        /// @param path The directory of the file containing the texture
        bool loadTexture(SDL_Renderer* mRenderer, const char* path);

        /// @brief Create texture from text
        /// @param mRenderer SDL_Renderer
        /// @param mFont TTF_Font
        /// @param text The text being rendered
        /// @param textColor The text color
        /// @return 
        bool loadTexture(SDL_Renderer* mRenderer, TTF_Font* mFont, const char* text, const SDL_Color& textColor);

        /// @brief Render the texture on the window
        /// @param mRenderer SDL_Renderer
        /// @param x,y 2 integers, represent the upper left position
        /// @param clip SDL_Rect, represent the part of the texture to be render. NULL means all.
        void render(SDL_Renderer* mRenderer, const int& x, const int& y, SDL_Rect* clip = NULL);

        /// @brief Render the texture on the window
        /// @param mRenderer SDL_Renderer
        /// @param x,y 2 integers, represent the upper left position
        /// @param clip SDL_Rect, represent the part of the texture to be render. NULL means all.
        /// @param angle Rotation angle
        /// @param center Rotation center
        /// @param flip Flip flag
        void render(SDL_Renderer* mRenderer, int& x, int& y, SDL_Rect* clip = NULL, const double& angle = 0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        /// @brief Destroy the LTexture object
        void clear();
        
};