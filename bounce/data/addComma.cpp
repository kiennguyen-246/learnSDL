#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    freopen("map.txt", "r", stdin);
    freopen("commaMap.txt", "w", stdout);

    string line;
    while (getline(cin, line))
    {
        for (int i = 0; i < line.length(); i ++) cout << line[i] << ",";
        cout << "\n";
    }
}