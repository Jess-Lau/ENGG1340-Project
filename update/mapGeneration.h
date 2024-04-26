#pragma once

void createRooms(std::map<std::string, Rooms> & room) {
    room["Liminal Space"] = Rooms {"Liminal Space", "", 1, 0};
    room["Mirror Maze"] = Rooms {"Mirror Maze", "Crystal", 0, 0};
    room["Meat Locker"] = Rooms {"Meat Locker", "Fig Newtoon", 0, 0};
    room["Quicksand"] = Rooms {"Quicksand", "Robe", 0, 0};
    room["Bazaar"] = Rooms {"Bazaar", "Altoids", 0, 0};
    room["Dojo"] = Rooms {"Dojo", "", 0, 1};
    room["Bat Cavern"] = Rooms {"Bat Cavern", "Staff",0 ,0};
    room["Volcano"] = Rooms {"Volcano", "Syrup",0 ,0};
}

bool roomIsAvailable(std::string roomMap[][7], const int roomLength, const int roomWidth, Coordinates coordinate) {
    if ((coordinate.x < 0 || coordinate.x >= roomLength || coordinate.y < 0 || coordinate.y >= roomWidth) || (roomMap[coordinate.x][coordinate.y] != "")) { // check if the coordinate is out of bounds // check if the room is already occupied
        return false;
    }
    return true;
}

void placeRoom(std::map<std::string, Rooms> & rooms, std::string roomMap[][7], std::vector<std::string> &roomsNotGenerated, Coordinates coordinate, const int roomLength, const int roomWidth, int direction, std::string currentRoom, std::vector<Coordinates> &nextCordinates, int randomRoom) {
    switch (direction) {
        case 0: // Up
            coordinate.x -= 1;
            if (roomIsAvailable(roomMap, roomLength, roomWidth, coordinate)) {
                roomMap[coordinate.x][coordinate.y] = roomsNotGenerated[randomRoom];
                rooms[currentRoom].north = roomsNotGenerated[randomRoom];
                rooms[roomsNotGenerated[randomRoom]].south = currentRoom;
                roomsNotGenerated.erase(roomsNotGenerated.begin() + randomRoom);
                Coordinates newCoordinate = {coordinate.x, coordinate.y};
                nextCordinates.emplace_back(newCoordinate);
            }
            break;
        case 1: // Down
            coordinate.x += 1;
            if (roomIsAvailable(roomMap, roomLength, roomWidth, coordinate)) {
                roomMap[coordinate.x][coordinate.y] = roomsNotGenerated[randomRoom];
                rooms[currentRoom].south = roomsNotGenerated[randomRoom];
                rooms[roomsNotGenerated[randomRoom]].north = currentRoom;
                roomsNotGenerated.erase(roomsNotGenerated.begin() + randomRoom);
                Coordinates newCoordinate = {coordinate.x, coordinate.y};
                nextCordinates.emplace_back(newCoordinate);
            }
            break;
        case 2: // Left
            coordinate.y -= 1;
            if (roomIsAvailable(roomMap, roomLength, roomWidth, coordinate)) {
                roomMap[coordinate.x][coordinate.y] = roomsNotGenerated[randomRoom];
                rooms[currentRoom].west = roomsNotGenerated[randomRoom];
                rooms[roomsNotGenerated[randomRoom]].east = currentRoom;
                roomsNotGenerated.erase(roomsNotGenerated.begin() + randomRoom);
                Coordinates newCoordinate = {coordinate.x, coordinate.y};
                nextCordinates.emplace_back(newCoordinate);
            }
            break;
        case 3: // Right
            coordinate.y += 1;
            if (roomIsAvailable(roomMap, roomLength, roomWidth, coordinate)) {
                roomMap[coordinate.x][coordinate.y] = roomsNotGenerated[randomRoom];
                rooms[currentRoom].east = roomsNotGenerated[randomRoom];
                rooms[roomsNotGenerated[randomRoom]].west = currentRoom;
                roomsNotGenerated.erase(roomsNotGenerated.begin() + randomRoom);
                Coordinates newCoordinate = {coordinate.x, coordinate.y};
                nextCordinates.emplace_back(newCoordinate);
            }
            break;
        default:
            std::cout << "Error: Invalid direction" << std::endl;
            break;
    }
}

void spreadRoom(std::map<std::string, Rooms> & rooms, std::string roomMap[][7], std::vector<std::string> &roomsNotGenerated, Coordinates coordinate, const int roomLength, const int roomWidth, std::string bossRoom, bool &bossRoomPlaced) {
    if (roomsNotGenerated.size() == 0) {
        return;
    }

    int lastDirection = -1;
    std::vector<Coordinates> nextCordinates;
    nextCordinates.reserve(6);

    for (int i = 0; i < 4 && roomsNotGenerated.size() > 0; i++) {
        int direction = rand() % 4;
        if (lastDirection == direction) {
            continue;
        }

        std::string currentRoom = roomMap[coordinate.x][coordinate.y];
        int randomRoom = std::rand() % roomsNotGenerated.size();
        // std::cout << roomsNotGenerated.size() << ' ' << randomRoom << std::endl;
        placeRoom(rooms, roomMap, roomsNotGenerated, coordinate, roomLength, roomWidth, direction, currentRoom, nextCordinates, randomRoom);
        // printMap(roomMap, roomLength, roomWidth);

        lastDirection = direction;

        if (roomsNotGenerated.size() == 0 && !bossRoomPlaced) {
            roomsNotGenerated.emplace_back(bossRoom);
            bossRoomPlaced = true;
        }
    }
    nextCordinates.shrink_to_fit();
    for (int i = 0; i < nextCordinates.size(); i++) {
        // std::cout << nextCordinates[i].x << ' ' << nextCordinates[i].y << std::endl;
        spreadRoom(rooms, roomMap, roomsNotGenerated, nextCordinates[i], roomLength, roomWidth, bossRoom, bossRoomPlaced);
    }
}

void generateMap(std::string roomMap[][7], const int roomLength, const int roomWidth, std::map<std::string, Rooms> & rooms, std::string & startingRoom, std::string & bossRoom) {
    std::srand(time(NULL));
    createRooms(rooms);
    int numOfRooms = rooms.size();

    // find the starting room and boss room
    std::vector<std::string> roomsNotGenerated;
    roomsNotGenerated.reserve(numOfRooms);

    for (auto room : rooms) {
        if (room.second.isStartingRoom) startingRoom = room.first;
        else if (room.second.isBossRoom) bossRoom = room.first;
        else roomsNotGenerated.emplace_back(room.first);
    }
    // std::cout << startingRoom << " --> " << bossRoom << std::endl;
    
    Coordinates coordinate = {roomLength/2, roomWidth/2}; // starting room coordinate
    roomMap[coordinate.x][coordinate.y] = startingRoom; // set the Starting Room at the centre
    bool bossRoomPlaced = false;
    while (roomsNotGenerated.size() > 0) {
        spreadRoom(rooms, roomMap, roomsNotGenerated, coordinate, roomLength, roomWidth, bossRoom, bossRoomPlaced);
    }

    printMap(roomMap, roomLength, roomWidth);
}

void exportMap(std::string roomMap[][7], std::map<std::string, Rooms> rooms, const int roomLength, const int roomWidth) {
    std::string mapFile = "map.txt";
    std::ofstream fout;
    fout.open(mapFile.c_str());
    if (fout.fail()) {
        std::cout << "Error: Unable to create file: " << mapFile << std::endl;
    }
    else {
        // reduceMap(roomMap, roomLength, roomWidth);
        for (int i = 0; i < roomLength; i++) {
            for (int j = 0; j < roomWidth; j++) {
                std::string mod = "";
                if (rooms[roomMap[i][j]].east != "") mod = " ——— ";
                fout << std::setw(15) << roomMap[i][j] << std::setw(5) << mod;
            }
            fout << std::endl;
            for (int j = 0; j < roomWidth; j++) {
                std::string mod = "";
                if (rooms[roomMap[i][j]].south != "") mod = "| ";
                fout << std::setw(15) << mod << std::setw(5) << ' ';
            }
            fout << std::endl;
        }
        fout.close();
    }
}

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

bool isVowel(std::string name) {
    return (name[0] == 'a' || name[0] == 'e' || name[0] == 'i' || name[0] == 'o' || name[0] == 'u');
}

std::string toLower(std::string str) {
    for (int i = 0; i < str.size(); i++) {
        str[i] = std::tolower(str[i]);
    }
    return str;
}

void printAtMiddle(std::string str, int width) {
    int length = str.size();
    int start = (width - length) / 2;
    std::cout << std::setw(start + length) << str << std::endl;
}