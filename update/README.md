RUN THE GAME BY:
g++ -pedantic-errors -std=c++11 main.cpp -o test $(ncursesw5-config --cflags --libs) -lncurses -DNCURSES_STACTIC
./test

Run the game once to check the bugs:
- when starting the game by pressing the arrow keys, the exit game may pop up (solved)
- The flee function in boss fights have not been implemented (done)
- half of the boss screen appear sometimes in the in-game menu (solved)
- health = 0% and the game won't end after defecting the boss (solved)
- debug info sudden comes up (solved)
- two game screens appear when starting initially (tested on WSL 2) (solved)
- todo: create save folder (solved)
- After beating the boss and pressing play again (the first time), the map didn't update: The player appeared in the boss room (but the room was empty), when the player entered the boss room again, the boss: 0% HP, the player HP = HP after beating the boss, After beating boss and press play again (2nd time) --> start game bug (solved)
- When no file is in the same folder, choose "Resume Game" and the screen will only display "Saves", not "Displaying no save file found (solved)



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

--------------------------------------------------------------------

# Welcome to DUNGEON GAME !!

## Game Description:
This dungeon game is an immersive experience set in a fantasy world, combining text-based and visual elements. The main objective for the player is to conquer the powerful boss and successfully escape the treacherous dungeon. By issuing commands, the player navigates through the captivating fantasy realm, strategically managing their inventory along the way.

Embarking on their journey, the player finds themselves in the initial room known as the "Liminal Space." Each room within the game is connected through a map called "rooms," which outlines the available movements. This map consists of nested structures associating directions (north, south, east, and west) with specific room names. Certain rooms may contain valuable collectable items, indicated by an "Item" key, while others may harbor a group of zombies, typically ranging from one to three in number.

The game loop persists until the player either emerges victorious against the boss or decides to exit the game. During each iteration of the loop, the game provides pertinent information to the player, including details about their current location, the contents of their inventory, and any messages relevant to their actions and decisions. 

Moreover, the player has the option to resume their progress. This means they can exit the game and save their current state, resuming their adventure from where they left off during subsequent gameplay sessions.

## Compilation and execution instructions:
To compile the program, type:
g++ -pedantic-errors -std=c++11 main.cpp -o dungeon $(ncursesw5-config --cflags --libs) -lncurses -DNCURSES_STACTIC 

To execute the program, type: 
./dungeon

## Game Objective:
The player's objective is to defeat the boss and complete their journey.

## Gameplay Actions:  

During the game, the player may pick up items, beat the zombies, and use the items in inventory to kill the boss.
Navigation and Movement: The player can move around using the navigation keys: ‘W’, ‘S’, ‘A’, ‘D’ keys, representing north, south, west, and east respectively.

Picking Up items: “●” represents nearby items that are available for player collection. The player shall press “Space Bar” to collect the item into inventory.

Killing Zombies: The player shall use navigation keys to point towards the direction of the zombie, then press “SpaceBar” to kill the zombie. 

Boss Battle: will be separately discussed

## Gameplay Mechanics:
Starting Room: The game begins in the "Liminal Space," which is the initial room.

Current Room Visual Map: The game provides a visualized map which shows the room the player is currently staying in. “P” character represents the player’s current position. “Z" character represents the zombie’s current position. “○” represents uncollected items in the room. “●” represents nearby items that are available for player collection.

Game Map: The game map consists of different rooms that the player can move between. Each room may contain valuable collectable items or a group of zombies. The link between different rooms and the position of the boss room is randomized. The player will face a different game map for every gameplay. The blank space around the boundary of each room represents the “door” to exit the current room, which will lead the player to enter a new room.

Saving Progress: The player can save their progress and resume the game later by pressing esc, then select "save and exit". Saved progress includes the player's location, inventory, and other relevant information.

## Hit Points and Interaction with Items and Zombie:
Hit Points (Health): A bar that shows the Hit Points of the player is displayed as follows: Health: ████████ 80%. The player initial starts with 100% hit points. When Health reaches 0%, the player loses. There is no upper limit of the player's health unless when the player is inside the boss room. 

Inventory: The player has an inventory to store collected items, demonstrated using white dots in the game. An inventory consisting of 2 item and maximum capacity of 7 items will show an inventory list as: □ □ ■ ■ ■ ■ ■ .

Picking up an item can increase the Hit Points by 5%, disregarding the initial Hit Points.

Zombie: The player would encounter one to three zombies inevitably in most of the rooms. Zombies will chase after the player, and the player will get attacked by zombies when they are in close proximity.

Being attacked by a zombie would lead to a decrease in Hit Points of 5%. Killing a zombie can increase the Hit Points by 5%, disregarding the initial Hit Points.

## Boss Battle:
The player will encounter a boss and beat it in order to complete their journey. The player needs to strategize and use collected items to defeat the boss.

The players maximum Hit Points will be capped to the Hit Points just before they enter the boss room. If the player's HP = 80% before entering boss room, the maximum health the player can reach after healing is 80%.

Players can choose between 3 options in response to the Boss’ action (Attack, Dodge or Heal) in each turn:
-- Attack: This option would lead to a 10-point decrement of the boss’s hit points. Every item in the inventory would create an additional Attack Point.
50% chance for the boss to attack.

-- Dodge: This option would dodge the boss’ attack to prevent from resulting Hit Points reduction. If the boss does not choose the "Attack" option, nothing will happen.
25% chance for the boss to dodge
      
-- Heal: This option would lead to a 10-point increment of the player’s hit points.
25% chance for the boss to dodge

Super Attack Modes of the Boss:

Emotional Damage Attack and Depression: When the Boss chooses to attack the player using “Emotional Damage Attack”, the player will face “Depression mode”. Under “Depression mode”, the Attack Points of the player are reduced by 1 (Can be superimposed if the boss makes more than 1 “Emotional Damage Attack” during the fight.)

Rage Mode: The boss will be under “Rage mode” if the Hit Points of the boss are more than 120%. Under “Rage Mode”, the Attack Points of the Boss increase by 10.

“Rage Mode” and “Emotional Damage Attack” Mode can be superimposed.

## Challenges and highlights: 
Map: The map of the game would not be shown, which also makes the game challenging. 
Randomized positions: The position of items and zombies are randomized, and the map is also randomly generated, leading to a sense of uncertainty, which makes the game more intriguing. 
