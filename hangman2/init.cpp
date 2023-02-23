//init.cpp
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include "init.h"

using namespace std;

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

string word::getWord()
{
    return value;
}

string word::getHint1()
{
    return hint1;
}

string word::getHint2()
{
    return hint2;
}

string word::getHint3()
{
    return hint3;
}

int word::getLength()
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

void optimize(string& line)
{
    for (auto &ch : line)
    {
        if (ch == ' ') ch = '_';
        else if (ch == ',') ch = ' ';
        else if (ch == ';') ch = ',';
    }
}

void rollBack(string& item)
{
    for (auto &ch : item)
    {
        if (ch == '_') ch = ' ';
    }
}

void initWords(vector <word> dictionary[])
{
    ifstream fi("dictionary.csv");
    if (!fi.is_open()) return;
    string line;
    int curDifficulty = 0;
    while(getline(fi, line))
    {
        optimize(line);

        stringstream ssLine(line);
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
        dictionary[curDifficulty].push_back(word(value, hint1, hint2, hint3));
    }
}

string spaced(const string& __word)
{
    string ret = "";
    for (auto i : __word) ret.push_back(i), ret.push_back(' ');
    return ret; 
}

int randInt(const int& limit)
{
    int x = rand();
    int y = rand();
    int z = rand();
    return (1ll * x * y + z) % (limit + 1);
}

int randInt(const int& lLimit, const int& rLimit)
{
    return randInt(rLimit - lLimit) + lLimit;
}

void initRand()
{
    srand(int(time(0)));
}