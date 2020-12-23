#include <iostream>
#include <map>
#include <fstream>
#include "soundex.h"
#include "levenshtein.h"

using namespace std;


void populateDict(string filename, multimap<string, string> &dict) {

    ifstream file(filename);

    if(file.is_open()) {
        string line;

        while(getline(file, line))  {
            string soundex = Soundex(line);
            dict.insert(make_pair(soundex, line));
        }
        file.close();

    } else {
        cout << "Cannot open file: " << filename << endl;
        exit(-1);
    }

    cout << dict.size() << endl;

}

int main() {

    multimap<string, string> dict;
    multimap<string, string> affixes;

    populateDict(R"(C:\Users\navruz\CLionProjects\uzbek-spellchecker\lexicon.txt)", dict);
    populateDict(R"(C:\Users\navruz\CLionProjects\uzbek-spellchecker\oneElementAffix.txt)", affixes);
    populateDict(R"(C:\Users\navruz\CLionProjects\uzbek-spellchecker\twoElementsAffixes.txt)", affixes);

    string word = "daftaringizmikan";

    multimap<int, string> sortedWords;

    bool isFound = false;
    string temp = word;
    int affixBoundary;

    for (affixBoundary = temp.length() - 1; affixBoundary >= 0 && !isFound; affixBoundary--) {

        temp = temp.substr(0, affixBoundary);

        pair<multimap<string, string>::iterator, multimap<string, string>::iterator> its = dict.equal_range(Soundex(temp));

        for (auto it = its.first; it != its.second; it++) {
            int distance = levenshteinDistance(temp, it->second);

            if (distance == 0) {
                sortedWords.insert(make_pair(distance, it->second));
                isFound = true;
            }
        }

    }


    string affixPart = word.substr(affixBoundary + 1);

    pair<multimap<string, string>::iterator, multimap<string, string>::iterator> its2 = affixes.equal_range(Soundex(affixPart));

    multimap<int, string> sortedAffixes;

    for (auto it = its2.first; it != its2.second; it++) {
        int distance = levenshteinDistance(affixPart, it->second);

        if (distance < 4) {
            sortedAffixes.insert(make_pair(distance, it->second));
        }
    }



    cout << "Current entry: " + word << endl;
    cout << "\nMatching morphemes: " << endl;


    for (auto it = sortedAffixes.begin(); it != sortedAffixes.end(); it++) {
        cout << word << it->second << " - " << it->first << endl;
    }


    return 0;
}
