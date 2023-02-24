// guessWord.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "dictionary.h"

class guessWord
{
    private:
        /// @brief The word need guessing, in hidden form
        std::string value;

        /// @brief The texture object of the word need guessing
        LTexture guessWordTexture;
    
    public:
        /// @brief Default constructor
        guessWord();

        /// @brief Default destructor
        ~guessWord();

        /// @brief Get the current state of the hidden word
        std::string getWord() const;

        /// @brief Initialization
        void init(const word& key);

        /// @brief Update the hidden word
        /// @param pos The position need updating
        /// @param symbol The updated symbol
        void updateChar(const int& pos, const char& symbol);
        
        /// @brief Check if the game is over when no blank left
        /// @return 1 if the game is over, 0 otherwise
        bool victory() const;

        /// @brief Render the word need guessing
        void render(SDL_Renderer* renderer);

        /// @brief Cleanup
        void clear();


};