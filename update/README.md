RUN THE GAME BY:
g++ -pedantic-errors -std=c++11 main.cpp -o test $(ncursesw5-config --cflags --libs) -lncurses -DNCURSES_STACTIC ; ./test

Run the game once to check the bugs:
- when start the game by pressing arrow keys, the exit game meun pop up
- flee function in boss fight have not implemented
- half of boss screen appear sometimes in in-game menu
- health = 0% and game won't end after defected the boss
- debug info suddent comes up
- two game screens appear when start initially (tested on WSL 2)
- todo: create save folder
- After beating boss and press play again (first time), the map didn't update: Player appeared in the boss room (but the room is empty), when the player enters the boss room again, boss: 0% HP, player HP = HP after beating the boss, After beating boss and press play again (2nd time) --> start game bug
- When no file is in the save folder, choose "Resume Game" and the screen will only display "Saves", not "Displaying no save file found



# ENGG1340-Group
Members:
1. UID: 
2. UID: 
3. UID:
4. UID:
5. UID: 

Project instructions:
• In the same Github repo that you set up in Stage 1.
o Create a README.MD file in the repo that contains:
▪ Identification of the team members.
▪ A description of your game and introduce the game rules.
▪ A list of features that you have implemented and explain how each coding element
1 to 5 listed under the coding requirements aforementioned support your features.
▪ A list of non-standard C/C++ libraries, if any, that are used in your work and integrated
to your code repo. Please also indicate what features in your game are supported by
these libraries.
▪ Compilation and execution instructions. This serves like a "Quick start" of your game.
The teaching team will follow your instructions to compile and run your game.
o Your code including the makefile and all source files (.h / .cpp / .c)
• Submit a link to the repo to Moodle. (Refer to the details on the submission page)
• A video (at most 3 minutes long) demonstrating a gameplay and the implemented features of
your program.
o You can either submit a link to the video or upload it to Moodle, and add that in the
README.MD file.
o Video clips after 3 mins would be ignored if your demo is longer than 3 minutes.
What to submit (by each individual student)
• Submit on Moodle (Refer to the details on the submission page)
• A list of work that you have done for the project.
• A peer evaluation of your project partners in your group.
• To declare your own contribution with explanations if the Contributions Graph in GitHub does
not reflect the real contribution distribution.

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




