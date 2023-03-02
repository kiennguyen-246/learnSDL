// dictionary.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "init.h"

const std::string PATH_DICTIONARY = "data/dictionary.csv";

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
        
        /// @brief Cleanup
        void clear();
};