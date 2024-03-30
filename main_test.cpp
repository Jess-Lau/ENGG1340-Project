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

void printStat(string *currentRoom, string inventory[]) {
    cout << "You are in the " << *currentRoom << "\n" << "Inventory: ";
    int i = 0;
    while (inventory[i] != "") {
        string comma = ", ";
        if (inventory[i+1] == "") {
            comma = "";
        }
        cout << inventory[i] + comma;
        i++;
    }
}

int main() {

    // generate the rooms (fixed)
    Rooms room[8];
    setupRooms(room);

    string inventory[10] = {"Sword", "Sheild"}; // set a inventory of space 10
    string *currentRoom = &room[0].name; // a pointer to the current room (room[0])
    string msg = ""; // result of last move

    clearScreen();
    printIntro();

    // Gameplay
    while (1) {
        clearScreen();
        printStat(currentRoom, inventory);
        
    }
}