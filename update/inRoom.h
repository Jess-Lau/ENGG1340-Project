#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <cstdio>
#include <unistd.h>

// #include "pathfinding.h"
// #include "mapGeneration.h"

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

void clearScreenW() {
    std::wcout << "\033[2J\033[1;1H";
}

void printInRoom(std::string room[][15], const int roomWidth, const int roomLength, Coordinates playerCord ,std::string inputKey, std::vector<std::string> inventory, int health) {
    // _setmode(_fileno(stdout), _O_U16TEXT); // Change stdout to Unicode UTF-16
    for (int i = 1; i < roomWidth+3; i++) {
        // print map
        for (int j = 0; j < roomLength+4; j++) {
            std::string mod1, mod2;
            mod1 = mod2 = " ";

            if (inputKey == "UP" && i == playerCord.x-1 && j == playerCord.y) {
                mod1.insert(0, BOLD); mod2.insert(mod2.size()-1, RESET);
            } else if (inputKey == "DOWN" && i == playerCord.x+1 && j == playerCord.y) {
                mod1.insert(0, BOLD); mod2.insert(mod2.size()-1, RESET);
            } else if (inputKey == "LEFT" && i == playerCord.x && j == playerCord.y-1) {
                mod1.insert(0, BOLD); mod2.insert(mod2.size()-1, RESET);
            } else if (inputKey == "RIGHT" && i == playerCord.x && j == playerCord.y+1) {
                mod1.insert(0, BOLD); mod2.insert(mod2.size()-1, RESET);
            }
            
            if (j == 1 || j == roomWidth + 2) mod1 = mod2 = '\0';
            // Player
            if (room[i][j] == "P") {
                std::cout << GREEN << mod1 << "P" << mod2 << RESET;
                // ⚉
            }
            // Zombie
            else if (room[i][j] == "Z") {
                std::cout << RED << mod1 << "Z" << mod2 << RESET;
            }
            // item
            else if (room[i][j] == "I") {
                if (mod1 == " ") {
                    std::cout << YELLOW << mod1 << "○" << mod2 << RESET;
                }
                else {
                    std::cout << YELLOW << mod1 << "●" << mod2 << RESET;
                }
            }
            else {
                std::cout << room[i][j];
                if (j > 1 && j < roomLength+2) {
                    std::cout << room[i][j] << room[i][j];
                }
            }
        }
        //print stat
        if (i == 1) {
            std::cout << "Inventory:";
        }
        else if (i == 2) {
            for (int k = 0; k <= 7; k++) {
                if (k < inventory.size()) {
                    std::cout << "■ ";
                } else {
                    std::cout << "□ ";
                }
            }
        }
        else if (i == 3) {
            std::cout << "Health:";
        }
        else if (i == 4) {
            for (int k = 0; k < health/10; k++) {
                std::cout << "█";
            }
            if (health%10 >= 5) {
                std::cout << "▌";
            }
            std::cout << ' ' << health << '%';
        }
        std::cout << std::endl;
    }
    // _setmode(_fileno(stdout), _O_TEXT); // Change stdout back to ASCII
}

void createInRoom(std::map<std::string, Rooms> rooms, std::string roomName, std::string room[][15], const int roomWidth, const int roomLength, Coordinates playerCord, Coordinates itemCord, std::vector<Coordinates> zombieCord) {
    // _setmode(_fileno(stdout), _O_U16TEXT); // Change stdout to Unicode UTF-16

    // Open doors to other rooms
    if (rooms[roomName].north != "") {
        room[1][roomLength/2+2] = ' ';
    }
    if (rooms[roomName].south != "") {
        room[roomWidth+2][roomLength/2+2] = ' ';
    }
    if (rooms[roomName].east != "") {
        room[roomWidth/2+2][roomLength+2] = ' ';
    }
    if (rooms[roomName].west != "") {
        room[roomWidth/2+2][1] = ' ';
    }

    // Create zombies
    for (int i = 0; i < zombieCord.size(); i++) {
        room[zombieCord[i].x][zombieCord[i].y] = 'Z';
    }

    // Create player
    room[playerCord.x][playerCord.y] = 'P';

    // Create item
    room[itemCord.x][itemCord.y] = 'I';

    // Create walls
    for (int i = 0; i < roomWidth+4; i++) {
        for (int j = 0; j < roomLength+4; j++) {
            if (room[i][j] != " " && room[i][j] != "Z" && room[i][j] != "P" && room[i][j] != "I") {
                if (i == 1 && j == 1){
                    room[i][j] = "┌";
                } else if (i == 1 && j == roomLength+2) {
                    room[i][j] = "┐";
                } else if (i == roomWidth+2 && j == 1) {
                    room[i][j] = "└";
                } else if (i == roomWidth+2 && j == roomLength+2) {
                    room[i][j] = "┘";
                } else if (i == 0 || j == 0 || i == roomWidth+3 || j == roomLength+3) {
                    room[i][j] = " ";
                } else if (i == 1 || i == roomWidth+2) {
                    room[i][j] = "─";
                } else if (j == 1 || j == roomLength+2) {
                    room[i][j] = "│";
                } else {
                    room[i][j] = " ";
                }
            }
        }
    }

    // _setmode(_fileno(stdout), _O_TEXT); // Change stdout back to ASCII

    // printInRoom(room, roomWidth, roomLength);
}

int detectKey() {
    char c;
    // Set the terminal to raw mode
    system("stty raw -echo");  // turn off echoing
    c = getchar(); 
    system("stty cooked echo");  // turn echoing back on
    return (int)c;
}

void moveToNext(std::string room[][15], Coordinates & playerCord, std::string inputKey, std::string check) {
    std::string originalValue = room[playerCord.x][playerCord.y];
    room[playerCord.x][playerCord.y] = ' ';
    if (inputKey == "UP") {
        if (room[playerCord.x-1][playerCord.y] == check) {
            room[playerCord.x-1][playerCord.y] = room[playerCord.x][playerCord.y];
            room[playerCord.x][playerCord.y] = ' ';
            playerCord.x--;
        }
    } else if (inputKey == "DOWN") {
        if (room[playerCord.x+1][playerCord.y] == check) {
            room[playerCord.x+1][playerCord.y] = room[playerCord.x][playerCord.y];
            room[playerCord.x][playerCord.y] = ' ';
            playerCord.x++;
        }
    } else if (inputKey == "LEFT") {
        if (room[playerCord.x][playerCord.y-1] == check) {
            room[playerCord.x][playerCord.y-1] = room[playerCord.x][playerCord.y];
            room[playerCord.x][playerCord.y] = ' ';
            playerCord.y--;
        }
    } else if (inputKey == "RIGHT") {
        if (room[playerCord.x][playerCord.y+1] == check) {
            room[playerCord.x][playerCord.y+1] = room[playerCord.x][playerCord.y];
            room[playerCord.x][playerCord.y] = ' ';
            playerCord.y++;
        }
    } else {
        // PlaySound(TEXT("wall-hit.mp3"), NULL, SND_FILENAME | SND_ASYNC);
        // PlaySound(0, 0, 0);
        // add "-lwinmm" to task.json
    }
    room[playerCord.x][playerCord.y] = originalValue;
}

void facingSide(Coordinates & facing, std::string inputKey) {
    if (inputKey == "UP") {
        facing = {-1, 0};
    }
    else if (inputKey == "DOWN") {
        facing = {1, 0};
    }
    else if (inputKey == "LEFT") {
        facing = {0, -1};
    }
    else if (inputKey == "RIGHT") {
        facing = {0, 1};
    }
    else facing = {0, 0};
}

void clearRoom(std::string room[][15], const int roomLength, const int roomWidth) {
    for (int i = 0; i < roomWidth+4; i++) {
        for (int j = 0; j < roomLength+4; j++) {
            room[i][j] = '\0';
        }
    }
}

Coordinates randomCord(std::string room[15][15], int roomLength, int roomWidth) {
    Coordinates cord = {rand() % roomLength + 2, rand() % roomWidth + 2};
    // while (room[cord.x][cord.y] != L'\u00A0') {
    //     cord = {rand() % roomLength + 2, rand() % roomWidth + 2};
    // }
    return cord;
}

void spawnZombies(std::string room[15][15], const int roomLength, const int roomWidth, std::string roomName, std::vector<Coordinates> & zombieCord, std::map<std::string, std::vector<Coordinates>> & zombieCords, Coordinates itemCord) {
    int randomNum = rand() % 6;
    int numOfZombies;
    switch (randomNum) {
        case 0: case 1:
            numOfZombies = 1;
            break;
        case 2: case 3: case 4:
            numOfZombies = 2;
            break;
        case 5:
            numOfZombies = 3;
            break;
        default:
            numOfZombies = 0;
            break;
    }
    zombieCord.clear();
    zombieCord.reserve(numOfZombies);
    for (int i = 0; i < numOfZombies; i++) {
        Coordinates cord = randomCord(room, roomLength, roomWidth);
        while (cord.x == itemCord.x && cord.y == itemCord.y) {
            cord = randomCord(room, roomLength, roomWidth);
        }
        zombieCord.emplace_back(cord);
    }
}

std::string removeSpace (std::string name) {
    for (int i = 0; i < name.size(); i++) {
        if (name[i] == ' ') name[i] = '_';
    }
    return name;
}

std::string addSpace (std::string name) {
    for (int i = 0; i < name.size(); i++) {
        if (name[i] == '_') name[i] = ' ';
    }
    return name;
}

void saveGame(std::string roomMap[][7], std::map<std::string, Rooms> & rooms, Coordinates & currentRoomCord, std::vector<std::string> inventory, std::string room[][15], Coordinates playerCord, Coordinates itemCord, std::vector<Coordinates> zombieCord, std::map<std::string, Coordinates> itemCords, std::map<std::string, std::vector<Coordinates>> zombieCords, int health, int bossHealth) {
    // current date and time on the current system
    time_t now = time(0);
    // convert now to string form
    std::string date_time = ctime(&now);
    std::cout << date_time;

    date_time[date_time.length() - 1] = '\0';
    std::string gameFile = "save/" + removeSpace(date_time);
    std::ofstream fout;
    fout.open(gameFile.c_str());
    if (fout.fail()) {
        std::cout << "Error: Unable to create file: " << gameFile << std::endl;
    }
    else {
        fout << "RoomMap" << std::endl;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 7; j++) {
                if (roomMap[i][j] == "") fout << " |";
                else fout << roomMap[i][j] << '|';
            }
            fout << std::endl;
        }
        fout << std::endl;

        fout << "Rooms";
        for (auto it = rooms.begin(); it != rooms.end(); it++) {
            fout << std::endl;
            fout << removeSpace(it->first) << "|" << removeSpace(it->second.name) << "|";
            if (it->second.item == "") fout << " |";
            else fout << it->second.item << '|';
            fout << it->second.isStartingRoom << '|' << it->second.isBossRoom << '|';
            if (it->second.east == "") fout << " |";
            else fout << it->second.east << '|';
            if (it->second.south == "") fout << " |";
            else fout << it->second.south << '|';
            if (it->second.west == "") fout << " |";
            else fout << it->second.west << '|';
            if (it->second.north == "") fout << " |";
            else fout << it->second.north << '|';
        }
        fout << std::endl;
        fout << std::endl;

        fout << "CurrentRoomCord" << std::endl;
        fout << currentRoomCord.x << ' ' << currentRoomCord.y << std::endl;
        fout << std::endl;

        fout << "Inventory" << std::endl;
        for (int i = 0; i < inventory.size(); i++) {
            fout << removeSpace(inventory[i]) << ' ';
        }
        fout << std::endl << std::endl;

        fout << "PlayerCord" << std::endl;
        fout << playerCord.x << ' ' << playerCord.y << std::endl;
        fout << std::endl;

        fout << "ItemCord" << std::endl;
        fout << itemCord.x << ' ' << itemCord.y << std::endl;
        fout << std::endl;

        fout << "ZombieCord" << std::endl;
        for (int i = 0; i < zombieCord.size(); i++) {
            fout << zombieCord[i].x << ' ' << zombieCord[i].y;
            fout << std::endl;
        }
        fout << std::endl;

        fout << "ItemCords" << std::endl;
        for (auto it = itemCords.begin(); it != itemCords.end(); it++) {
            fout << removeSpace(it->first) << " " << it->second.x << ' ' << it->second.y << std::endl;
        }
        fout << std::endl;

        fout << "ZombieCords" << std::endl;
        for (auto it = zombieCords.begin(); it != zombieCords.end(); it++) {
            fout << removeSpace(it->first) << " ";
            for (int i = 0; i < it->second.size(); i++) {
                fout << it->second[i].x << ' ' << it->second[i].y << " ";
            }
            fout << std::endl;
        }
        fout << std::endl;

        fout << "Health" << std::endl;
        fout << health << std::endl;
        fout << std::endl;

        fout << "Boss health" << std::endl;
        fout << bossHealth << std::endl;
    }
}


void loadGame(std::string gameFile, std::string roomMap[][7], std::map<std::string, Rooms> & rooms, Coordinates & currentRoomCord, std::vector<std::string> & inventory, std::string room[][15], Coordinates & playerCord, Coordinates & itemCord, std::vector<Coordinates> & zombieCord, std::map<std::string, Coordinates> & itemCords, std::map<std::string, std::vector<Coordinates>> & zombieCords, int & health, int & bossHealth) {
    std::string gameFilePath = "save/" + gameFile;
    std::ifstream fin;
    fin.open(gameFilePath.c_str());

    if (fin.fail()) {
        std::cout << "Error: Unable to open file: " << gameFile << std::endl;
    }

    else {
        std::string line;
        while (getline(fin, line)) {
            if (line == "RoomMap") {
                for (int i = 0; i < 5; i++) {
                    getline(fin, line);
                    std::istringstream iss(line);
                    std::string target;
                    int count = 0;
                    while (getline(iss, target, '|')) {
                        if (target == " ") roomMap[i][count] = "";
                        else roomMap[i][count] = target;
                        count++;
                    }
                }
            }
            else if (line == "Rooms") {
                while (getline(fin, line)) {
                    if (line == "") break;
                    std::istringstream iss(line);
                    std::string target;
                    std::string roomName;
                    int count = 0;
                    while (getline(iss, target, '|')) {
                        if (count == 0) {
                            roomName = addSpace(target);
                            count++;
                            continue;
                        }
                        else if (count == 1) {
                            rooms[roomName].name = addSpace(target);
                        }
                        else if (count == 2 && target != " ") {
                            rooms[roomName].item = addSpace(target);
                        }
                        else if (count == 3) {
                            rooms[roomName].isStartingRoom = stoi(target);
                        }
                        else if (count == 4) {
                            rooms[roomName].isBossRoom = stoi(target);
                        }
                        else if (count == 5 && target != " ") {
                            rooms[roomName].east = addSpace(target);
                        }
                        else if (count == 6 && target != " ") {
                            rooms[roomName].south = addSpace(target);
                        }
                        else if (count == 7 && target != " ") {
                            rooms[roomName].west = addSpace(target);
                        }
                        else if (count == 8 && target != " ") {
                            rooms[roomName].north = addSpace(target);
                        }
                        count++;
                    }
                }
            }
            else if (line == "CurrentRoomCord") {
                getline(fin, line);
                std::istringstream iss(line);
                iss >> currentRoomCord.x >> currentRoomCord.y;
            }
            else if (line == "Inventory") {
                getline(fin, line);
                std::istringstream iss(line);
                while (iss >> line) {
                    inventory.emplace_back(addSpace(line));
                }
            }
            else if (line == "PlayerCord") {
                getline(fin, line);
                std::istringstream iss(line);
                iss >> playerCord.x >> playerCord.y;
            }
            else if (line == "ItemCord") {
                getline(fin, line);
                std::istringstream iss(line);
                iss >> itemCord.x >> itemCord.y;
            }
            else if (line == "ZombieCord") {
                zombieCord.clear();
                while (getline(fin, line)) {
                    if (line == "") break;
                    int x, y;
                    std::istringstream iss(line);
                    iss >> x >> y;
                    zombieCord.emplace_back(Coordinates({x, y}));
                }
            }
            else if (line == "ItemCords") {
                while (getline(fin, line)) {
                    if (line == "") break;
                    std::string roomName;
                    int x, y;
                    std::istringstream iss(line);
                    iss >> roomName >> x >> y; // Use iss here, not fin
                    itemCords[addSpace(roomName)] = {x, y};
                }
            }
            else if (line == "ZombieCords") {
                while (getline(fin, line)) {
                    if (line == "") break;
                    std::string roomName;
                    int x, y;
                    std::istringstream iss(line);
                    iss >> roomName >> x >> y; // Use iss here, not fin
                    zombieCords[addSpace(roomName)].emplace_back(Coordinates({x, y}));
                }
            }
            else if (line == "Health") {
                fin >> health;
            }
            else if (line == "Boss health") {
                fin >> bossHealth;
            }
        }
    }

    fin.close();

    std::remove(gameFilePath.c_str());
}

void printAll(std::string roomMap[][7], std::map<std::string, Rooms> rooms, Coordinates currentRoomCord, std::vector<std::string> inventory, std::string room[][15], Coordinates playerCord, Coordinates itemCord, std::vector<Coordinates> zombieCord, std::map<std::string, Coordinates> itemCords, std::map<std::string, std::vector<Coordinates>> zombieCords, int health) {
    std::cout << "RoomMap" << std::endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 7; j++) {
            std::cout << roomMap[i][j] << '|';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Rooms" << std::endl;
    for (auto it = rooms.begin(); it != rooms.end(); it++) {
        std::cout << it->first << ' ' << it->second.name << ' ' << it->second.item << ' ' << it->second.isStartingRoom << ' ' << it->second.isBossRoom << ' ' << it->second.east << ' ' << it->second.south << ' ' << it->second.west << ' ' << it->second.north << std::endl;
    }
    std::cout << std::endl;

    std::cout << "CurrentRoomCord" << std::endl;
    std::cout << currentRoomCord.x << ' ' << currentRoomCord.y << std::endl;
    std::cout << std::endl;

    std::cout << "Inventory" << std::endl;
    for (int i = 0; i < inventory.size(); i++) {
        std::cout << inventory[i] << ' ';
    }
    std::cout << std::endl;

    std::cout << "Room" << std::endl;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            std::cout << room[i][j] << '|';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "PlayerCord" << std::endl;
    std::cout << playerCord.x << ' ' << playerCord.y << std::endl;
    std::cout << std::endl;

    std::cout << "ItemCord" << std::endl;
    std::cout << itemCord.x << ' ' << itemCord.y << std::endl;
    std::cout << std::endl;

    std::cout << "ZombieCord" << std::endl;
    for (int i = 0; i < zombieCord.size(); i++) {
        std::cout << zombieCord[i].x << ' ' << zombieCord[i].y;
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "ItemCords" << std::endl;
    std::cout << itemCords.size() << std::endl;
    for (auto it = itemCords.begin(); it != itemCords.end(); it++) {
        std::cout << it->first << ' ' << it->second.x << ' ' << it->second.y << std::endl;
    }
    std::cout << std::endl;

    std::cout << "ZombieCords" << std::endl;
    std::cout << zombieCords.size() << std::endl;
    for (auto it = zombieCords.begin(); it != zombieCords.end(); it++) {
        std::cout << it->first << " ";
        for (int i = 0; i < it->second.size(); i++) {
            std::cout << it->second[i].x << ' ' << it->second[i].y << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Health" << std::endl;
    std::cout << health << std::endl;
}

void clearData(const int mapLength, const int mapWidth, std::string roomMap[][7], std::map<std::string, Rooms> & rooms, Coordinates & currentRoomCord, std::vector<std::string> & inventory, std::string & msg, const int roomWidth, const int roomLength, std::string room[15][15], Coordinates & playerCord, Coordinates & itemCord, std::vector<Coordinates> & zombieCord, std::map<std::string, Coordinates> & itemCords, std::map<std::string, std::vector<Coordinates>> & zombieCords, int & health, int & bossHealth, Coordinates facing) {

    for (int i = 0; i < mapLength; i++) {
        for (int j = 0; j < mapWidth; j++) {
            roomMap[i][j] = "";
        }
    }

    rooms.clear();

    currentRoomCord = {mapLength/2, mapWidth/2};

    inventory.clear();

    msg = "";

    for (int i = 0; i < roomWidth+4; i++) {
        for (int j = 0; j < roomLength+4; j++) {
            room[i][j] = "";
        }
    }

    playerCord = {roomLength/2 + 2, roomWidth/2 + 2};

    itemCord = {0, 0};

    zombieCord.clear();
    zombieCord = {{0,1}};

    itemCords.clear();

    zombieCords.clear();

    health = 100;
    bossHealth = 100;
    // const int arr_size = 10;
    // some_type src[arr_size];
    // // ...
    // some_type dest[arr_size];
    // std::copy(std::begin(src), std::end(src), std::begin(dest));

    return;
}