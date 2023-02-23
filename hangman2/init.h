//init.h
#include <iostream>
#include <vector>

using namespace std;

enum DIFFICULTY
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
const int PAGE_SIZE = 30;
const string UPPERCASE_LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string VERSION = "0.4";

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
        string getWord();

        /// @brief Get the first hint
        /// @return A string represents the first hint
        string getHint1();

        /// @brief Get the second hint
        /// @return A string represents the second hint
        string getHint2();

        /// @brief Get the third hint
        /// @return A string represents the third hint
        string getHint3();  

        /// @brief Get the length of the word
        /// @return An integers represent the length of the word
        int getLength();   

        /// @brief Clear the word
        void clear();
};

/// Replace ';' to ',', set ',' as delimiter
void optimize(string& line);

/// Replace '_' to ' '
void rollBack(string& item);

/// @brief Initialize the words;-
void initWords(vector <word> dictionary[]);

/// @brief Get the word with a space created between the characters
string spaced(const string& __word);

/// @brief Random integer in [0, limit]
int randInt(const int& limit);

/// @brief Random integer in [lLimit, rLimit]
int randInt(const int& lLimit, const int& rLimit);

/// @brief rand() setup
void initRand();