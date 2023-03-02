// dictionary.cpp
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "dictionary.h"

word::word()
{
    value = "";
    hint1 = "";
    hint2 = "";
    hint3 = "";
    len = 0;
}

word::word(const string& __value, const string& __hint1, const string& __hint2, const string& __hint3)
{
    value = __value;
    hint1 = __hint1;
    hint2 = __hint2;
    hint3 = __hint3;
    len = value.length();
}

word::~word()
{
    clear();
}

string word::getWord() const
{
    return value;
}

string word::getHint1() const
{
    return hint1;
}

string word::getHint2() const
{
    return hint2;
}

string word::getHint3() const
{
    return hint3;
}

int word::getLength() const
{
    return len;
}

void word::clear()
{
    value.clear();
    hint1.clear();
    hint2.clear();
    hint3.clear();
    len = 0;
}

void dictionary::optimize(string& line)
{
    for (auto &ch : line)
    {
        if (ch == ' ') ch = '_';
        else if (ch == ',') ch = ' ';
        else if (ch == ';') ch = ',';
    }
}

void dictionary::rollBack(string& item)
{
    for (auto &ch : item)
    {
        if (ch == '_') ch = ' ';
    }
}

void dictionary::init()
{
    std::ifstream fi(PATH_DICTIONARY);
    if (!fi.is_open()) return;
    string line;
    int curDifficulty = 0;
    while(getline(fi, line))
    {
        optimize(line);

        std::stringstream ssLine(line);
        int cnt = 0;
        string value, hint1, hint2, hint3, item;
        while (ssLine >> item)
        {
            rollBack(item);
            cnt ++;
            if (cnt == 1) value = item;
            if (cnt == 2) hint1 = item;
            if (cnt == 3) hint2 = item;
            if (cnt == 4) hint3 = item;
        }
        if (value[0] == 'e' || value[0] == 'n' || value[0] == 'd' || value[0] == 'c') 
        {
            curDifficulty++;
            continue;
        }
        if (value == "word") continue;
        vWord[curDifficulty].push_back(word(value, hint1, hint2, hint3));
        size[curDifficulty] ++;
    }
}

word dictionary::getWord(const int& difficulty) const
{
    int pos = random::randInt(0, vWord[difficulty].size() - 1);
    return vWord[difficulty][pos];
}

void dictionary::clear()
{
    for (int i = 0; i < DIFFICULTY_COUNT; i ++) 
    {
        vWord[i].clear();
        size[i] = 0;
    }

}