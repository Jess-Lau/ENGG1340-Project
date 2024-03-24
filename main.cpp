#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <string>
#include <algorithm>

// For simplicity, replacing the PIL and os functions with dummy functions
void showImage(const std::string& filename) {
    std::cout << "Displaying image: " << filename << std::endl;
}

void clear() {
    system("clear"); // Assuming Unix-like systems
}

void prompt() {
    std::cout << "\t\tWelcome to my game\n\n"
                 "You must collect all six items before fighting the boss.\n\n"
                 "Moves:\t'go {direction}' (travel north, south, east, or west)\n"
                 "\t'get {item}' (add nearby item to inventory)\n"
                 << std::endl;

    std::cout << "Press any key to continue...";
    std::cin.ignore(); // Wait for user input
}

int main() {
    // Show map
    showImage("Map.png");

    // Display starting menu
    clear();
    prompt();

    // Map
    std::map<std::string, std::map<std::string, std::string>> rooms = {
        {"Liminal Space", {{"North", "Mirror Maze"}, {"South", "Bat Cavern"}, {"East", "Bazaar"}}},
        {"Mirror Maze", {{"South", "Liminal Space"}, {"Item", "Crystal"}}},
        {"Bat Cavern", {{"North", "Liminal Space"}, {"East", "Volcano"}, {"Item", "Staff"}}},
        {"Bazaar", {{"West", "Liminal Space"}, {"North", "Meat Locker"}, {"East", "Dojo"}, {"Item", "Altoids"}}},
        {"Meat Locker", {{"South", "Bazaar"}, {"East", "Quicksand Pit"}, {"Item", "Fig"}}},
        {"Quicksand Pit", {{"West", "Meat Locker"}, {"Item", "Robe"}}},
        {"Volcano", {{"West", "Bat Cavern"}, {"Item", "Elderberry"}}},
        {"Dojo", {{"West", "Bazaar"}, {"Boss", "Shadow Man"}}}
    };

    // List to track inventory
    std::vector<std::string> inventory;

    // Tracks current room
    std::string current_room = "Liminal Space";

    // Tracks last move
    std::string msg = "";

    while (true) {
        clear();

        // Display player info
        std::cout << "You are in the " << current_room << "\nInventory : ";
        for (const auto& item : inventory) {
            std::cout << item << " ";
        }
        std::cout << "\n---------------------------\n";

        // Display msg
        std::cout << msg << std::endl;

        // Item indicator
        if (rooms[current_room].count("Item")) {
            std::string nearby_item = rooms[current_room]["Item"];
            if (std::find(inventory.begin(), inventory.end(), nearby_item) == inventory.end()) {
                if (nearby_item.back() == 's') {
                    std::cout << "You see " << nearby_item << std::endl;
                } else if (std::find(vowels.begin(), vowels.end(), nearby_item[0]) != vowels.end()) {
                    std::cout << "You see an " << nearby_item << std::endl;
                } else {
                    std::cout << "You see a " << nearby_item << std::endl;
                }
            }
        }

        // Boss encounter
        if (rooms[current_room].count("Boss")) {
            if (inventory.size() < 6) {
                std::cout << "You lost a fight with " << rooms[current_room]["Boss"] << "." << std::endl;
                break;
            } else {
                std::cout << "You beat " << rooms[current_room]["Boss"] << "!" << std::endl;
                break;
            }
        }

        // Accepts player's move as input
        std::cout << "Enter your move:" << std::endl;
        std::string user_input;
        std::getline(std::cin, user_input);

        // Splits move into words
        std::string action, item, direction;
        size_t space_pos = user_input.find(' ');
        if (space_pos != std::string::npos) {
            action = user_input.substr(0, space_pos);
            item = user_input.substr(space_pos + 1);
        } else {
            action = user_input;
        }

        // Moving between rooms
        if (action == "go" || action == "Go") {
            direction = item;
            try {
                current_room = rooms[current_room][direction];
                msg = "You travel " + direction;
            } catch (...) {
                msg = "You can't go that way.";
            }
        }
        // Picking up items
        else if (action == "get" || action == "Get") {
            try {
                if (item == rooms[current_room]["Item"]) {
                    if (std::find(inventory.begin(), inventory.end(), item) == inventory.end()) {
                        inventory.push_back(rooms[current_room]["Item"]);
                        msg = item + " retrieved!";
                    } else {
                        msg = "You already have the " + item;
                    }
                } else {
                    msg = "Can't find " + item;
                }
            } catch (...) {
                msg = "Can't find " + item;
            }
        }
        // Exit program
        else if (action == "exit" || action == "Exit") {
            break;
        }
        // Any other commands invalid
        else {
            msg = "Invalid command";
        }
    }

    return 0;
}

