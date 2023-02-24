//init.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using std::cout;
using std::string;
using std::vector;

enum GAME_DIFFICULTY
{
    DIFFICULTY_NULL,
    DIFFICULTY_EASY,
    DIFFICULTY_NORMAL,
    DIFFICULTY_DIFFICULT,
    DIFFICLUTY_CHINA,
    DIFFICULTY_COUNT
};
const int LIVES_COUNT_DEFAULT = 7;
const int LIVES_COUNT_CHINA = 8;
const string UPPERCASE_LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int HANGMAN_STATES_COUNT = 9;

const SDL_Color SDL_COLOR_BLACK = {0, 0, 0};
const SDL_Color SDL_COLOR_GRAY = {192, 192, 192};

const std::string PATH_COMIC_FONT = "fonts/comic.ttf";
const std::string PATH_LIKE_EMOJI = "img/likeEmoji.png";
const std::string PATH_SUNGLASSES_EMOJI = "img/sunglasses.png";
const std::string PATH_WHITHERAWAY_EMOJI = "img/whitheraway.png";
const std::string PATH_YELLOWSAD_EMOJI = "img/yellowSad.png";
const std::string PATH_SHACK = "img/shack.png";
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

const std::string PATH_YEAHSOUND_SOUND = "sounds/yeahsound.mp3";
const std::string PATH_WRONGANSWER_SOUND = "sounds/wrongAnswer.mp3";
const std::string PATH_SPECTRE_SOUND = "sounds/spectre.mp3";
const std::string PATH_ALARM_SOUND = "sounds/alarm.mp3";

const std::string PATH_DICTIONARY = "dictionary.csv";

/// @brief Random numbers generating functions
namespace random
{
    /// @brief Random integer in [0, limit]
    int randInt(const int& limit);

    /// @brief Random integer in [lLimit, rLimit]
    int randInt(const int& lLimit, const int& rLimit);

    /// @brief rand() setup
    void initRand();
};

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
};
/// @brief Word class, including the word itself, the hints for guessing
class word
{
    private:
        string value;
        string hint1;
        string hint2;
        string hint3;
        int len;
    
    public:
        /// @brief Default constructor
        word();

        /// @brief Constructor
        /// @param __value The word
        /// @param __hint1 The first hint
        /// @param __hint2 The second hint
        /// @param __hint3 The third hint
        word(const string& __value, const string& __hint1, const string& __hint2, const string& __hint3);

        ///Destructor
        ~word();

        /// @brief Get the key word
        /// @return A string represents the key hint
        string getWord() const;

        /// @brief Get the first hint
        /// @return A string represents the first hint
        string getHint1() const;

        /// @brief Get the second hint
        /// @return A string represents the second hint
        string getHint2() const;

        /// @brief Get the third hint
        /// @return A string represents the third hint
        string getHint3() const;  

        /// @brief Get the length of the word
        /// @return An integers represent the length of the word
        int getLength() const;   

        /// @brief Clear the word
        void clear();
};

/// @brief Dictionary class
class dictionary
{
    private:
        /// @brief The vector containing all the words in the game
        vector <word> vWord[DIFFICULTY_COUNT];

        /// @brief Size of the dictionary
        int size[DIFFICULTY_COUNT];

        /// Replace ';' to ',', set ',' as delimiter
        void optimize(string& line);

        /// Replace '_' to ' '
        void rollBack(string& item);

    public:
        /// @brief Load words from the csv file
        void init();

        /// @brief Get a word from the dictionary
        word getWord(const int& difficulty) const;
};

/// @brief Get the word with a space created between the characters
string spaced(const string& __word);

/// @brief Render some text to the screen
/// @param texture The LTexture object
/// @param x,y The render position
/// @param text The text being rendered, in char* form
/// @param fontPath Path to the font (ttf) file
/// @param fontSize Font size
/// @param textColor Text color, in SDL_Color form
void renderText(SDL_Renderer* renderer, LTexture& texture, const char* text, const int& x, const int& y, const int& fontSize = 48, const char* fontPath = &PATH_COMIC_FONT[0], const SDL_Color& fontColor = SDL_COLOR_BLACK);
