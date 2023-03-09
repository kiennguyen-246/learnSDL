//base.h

/**
 * This file will define some base classes and constants for the game
 * 
*/

#ifndef BASE_GUARD
#define BASE_GUARD

#include <iostream>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int GAMEPLAY_AREA_POS_X = 0;
const int GAMEPLAY_AREA_POS_Y = 0;
const int GAMEPLAY_AREA_WIDTH = 1280;
const int GAMEPLAY_AREA_HEIGHT = 640;

const SDL_Color SDL_COLOR_BLACK = {0, 0, 0};
const SDL_Color SDL_COLOR_MALIBU = {81, 218, 254};

const std::string SPRITESHEET_PATH = "./img/spritesheet.png";

/// @brief Texture class
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
        bool loadTexture(SDL_Renderer* mRenderer, const char* path, const SDL_Color& colorKey = SDL_COLOR_BLACK);

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
        /// @param angle Rotation angle
        /// @param center Rotation center
        /// @param flip Flip flag
        void render(SDL_Renderer* mRenderer, const int& x, const int& y, SDL_Rect* clip = NULL, const double& angle = 0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        /// @brief Destroy the LTexture object
        void clear();
};

/// @brief Button class
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

        /// @brief Handle the SDL events
        virtual void handleEvent(SDL_Event event) = 0;
};

class gameObject
{
protected:
    /// @brief The BOTTOM LEFT position of the object
    int mPosX, mPosY;

    int mWidth, mHeight;

    SDL_Rect mSpriteClip;

public:
    gameObject();

    ~gameObject();

    void setPos(const int& x, const int& y);

    void setPos(const std::pair <int, int>& __Pos);

    void setSize(const int& w, const int& h);

    int getPosX() const;

    int getPosY() const;

    /// @brief Get the clip for the sprite of the object
    /// @param spritesheet 
    void setSpriteClip(LTexture& spritesheet, const int& x, const int& y, const int& w, const int& h);

    virtual void render(SDL_Renderer* renderer, LTexture& spritesheet) = 0;

    friend bool collide(const gameObject& obj1, const gameObject& obj2);
};

/// @brief Check if two objects "touch" each other by their hitbox
/// @return 1 if obj1 touches obj2, 0 otherwise
bool collide(const gameObject& obj1, const gameObject& obj2);

#endif