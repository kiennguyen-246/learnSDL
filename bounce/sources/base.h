//base.h

/**
 * This file will define some base classes and constants for the game
 * 
*/

#ifndef BASE_GUARD
#define BASE_GUARD

#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

typedef std::vector <std::vector <std::string> > vector_2d_string;
typedef std::vector <std::vector <std::pair <std::pair <int, int>, std::pair <int, int> > > > vector_2d_pair_of_pii;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

const int GAMEPLAY_AREA_POS_X = 0;
const int GAMEPLAY_AREA_POS_Y = 0;
const int GAMEPLAY_AREA_WIDTH = 1280;
const int GAMEPLAY_AREA_HEIGHT = 640;

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;

const int STATUS_AREA_POS_X = 0;
const int STATUS_AREA_POS_Y = GAMEPLAY_AREA_HEIGHT;
const int STATUS_AREA_WIDTH = 1280;
const int STATUS_AREA_HEIGHT = 640;
const int STATUS_AREA_FONT_SIZE = 72;

const SDL_Color SDL_COLOR_BLACK = {0, 0, 0};
const SDL_Color SDL_COLOR_WHITE = {255, 255, 255};
const SDL_Color SDL_COLOR_MALIBU = {81, 218, 254};

const std::string SPRITESHEET_PATH = "./img/spritesheet.png";
const std::string CALIBRI_FONT_PATH = "./fonts/calibri.ttf";
const std::string MAPCONFIG_PATH = "./data/mapConfig";

const int LIVES_LEFT_DEFAULT = 3;

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
        int getWidth() const;

        /// @brief Get the height of the texture
        /// @return An integer represents the height of the texture
        int getHeight()const ;

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
        void render(SDL_Renderer* mRenderer, const int& x, const int& y, SDL_Rect* clip = NULL, const int& stretchSize = 1, 
                    const double& angle = 0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        /// @brief Destroy the LTexture object
        void clear();
};

/// @brief Button class
class LButton
{
    private:
        /// @brief Upper left position of the button
        SDL_Point mPos;

        int mWidth;
        int mHeight;
        
        /// @brief The texture rendered with the button
        LTexture mButtonTexture;
        SDL_Rect* mSpriteClipPtr;

        /// @brief The text rendered inside the button
        std::string mText;
        LTexture mTextTexture;

        bool textIsSetUp;

    public:
        /// @brief Constructor
        LButton();

        /// @brief Set the texture rendered with the button
        void setTexture(const LTexture& __Texture);

        /// @brief Set the text rendered inside the button
        void setText(SDL_Renderer* renderer, const std::string& __Text, const int& fontSize = 48, const char* fontPath = &CALIBRI_FONT_PATH[0], const SDL_Color& fontColor = SDL_COLOR_BLACK);

        /// @brief Set the upper left position, width and height of the button
        void set(const int& x, const int& y, const int& __w, const int& __h);
        
        /// @brief Set up a button with a given texture
        void set(const int& x, const int& y, const LTexture& __ButtonTexture);

        /// @brief Get the upper left position of the button
        SDL_Point getPos();
        /// @brief Get the width of the button
        int getWidth();

        /// @brief Get the height of the button
        int getHeight();

        /// @brief Render the button
        void render(SDL_Renderer* renderer);

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

    /// @brief Set the clip for the sprite of the object in the spritesheet
    /// @param spritesheet 
    void setSpriteClip(LTexture& spritesheet, const int& x, const int& y, const int& w, const int& h);

    /// @brief Set the clip for the sprite of the object in the spritesheet
    SDL_Rect* getSpriteClipPtr();

    virtual void render(SDL_Renderer* renderer, LTexture& spritesheet) = 0;

    friend bool collide(const gameObject& obj1, const gameObject& obj2);
};

/// @brief Check if two objects "touch" each other by their hitbox
/// @return 1 if obj1 touches obj2, 0 otherwise
bool collide(const gameObject& obj1, const gameObject& obj2);

/// @brief Render some text to the screen
/// @param texture The LTexture object
/// @param x,y The render position
/// @param text The text being rendered, in char* form
/// @param fontPath Path to the font (ttf) file
/// @param fontSize Font size
/// @param textColor Text color, in SDL_Color form
void renderText(SDL_Renderer* renderer, LTexture& texture, const char* text, const int& x, const int& y, const int& fontSize = 48, const char* fontPath = &CALIBRI_FONT_PATH[0], const SDL_Color& fontColor = SDL_COLOR_BLACK);

struct spiderInfo
{
    int x1, y1, x2, y2, startPos;
};

typedef std::vector <std::vector <spiderInfo> > vector_2d_spiderInfo;

#endif