//charButton.cpp
#include <iostream>
#include <sdl.h>
#include "LButton.h"

class charButton: public LButton
{
    private:
        bool __isUsed;
        char symbol;

    public: 
        /// @brief Check if the button is clicked before
        bool isUsed();

        /// @brief Use the button and change __isUsed to 1
        void useKey();

        /// @brief Update the symbol of the button, 
        /// @param newSym The new symbol, can be uppercase letters or _
        void updateSymbol(char newSym);
        
        /// @brief Handle the cases when the mouse is in or out button
        void handleEvent(SDL_Event* event, const word& key, string& curWord, bool& isTriggered, bool& isIn);

        /// @brief Update the word
        /// @return 1 if the chosen button is in the word
        void trigger(const word& key, string& curWord, bool& isIn);


};