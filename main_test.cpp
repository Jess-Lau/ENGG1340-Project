#include <iostream>
// using namespace std;

void prompt() {
    std::cout << "\t\tWelcome to my game\n\n"
                 "You must collect all six items before fighting the boss.\n\n"
                 "Moves:\t'go {direction}' (travel north, south, east, or west)\n"
                 "\t'get {item}' (add nearby item to inventory)\n"
                 << std::endl;

    std::cout << "Press any key to continue...";
    std::cin.ignore(); // Wait for user input

int main() {

}