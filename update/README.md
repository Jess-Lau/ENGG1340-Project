RUN THE GAME BY:
g++ -pedantic-errors -std=c++11 inRoom.cpp -o test $(ncursesw5-config --cflags --libs) -lncurses -DNCURSES_STACTIC ; ./test

Run the game once to check the bugs:
- when start the game by pressing arrow keys, the exit game meun pop up
- flee function in boss fight have not implemented
- 
