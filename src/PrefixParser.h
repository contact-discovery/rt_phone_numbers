//
// Created by Sebastian Schindler on 20.06.19.
//

#ifndef RAINBOWCRACK_PREFIXPARSER_H
#define RAINBOWCRACK_PREFIXPARSER_H
#include <list>
#include <map>
#include <vector>
#include <string>
using namespace std;
class PrefixParser {
public:
    //static bool startsWith(string mainStr, string toMatch);

    static vector<tuple < string, int>> read_numberspecs(string file);

    //static map <string, vector<string>> country_prefixes();
};

#endif //RAINBOWCRACK_PREFIXPARSER_H
