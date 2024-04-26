RUN THE GAME BY:
g++ -pedantic-errors -std=c++11 inRoom.cpp -o test $(ncursesw5-config --cflags --libs) -lncurses -DNCURSES_STACTIC ; ./test

Run the game once to check the bugs:
- when start the game by pressing arrow keys, the exit game meun pop up
- flee function in boss fight have not implemented
- half of boss screen appear sometimes in in-game menu
- health = 0% and game won't end after defected the boss
- debug info suddent comes up
- two game screens appear when start initially (tested on WSL 2)
- todo: create save folder
- After beating boss and press play again (first time), the map didn't update: Player appeared in the boss room (but the room is empty), when the player enters the boss room again, boss: 0% HP, player HP = HP after beating the boss, After beating boss and press play again (2nd time) --> start game bug



# ENGG1340-Group
Members:
1. UID: 
2. UID: 
3. UID:
4. UID:
5. UID: 

--------------------------------------------------------------------

# Welcome to DUNGEON GAME !!

## Game Description:
This dungeon game is an immersive experience set in a fantasy world, combining text-based and visual elements. The main objective for the player is to conquer the powerful boss and successfully escape the treacherous dungeon. By issuing commands, the player navigates through the captivating fantasy realm, strategically managing their inventory along the way.

Embarking on their journey, the player finds themselves in the initial room known as the "Liminal Space." Each room within the game is connected through a map called "rooms," which outlines the available movements. This map consists of nested structures associating directions (north, south, east, and west) with specific room names. Certain rooms may contain valuable collectible items, indicated by an "Item" key, while others may harbor a group of zombies, typically ranging from one to three in number.

The game loop persists until the player either emerges victorious against the boss or decides to exit the game. During each iteration of the loop, the game provides pertinent information to the player, including details about their current location, the contents of their inventory, and any messages relevant to their actions and decisions. 

Moreover, the player has the option to resume their progress. This means they can exit the game and save their current state, resuming their adventure from where they left off during subsequent gameplay sessions.

## Compilation and execution instructions:
To compile the program, type:
g++ -pedantic-errors -std=c++11 inRoom.cpp -o test $(ncursesw5-config --cflags --libs) -lncurses -DNCURSES_STACTIC 

To execute the program, type: 
./test

## Features implemented:




