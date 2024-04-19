#include <iostream>
#include <string.h>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <sstream>
using namespace std;

void clearScreen();

void printIntro();

struct Rooms {
    string name;
    string east;
    string south;
    string west;
    string north;
    string item;
};

struct Inventory {
    string item;
    Inventory *next;
};

void add_to_inventory(Inventory * &head, Inventory * &tail, string item);

void setupRooms(Rooms room[]);

void printStat(string *currentRoom, Inventory * inventory_head);

bool isItemNearby(string *currentRoom, Rooms room[]);

string findNearbyItem(string *currentRoom, Rooms room[]);

bool isInInventory (Inventory *inventoryHead, string item);

bool isVowel(string item);

bool bossInRoom(Rooms room[], string currentRoom);

int sizeOfInventory(Inventory *inventoryHead);

string toLower(string str);

int roomNumber(Rooms *room, string currentRoom);

int main() {

    // generate the rooms (fixed)
    Rooms room[8];
    setupRooms(room);

    Inventory *inventory_head = NULL, *inventory_tail = NULL; // create a linked list of inventory

    // add_to_inventory(inventory_head, inventory_tail, "Knife");
    // add_to_inventory(inventory_head, inventory_tail, "Sword");
    
    string *currentRoom = &room[0].name; // a pointer to the current room (room[0])
    string msg = ""; // result of last move
    string nearbyItem; // item nearby

    printIntro();

    // Gameplay
    while (1) {
        clearScreen();
        printStat(currentRoom, inventory_head);

        cout << msg << endl;

        // item indicator
        if (isItemNearby(currentRoom, room)) {
            nearbyItem = findNearbyItem(currentRoom, room);
            if (!isInInventory(inventory_head, nearbyItem)) {
                string mod = "a ";

                // plural
                if (nearbyItem[nearbyItem.length()-1] == 's') {
                    mod = "";
                }

                // vowel
                else if (isVowel(nearbyItem)) {
                    mod = "an ";
                }
                cout << "You see " << mod + nearbyItem << endl;
           }
           else {
                cout << "Item collected..." << endl;
           }
        }
        else {
            cout << "No items nearby" << endl;
        }
        
        // Boss encounter
        if (bossInRoom(room, *currentRoom)) {

            // Lose
            if (sizeOfInventory(inventory_head) < 6) {
                cout << "You lost a fight with " << findNearbyItem(currentRoom, room) << '.' << endl;
            }

            // Win
            else {
                cout << "You beat " << findNearbyItem(currentRoom, room) << '!' << endl;
                break;
            }
        }

        // Player's move
        string action, item, direction;
        cout << "Enter your move: ";
        cin >> action;

        action = toLower(action);

        // Moving player
        if (action == "go") {
            getline(cin, direction);
            direction = toLower(direction.substr(1))[0];
            // cout << direction << endl;
            if (direction == "n" || direction == "s" || direction == "e" || direction == "w") {
                int currentRoomNumber = roomNumber(room, *currentRoom);
                if (direction == "n" && room[currentRoomNumber].north != "") {
                    int nextRoomNumber = roomNumber(room, room[currentRoomNumber].north);
                    currentRoom = &room[nextRoomNumber].name;
                    // cout << "After" << endl;
                    // cout << "Current room: " << *currentRoom << endl;
                    // cout << "Current room number: " << nextRoomNumber << endl;
                    // cout << "---------------------------------" << endl;
                    msg = "You travelled north";
                }
                else if (direction == "s" && room[currentRoomNumber].south != "") {
                    int nextRoomNumber = roomNumber(room, room[currentRoomNumber].south);
                    currentRoom = &room[nextRoomNumber].name;
                    msg = "You travelled south";
                }
                else if (direction == "e" && room[currentRoomNumber].east != "") {
                    int nextRoomNumber = roomNumber(room, room[currentRoomNumber].east);
                    currentRoom = &room[nextRoomNumber].name;
                    msg = "You travelled east";
                }
                else if (direction == "w" && room[currentRoomNumber].west != "") {
                    int nextRoomNumber = roomNumber(room, room[currentRoomNumber].west);
                    currentRoom = &room[nextRoomNumber].name;
                    msg = "You travelled west";
                }
                else {
                    msg = "You can't go that way.";
                }
            }
            else {
                msg = "Invalid direction";
            }
        }

        // Picking up items
        else if (action == "get") {
            getline(cin, item);
            item = toLower(item.substr(1));
            cout << item << endl;
            if (item == toLower(findNearbyItem(currentRoom, room))) {
                item = findNearbyItem(currentRoom, room);
                add_to_inventory(inventory_head, inventory_tail, item);
                item[0] = toupper(item[0]); // capitalize the first letter
                msg = item + " retreived!";
            }
            else {
                msg = "No such item nearby";
            }
        }

        // Exit game
        else if (action == "quit") {
            break;
        }

        // Invalid command
        else {
            msg = "Invalid action";
        }
    }
}



void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void printIntro() {
    cout << "Welcome to my game\n\n"
            "You must collect all six items before fighting the boss.\n" <<
            "Moves:\t'go {direction}' (travel north, south, east, or west)\n" <<
            "\t'get {item}' (add nearby item to inventory)\n" << endl;

    // Press any key to continue . . . 
    system("pause");
}

void add_to_inventory(Inventory * &head, Inventory * &tail, string item) {
    Inventory *p = new Inventory;
    p->item = item;
    p->next = NULL;

    if (head == NULL) {
        head = p;
        tail = p;
    }
    else {
        tail->next = p;
        tail = p;
    }
}

void setupRooms(Rooms room[]) {
    room[0] = {"Liminal Space", "Bazaar", "Bat Cavern", "", "Mirror Maze", ""};
    room[1] = {"Mirror Maze", "", "Liminal Space", "", "", "Crystal"};
    room[2] = {"Meat Locker", "Quicksand", "Bazaar", "", "", "Fig Newtoon"};
    room[3] = {"Quicksand", "", "", "Meat Locker", "", "Robe"};
    room[4] = {"Bazaar", "Dojo", "", "Liminal Space", "Meat Locker", "Altoids"};
    room[5] = {"Dojo", "", "", "Bazaar", "", "Boss"};
    room[6] = {"Bat Cavern", "Volcano", "", "", "Liminal Space", "Staff"};
    room[7] = {"Volcano", "", "", "Bat Cavern", "", "Syrup"};
}

void printStat(string *currentRoom, Inventory * inventory_head) {
    cout    << "You are in the " << *currentRoom << "\n"
            << "Inventory: ";

    // print inventory
    Inventory *current = inventory_head;
    while (current != NULL) {
        if (current->next == NULL) {
            cout << current->item << endl;
        }
        else {
            cout << current->item << ", ";
        }
        current = current->next;
    }

    cout << "\n----------------------------\n";
}

bool isItemNearby(string *currentRoom, Rooms room[]) {
    for (int i = 0; i < 8; i++) {
        if (room[i].name == *currentRoom) {
            if (room[i].item != "") {
                // cout << "\tisItemNearby: true\n" << endl;
                return true;
            }
            return false;
        }
    }
    return false;
}

string findNearbyItem(string *currentRoom, Rooms room[]) {
    for (int i = 0; i < 8; i++) {
        if (room[i].name == *currentRoom){
            // cout << "\tfindNearbyItem: " << room[i].item << endl;
            return room[i].item;
        }
    }
    return "";
}

bool isInInventory (Inventory *inventoryHead, string item) {
    Inventory *current = inventoryHead;
    while (current != NULL) {
        if (toLower(current->item) == toLower(item)) {
            // cout << "\tisInInventory: true\n" << endl;
            return true;
        }
        current = current->next;
    }
    return false;
}

bool isVowel(string item) {
    if (item[0] == 'a' || item[0] == 'e' || item[0] == 'i' || item[0] == 'o' || item[0] == 'u') {
        return true;
    }
    return false;
}

bool bossInRoom(Rooms room[], string currentRoom) {
    for (int i = 0; i < 8; i++) {
        if (room[i].name == currentRoom && room[i].item == "Boss") {
            return true;
        }
    }
    return false;
}

int sizeOfInventory(Inventory *inventoryHead) {
    int count = 0;
    Inventory *current = inventoryHead;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

string toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

int roomNumber(Rooms *room, string currentRoom) {
    // cout << "Before" << endl;
    // cout << "Current room: " << currentRoom << endl;
    for (int i = 0; i < 8; i++) {
        if (room[i].name == currentRoom) {
            return i;
        }
    }
    return -1;

}