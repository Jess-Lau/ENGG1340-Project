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
