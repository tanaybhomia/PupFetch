#include "dogbreeds.h"
#include <fstream>
#include <iostream>
using  namespace std;

void displayDogArt(string breed) {
    ifstream artFile("art/" + breed + ".txt");
    if (artFile.is_open()) {
        string line;
        while (getline(artFile, line)) {
            cout << line << endl; // Output each line of ASCII art
        }
        artFile.close();
    } else {
        cerr << "Could not open file for breed: " << breed << endl;
    }
}
