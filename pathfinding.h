#pragma once

// #include "inRoom.h"

bool isInPath(std::vector<Coordinates> path, Coordinates zombieCord) {
    for (int i = 0; i < path.size(); i++) {
        if (path[i].x == zombieCord.x && path[i].y == zombieCord.y) {
            return true;
        }
    }
    return false;
}

void pathFinding(int roomSteps[][11], int steps, std::vector<Coordinates> path, Coordinates zombieCord, Coordinates playerCord, std::vector<Coordinates> & closestPath, int & minimumSteps) {

    if (path.size() >= 1 && path.rbegin()[0].x == path.rbegin()[1].x && path.rbegin()[0].y == path.rbegin()[1].y) {
        path.pop_back();
    }

    if (zombieCord.x == playerCord.x && zombieCord.y == playerCord.y) {
        minimumSteps = steps;
        closestPath.clear();
        closestPath = path;
        return;
    }

    if (!(closestPath.empty()) && steps > minimumSteps) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        if (i == 0 && zombieCord.x-1 >= 0 && roomSteps[zombieCord.x-1][zombieCord.y] != -1) {
            if (!(isInPath(path, Coordinates({zombieCord.x-1, zombieCord.y}))) && roomSteps[zombieCord.x-1][zombieCord.y] > steps+1) {
                roomSteps[zombieCord.x-1][zombieCord.y] = steps+1;
                path.emplace_back(zombieCord);
                pathFinding(roomSteps, steps+1, path, {zombieCord.x-1, zombieCord.y}, playerCord, closestPath, minimumSteps);
            }
        }
        else if (i == 1 && zombieCord.x+1 < 11 && roomSteps[zombieCord.x+1][zombieCord.y] != -1) {
            if (!(isInPath(path, Coordinates({zombieCord.x+1, zombieCord.y}))) && roomSteps[zombieCord.x+1][zombieCord.y] > steps+1) {
                roomSteps[zombieCord.x+1][zombieCord.y] = steps+1;
                path.emplace_back(zombieCord);
                pathFinding(roomSteps, steps+1, path, {zombieCord.x+1, zombieCord.y}, playerCord, closestPath, minimumSteps);
            }
        }
        else if (i == 2 && zombieCord.y-1 >= 0 && roomSteps[zombieCord.x][zombieCord.y-1] != -1) {
            if (!(isInPath(path, Coordinates({zombieCord.x, zombieCord.y-1}))) && roomSteps[zombieCord.x][zombieCord.y-1] > steps+1) {
                roomSteps[zombieCord.x][zombieCord.y-1] = steps+1;
                path.emplace_back(zombieCord);
                pathFinding(roomSteps, steps+1, path, {zombieCord.x, zombieCord.y-1}, playerCord, closestPath, minimumSteps);
            }
        }
        else if (i == 3 && zombieCord.y+1 < 11 && roomSteps[zombieCord.x][zombieCord.y+1] != -1) {
            if (!(isInPath(path, Coordinates({zombieCord.x, zombieCord.y+1}))) && roomSteps[zombieCord.x][zombieCord.y+1] > steps+1) {
                roomSteps[zombieCord.x][zombieCord.y+1] = steps+1;
                path.emplace_back(zombieCord);
                pathFinding(roomSteps, steps+1, path, {zombieCord.x, zombieCord.y+1}, playerCord, closestPath, minimumSteps);
            }
        }
    }
    path.pop_back();

    // for (int i = 0; i < 11; i++) {
    //     for (int j = 0; j < 11; j++) {
    //         std::cout << std::setw(5) << roomSteps[i][j];
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << "----------------------------------------------" << std::endl;
}

void checkPath(int roomSteps[][11], Coordinates zombieCord, Coordinates playerCord, std::vector<Coordinates> & closestPath) {

    roomSteps[zombieCord.x][zombieCord.y] = 0;

    int steps = 0;
    std::vector<Coordinates> path;
    int minimumSteps = 11*11;

    pathFinding(roomSteps, steps, path, zombieCord, playerCord, closestPath, minimumSteps);
    
    // for (int i = 0; i < 11; i++) {
    //     for (int j = 0; j < 11; j++) {
    //         std::cout << std::setw(5) << roomSteps[i][j];
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << "----------------------------------------------" << std::endl;
    
}

void moveZombies(std::string room[][15], std::vector<Coordinates> & zombieCords, Coordinates playerCord) {
    playerCord = {playerCord.x-2, playerCord.y-2};

    for (int i = 0; i < zombieCords.size(); i++) {
        int roomForPathfinding[11][11];
        for (int i = 2; i < 13; i++) {
            for (int j = 2; j < 13; j++) {
                if (room[i][j] == "P") {
                    roomForPathfinding[i-2][j-2] = 11*11 + 1;
                }
                else if (room[i][j] == " ") {
                    roomForPathfinding[i-2][j-2] = 11*11;
                }
                else {
                    roomForPathfinding[i-2][j-2] = -1;
                }
            }
        }
        std::vector<Coordinates> closestPath;
        checkPath(roomForPathfinding, Coordinates{zombieCords[i].x-2, zombieCords[i].y-2}, playerCord, closestPath);
        // std::cout << closestPath.size() << std::endl;
        if (closestPath.size() > 0) {
            for (;closestPath[0].x == closestPath[1].x && closestPath[0].y == closestPath[1].y;) {
                // std::cout << "CHECK" << std::endl;
                closestPath.erase(closestPath.begin());
                if (closestPath.size() == 1) break;
            }
            // for (Coordinates cord : closestPath) {
            //     std::cout << '{' << cord.x << ',' << cord.y << "}, ";
            // }
            // std::cout << std::endl;
            // std::cout << closestPath[1].x << ' ' << (zombieCords[i].x-2) << ' ' << closestPath[1].y << ' ' << (zombieCords[i].y-2) << std::endl;
            if (closestPath.size() > 1 && abs(closestPath[1].x - (zombieCords[i].x-2)) + abs(closestPath[1].y - (zombieCords[i].y-2)) == 1) {
                // std::cout << "IN" << std::endl;
                std::string intputKey = "";
                if (closestPath[1].x - (zombieCords[i].x-2) == 1) intputKey = "DOWN";
                else if (closestPath[1].x - (zombieCords[i].x-2) == -1) intputKey = "UP";
                else if (closestPath[1].y - (zombieCords[i].y-2) == 1) intputKey = "RIGHT";
                else if (closestPath[1].y - (zombieCords[i].y-2) == -1) intputKey = "LEFT";
                moveToNext(room, zombieCords[i], intputKey, " ");
                // room[zombieCords[i].x][zombieCords[i].y] = ' ';
                // room[closestPath[0][1].x+2][closestPath[0][1].y+2] = 'Z';
                // zombieCords[i] = {closestPath[0][1].x+2, closestPath[0][1].y+2};
            }
        }
    }
}
