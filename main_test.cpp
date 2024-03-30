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

void setupRooms(Rooms room[]) {
    room[0] = {"Minimal Space", "Bazaar", "Bat Cavern", "", "Mirror Maze", ""};
    room[1] = {"Mirror Maze", "", "Liminal Space", "", "", "Crystal"};
    room[2] = {"Meat Locker", "Quicksand", "Bazaar", "", "", "Fig Newtoon"};
    room[3] = {"Quicksand", "", "", "Meat Locker", "", "Robe"};
    room[4] = {"Bazaar", "Dojo", "", "Liminal Space", "Meat Locker", "Altoids"};
    room[5] = {"Dojo", "", "", "Bazaar", "", "Boss"};
    room[6] = {"Bat Cavern", "Volcano", "", "", "Liminal Space", "Staff"};
    room[7] = {"Volcano", "", "", "Bat Cavern", "", "Syrup"};
}

int main() {
    printIntro();

    // generate the rooms (fixed)
    Rooms room[8];
    setupRooms(room);

    //
}