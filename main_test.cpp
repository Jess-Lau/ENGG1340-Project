#include <iostream>
using namespace std;

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void printIntro() {
    cout << "Welcome to my game\n\n"
            "You must collect all six items before fighting the boss.\n" << endl;

    // Press any key to continue . . . 
    system("pause");
    clearScreen();
}

struct Rooms {
    string name;
    string east;
    string south;
    string west;
    string north;
    string item;
};

int main() {
    printIntro();
    cout << "Test" << endl;
}

