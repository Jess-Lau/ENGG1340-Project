#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <cstdio>
#include <unistd.h>

#include "gameSetUp.h"
#include "inRoom.h"
// #include "mapGeneration.h"
#include "pathfinding.h"
#include "window.h"

int main() {

    const int mapLength = 5, mapWidth = 7;
    std::string roomMap[mapLength][mapWidth]; // make a 5x7 map
    std::map<std::string, Rooms> rooms; // store room details
    std::string startingRoom, bossRoom; // store the starting room and boss room
    Coordinates currentRoomCord = {mapLength/2, mapWidth/2};

    std::vector<std::string> inventory;
    std::string msg = ""; // result of last move

    // Create in-room
    const int roomWidth = 11, roomLength = 11;
    std::string room[15][15]; // 11 + 4 = 15, one outter layer for walls, one more outter layer for travel room detection
    Coordinates playerCord = {roomLength/2 + 2, roomWidth/2 + 2}; // set player coordinates
    Coordinates itemCord = {0, 0}; // set item coordinates (cannot be see yet, cuz player is in the starting room)
    std::vector<Coordinates> zombieCord;
    zombieCord = {{0,1}}; // make zombies (cannot be see yet, cuz player is in the starting room)
    std::map<std::string, Coordinates> itemCords; // for storing item coordinates
    std::map<std::string, std::vector<Coordinates>> zombieCords; // for storing zombie coordinates

    
    // Player Stat
    int health = 100;
    int bossHealth = 20;

    Coordinates facing = {0, 0};

    std::string mode = "";
    menuWin(mode);

    while (mode == "Resume Game") {
        std::cout << "Resume Game" << std::endl;
        std::string gameFile;
        resumeGameWin(gameFile);
        if (!gameFile.empty()) {
            loadGame(gameFile, roomMap, rooms, currentRoomCord, inventory, room, playerCord, itemCord, zombieCord, itemCords, zombieCords, health, bossHealth);
            // printAll(roomMap, rooms, currentRoomCord, inventory, room, playerCord, itemCord, zombieCord, itemCords, zombieCords, health);
            createInRoom(rooms, roomMap[currentRoomCord.x][currentRoomCord.y], room, roomWidth, roomLength, playerCord, itemCord, zombieCord);
            break;
        }
        else {
            menuWin(mode);
        }
    }

    if (mode == "Setting") {
        settingWin();
        menuWin(mode);
    }

    while (mode == "Start Game" || mode == "Resume Game") {
        std::cout << "Start Game" << std::endl;
        clearScreenW();

        if (mode == "Start Game") {
            generateMap(roomMap, mapLength, mapWidth, rooms, startingRoom, bossRoom);
            createInRoom(rooms, roomMap[currentRoomCord.x][currentRoomCord.y], room, roomWidth, roomLength, playerCord, itemCord, zombieCord);
        }

        exportMap(roomMap, rooms, mapLength, mapWidth);
        // printInRoom(room, roomWidth, roomLength, playerCord, "NULL", inventory, health);
        // gameplayWin(room, roomWidth, roomLength);

        bool endGame = false;
        bool restart = false;

        while (!endGame) {
            std::string inputKey;
            while (inputKey != "q") {
                if (endGame) break;

                printInRoom(room, roomWidth, roomLength, playerCord, inputKey, inventory, health);
                printAtMiddle(msg, roomWidth*3 + 4);
                printAtMiddle(roomMap[currentRoomCord.x][currentRoomCord.y], roomWidth*3 + 4);

                inputKey = getInput();

                clearScreen();
                msg = "";

                if (inputKey == "ESC") {
                    selectionWin(endGame, restart);
                    if (endGame) {
                        saveGame(roomMap, rooms, currentRoomCord, inventory, room, playerCord, itemCord, zombieCord, itemCords, zombieCords, health, bossHealth);
                        break;
                    }
                    else if (restart) {
                        break;
                    }
                }

                // If player is wants to move
                else if (inputKey == "UP" || inputKey == "DOWN" || inputKey == "LEFT" || inputKey == "RIGHT"){
                    Coordinates lastCord = playerCord;
                    moveToNext(room, playerCord, inputKey, " ");
                    facingSide(facing, inputKey);
                    if (lastCord.x == playerCord.x && lastCord.y == playerCord.y) {
                        continue;
                    }
                }

                else if (inputKey == "SPACE" || inputKey == "ENTER") {
                    if (room[playerCord.x+facing.x][playerCord.y+facing.y] == "I") {
                        inventory.emplace_back(rooms[roomMap[currentRoomCord.x][currentRoomCord.y]].item);
                        msg = "You picked up " + rooms[roomMap[currentRoomCord.x][currentRoomCord.y]].item;
                        room[playerCord.x+facing.x][playerCord.y+facing.y] = " ";
                        itemCord = itemCords[roomMap[currentRoomCord.x][currentRoomCord.y]] = {0, 0};
                    }
                    else if (room[playerCord.x+facing.x][playerCord.y+facing.y] == "Z") {
                        for (int i = 0; i < zombieCord.size(); i++) {
                            if (zombieCord[i].x == playerCord.x+facing.x && zombieCord[i].y == playerCord.y+facing.y) {
                                if (facing.x == 1) moveToNext(room, playerCord, "DOWN", "Z");
                                else if (facing.x == -1) moveToNext(room, playerCord, "UP", "Z");
                                else if (facing.y == 1) moveToNext(room, playerCord, "RIGHT", "Z");
                                else if (facing.y == -1) moveToNext(room, playerCord, "LEFT", "Z");
                                zombieCord.erase(zombieCord.begin()+i);
                                msg = "You killed a zombie";
                                health += 5;
                                break;
                            }
                        }
                    }
                }

                else continue;

                // Zombie attack
                if (zombieCord.size() > 0) {
                    for (int i = 0; i < zombieCord.size(); i++) {
                        if (abs(playerCord.x - zombieCord[i].x) + abs(playerCord.y - zombieCord[i].y) == 1) {
                            health -= 5;
                            msg = "You got attacked by a zombie";
                            if (health == 0) {
                                endGame = true;
                                break;
                            }
                        }
                    }
                }

                if (!rooms[roomMap[currentRoomCord.x][currentRoomCord.y]].isStartingRoom && zombieCord.size() > 0) {
                    // for (int i = 0; i < 15; i++) {
                    //     for (int j = 0; j < 15; j++) {
                    //         std::cout << '|' << room[i][j];
                    //     }
                    //     std::cout << std::endl;
                    // }
                    moveZombies(room, zombieCord, playerCord);
                }

                // If player is at the edge of the room
                if (playerCord.x == 0 || playerCord.x == roomWidth+3 || playerCord.y == 0 || playerCord.y == roomLength+3) {
                    clearRoom(room, roomLength, roomWidth);
                    zombieCords[roomMap[currentRoomCord.x][currentRoomCord.y]] = zombieCord;

                    if (playerCord.x == 0) {
                        currentRoomCord.x--;
                        msg = "You travelled North";
                        playerCord.x = roomWidth+2;
                    } else if (playerCord.x == roomWidth+3) {
                        currentRoomCord.x++;
                        msg = "You travelled South";
                        playerCord.x = 1;
                    } else if (playerCord.y == 0) {
                        currentRoomCord.y--;
                        msg = "You travelled West";
                        playerCord.y = roomLength+2;
                    } else if (playerCord.y == roomLength+3) {
                        currentRoomCord.y++;
                        msg = "You travelled East";
                        playerCord.y = 1;
                    }
                    // room[playerCord.x][playerCord.y] = 'P';

                    if (rooms[roomMap[currentRoomCord.x][currentRoomCord.y]].isBossRoom) {
                        bool needSaveGame;
                        bossFightWin(health, inventory.size(), endGame, needSaveGame, bossHealth, restart);
                        if (needSaveGame) {
                            saveGame(roomMap, rooms, currentRoomCord, inventory, room, playerCord, itemCord, zombieCord, itemCords, zombieCords, health, bossHealth);
                        }
                        else if (restart) {
                            break;
                        }
                    }
                    
                    if (rooms[roomMap[currentRoomCord.x][currentRoomCord.y]].item != "") {
                        if (itemCords.find(roomMap[currentRoomCord.x][currentRoomCord.y]) == itemCords.end()) {
                            itemCord = randomCord(room, roomLength, roomWidth); // set item coordinates
                            itemCords[roomMap[currentRoomCord.x][currentRoomCord.y]] = itemCord;
                        } else {
                            itemCord = itemCords[roomMap[currentRoomCord.x][currentRoomCord.y]];
                        }
                    }
                    else itemCord = {0, 0};

                    if (!(rooms[roomMap[currentRoomCord.x][currentRoomCord.y]].isStartingRoom)) {
                        if (zombieCords.find(roomMap[currentRoomCord.x][currentRoomCord.y]) == zombieCords.end()) {
                            spawnZombies(room, roomLength, roomWidth, roomMap[currentRoomCord.x][currentRoomCord.y], zombieCord, zombieCords, itemCord);
                        } else {
                            zombieCord = zombieCords[roomMap[currentRoomCord.x][currentRoomCord.y]];
                        }
                    }
                    else zombieCord = {{0,1}};

                    createInRoom(rooms, roomMap[currentRoomCord.x][currentRoomCord.y], room, roomWidth, roomLength, playerCord, itemCord, zombieCord);
                }
                // std::cin.get();
            }
            // gameOverWin();
            if (restart) break;
        }
        gameOverWin(mode);
    }

    return 0;
}