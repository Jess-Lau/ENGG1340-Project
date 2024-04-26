#pragma once

// #include "inRoom.cpp"
#include <ncurses.h>
#include <csignal>
#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <locale.h>
#include <unistd.h>


// Signal handling function
void signalHandler(int signum) {
    // std::cout << "Interrupt signal (" << signum << ") received.\n";

    // Ignore the signal
    signal(signum, SIG_IGN);
}

int printCentre(std::string text, int width) {
    return (width - text.length()) / 2;
}

std::string getInput(){
    initscr();
    cbreak();
    int key = wgetch(stdscr);
    keypad(stdscr, true);
    std::string inputKey;
    switch(key) {
        case 19: inputKey = "SAVE"; break;
        case 119: inputKey = "UP"; break;
        case 97: inputKey = "LEFT"; break;
        case 115: inputKey = "DOWN"; break;
        case 100: inputKey = "RIGHT"; break;
        case KEY_UP: inputKey = "UP"; break;
        case KEY_LEFT: inputKey = "LEFT"; break;
        case KEY_DOWN: inputKey = "DOWN"; break;
        case KEY_RIGHT: inputKey = "RIGHT"; break;
        case 27: inputKey = "ESC"; break;
        case 13: inputKey = "ENTER"; break;
        case 32: inputKey = "SPACE"; break;
    }
    endwin();
    return inputKey;
}

/*
void gameplayWin(std::string room[][15], const int roomWidth, const int roomLength) {
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);

    initscr();
    cbreak();
    noecho();

    WINDOW * gameWin = newwin(13, 37, 0, 1);
    refresh();
    wrefresh(gameWin);
    keypad(gameWin, true);
    start_color();

    for (int i = 1; i < roomWidth+3; i++) {
        for (int j = 0; j < roomLength+4; j++) {
            char mod[1] = {' '};
            if (j == 1 || j == roomWidth + 2) mod[0] = '\0';
            // Player
            if (room[i][j] == "P") {
                attron(COLOR_GREEN);
                wprintw(gameWin, "%sP%s", mod, mod);
                attroff(COLOR_GREEN);
                // ⚉
            }
            // Zombie
            else if (room[i][j] == "Z") {
                attron(COLOR_RED);
                wprintw(gameWin, "%sZ%s", mod, mod);
                attroff(COLOR_RED);
            }
            // item
            else if (room[i][j] == "I") {
                attron(COLOR_YELLOW);
                wprintw(gameWin, "%s□%s", mod, mod);
                attroff(COLOR_YELLOW);
            }
            else if (j > 1 && j < roomLength+2) {
                wprintw(gameWin, "%s%s%s", room[i][j].c_str(), room[i][j].c_str(), room[i][j].c_str());
            }
            else {
                wprintw(gameWin, "%s", room[i][j].c_str());
            }
        }
    }
    wgetch(gameWin);
    endwin();
    // std::cout << std::endl;
}
*/

void menuWin(std::string & mode) {

    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);
    setlocale(LC_ALL, "");
    
    // Ncurse initialization
    initscr();
    cbreak();
    noecho();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int width = 80, height = 13;

    WINDOW * meunWin = newwin(height, width, 0, 1);
    box(meunWin, 0, 0);
    refresh();
    wrefresh(meunWin);

    keypad(meunWin, true); // Enable arrow keys (and other special keys

    std::string title[6] = {
        "██████╗ ██╗   ██╗███╗   ██╗ ██████╗ ███████╗ ██████╗ ███╗   ██╗",
        "██╔══██╗██║   ██║████╗  ██║██╔════╝ ██╔════╝██╔═══██╗████╗  ██║",
        "██║  ██║██║   ██║██╔██╗ ██║██║  ███╗█████╗  ██║   ██║██╔██╗ ██║",
        "██║  ██║██║   ██║██║╚██╗██║██║   ██║██╔══╝  ██║   ██║██║╚██╗██║",
        "██████╔╝╚██████╔╝██║ ╚████║╚██████╔╝███████╗╚██████╔╝██║ ╚████║",
        "╚═════╝  ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═══╝"
    };
    std::vector<std::string> choices = {"Start Game", "Resume Game", "Settings"};
    int choice;
    int highlight = 0;


    while (choice != 27) {
        for (int i = 0; i < 6; i++) {
            mvwprintw(meunWin, i+1, 9, "%s", title[i].c_str());
        }
        mvwprintw(meunWin, 7, printCentre("ESCAPE", width), "ESCAPE");
        for (int i = 0; i < choices.size(); i++) {
            if (i == highlight) {
                wattron(meunWin, A_REVERSE);
            }
            mvwprintw(meunWin, i + 9, printCentre(choices[i], width), choices[i].c_str());
            wattroff(meunWin, A_REVERSE);
        }
        wmove(meunWin, 11, 2);
        refresh();
        choice = wgetch(meunWin);
        switch (choice) {
            case KEY_UP: case 'w': case 'W':
                highlight--;
                if (highlight == -1) {
                    highlight = 0;
                }
                break;
            case KEY_DOWN: case 's': case 'S':
                highlight++;
                if (highlight == choices.size()) {
                    highlight = choices.size()-1;
                }
                break;
            default:
                break;
        }
        if (choice == 10) {
            mode = choices[highlight];
            break;
        }
    }
    if (choice == 27) {
        mvwprintw(meunWin, 11, 69, "Exiting...");
        wrefresh(meunWin);
        sleep(0.01);
        mode = "Exit";
    }
    endwin();
    return;
}

void resumeGameWin(std::string & gameFile) {
    initscr();
    cbreak();
    noecho();

    int width = 80, height = 13;

    WINDOW * resumeWin = newwin(height, width, 0, 1);
    box(resumeWin, 0, 0);
    refresh();
    wrefresh(resumeWin);

    keypad(resumeWin, true); // Enable arrow keys (and other special keys)

    std::vector<std::string> choices;

    DIR * dir;
    struct dirent * ent;
    if ((dir = opendir("save")) != NULL) {
        int i = 1;
        while ((ent = readdir(dir)) != NULL) {
            if (ent->d_name[0] != '.') {
                choices.emplace_back(ent->d_name);
                i++;
            }
        }
        closedir(dir);
    } else {
        mvwprintw(resumeWin, 3, printCentre("No saved files found", width), "No saved files found");
    }

    int choice;
    int highlight = 0;

    mvwprintw(resumeWin, 1, printCentre("Saves", width), "Saves");

    while (choice != 27) {
        for (int i = 0; i < choices.size(); i++) {
            if (i == highlight) {
                wattron(resumeWin, A_REVERSE);
            }
            mvwprintw(resumeWin, i + 3, printCentre(choices[i].substr(0, choices[i].find(".")), width), choices[i].substr(0, choices[i].find(".")).c_str());
            wattroff(resumeWin, A_REVERSE);
            wmove(resumeWin, 11, 2);
        }
        choice = wgetch(resumeWin);
        switch (choice) {
            case KEY_UP: case 'w': case 'W':
                highlight--;
                if (highlight == -1) {
                    highlight = 0;
                }
                break;
            case KEY_DOWN: case 's': case 'S':
                highlight++;
                if (highlight == choices.size()) {
                    highlight = choices.size()-1;
                }
                break;
            default:
                break;
        }
        if (choice == 10) {
            gameFile = choices[highlight];
            break;
        }
    }

    endwin();
}

void settingWin() {
    initscr();
    cbreak();
    noecho();

    int width = 80, height = 13;

    WINDOW * settingWin = newwin(height, width, 0, 1);
    box(settingWin, 0, 0);
    refresh();
    wrefresh(settingWin);

    keypad(settingWin, true); // Enable arrow keys (and other special keys)

    int choice;
    int highlight = 0;

    while (choice != 27) {
        mvwprintw(settingWin, 7, printCentre("Nothing to see", width), "Nothing to see");
        choice = wgetch(settingWin);
        if (choice == 10) {
            break;
        }
    }

    endwin();
}

void makeMove(int & bossHealth, std::string & bossBar, int & health, std::string & healthBar, int highlight, std::string & msg, int damage, int maxHealth, int & depression) {
    int bossDamage = (bossHealth >= 120)? 30:20;
    std::string attackMsg[5] = {
        "The boss has farted (-",
        "You got slapped (-",
        "Your ass got kicked (-",
        "Linux told a horror story (-",
        "Emotional damage (-"
    };
    int bossChoice = rand()%5;
    srand(time(0));
    int bossMove = rand() % 8;
    switch(bossMove) {
        case 0: case 1: case 2: case 3:
            bossMove = 0; // attack
            health -= bossDamage;
            if (health < 0) health = 0;
            // msg = attackMsg[bossChoice] + std::to_string(bossDamage) + "%%HP)";
            msg = (attackMsg[bossChoice] + std::to_string(bossDamage) + "HP)");
            if (bossChoice == sizeof(bossChoice)-1) depression++;
            break;
        case 4: case 5:
            bossMove = 1; // dodge
            break;
        case 6: case 7:
            bossMove = 2; // heal
            bossHealth += 10;
            if (bossHealth > 100) bossHealth == 100;
            msg = "The boss has healed 10HP";
            break;
    }

    switch(highlight) {
        case 0:
            if (bossMove != 1) bossHealth -= damage;
            else msg = "Linux has dogde your attack";
            if (bossHealth < 0) bossHealth = 0;
            break;
        case 1:
            if (bossMove == 0) {
                health += 15;
                msg = "You have dodged the attack!";
            }
            else {
                msg = "The boss didn't attack!";
            }
            break;
        case 2:
            health += 10;
            if (health > maxHealth) {
                health = maxHealth;
                msg = "You have reach max health!";
            }
            break;
        case 3:
            health -= 40;
            if (health < 0) {
                health = 0;
            }
            break;
        default:
            break;
    }
    bossBar = "";
    for (int i = 0; i < bossHealth/10; i++) {
        bossBar += "█";
    }
    if (bossHealth%10 >= 5) {
        bossBar += "▌";
    }
    healthBar = "";
    for (int i = 0; i < health/10; i++) {
        healthBar += "█";
    }
    if (health%10 >= 5) {
        healthBar += "▌";
    }
}

void selectionWin(bool & endGame, bool & restart) {

    // signal(SIGINT, signalHandler);
    // signal(SIGTSTP, signalHandler);
    
    // Ncurse initialization
    initscr();
    cbreak();
    noecho();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int width = 80, height = 13;

    WINDOW * meunWin = newwin(height, width, 0, 1);
    box(meunWin, 0, 0);
    refresh();
    wrefresh(meunWin);

    keypad(meunWin, true); // Enable arrow keys (and other special keys)

    std::vector<std::string> choices = {"Save and Quit", "Restart", "Settings"};
    int choice;
    int highlight = 0;

    while (choice != 27) {
        for (int i = 0; i < choices.size(); i++) {
            if (i == highlight) {
                wattron(meunWin, A_REVERSE);
            }
            mvwprintw(meunWin, i + 1, printCentre(choices[i], width), choices[i].c_str());
            wattroff(meunWin, A_REVERSE);
            wmove(meunWin, 11, 2);
        }
        choice = wgetch(meunWin);
        switch (choice) {
            case KEY_UP: case 'w': case 'W':
                highlight--;
                if (highlight == -1) {
                    highlight = 0;
                }
                break;
            case KEY_DOWN: case 's': case 'S':
                highlight++;
                if (highlight == choices.size()) {
                    highlight = choices.size()-1;
                }
                break;
            default:
                break;
        }
        if (choice == 10) {
            break;
        }
    }

    if (choice == 27) {
        endGame = false;
    } else if (choices[highlight] == "Save and Quit") {
        endGame = true;
    } else if (choices[highlight] == "Restart") {
        restart = true;
    } else if (choices[highlight] == "Settings") {
        settingWin();
    }

    endwin();
}

void bossFightWin(int & health, int extraDamage, bool & endGame, bool & saveGame, int & bossHealth, bool & restart) {
    const int maxHealth = health;
    std::string move;
    int depression = 0;
    
    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);
    setlocale(LC_ALL, "");

    initscr();
    noecho();
    cbreak();

    int width = 80, height = 24;

    WINDOW * bossWin = newwin(height, width, 0, 1);
    box(bossWin, 0, 0);
    mvwprintw(bossWin, 2, printCentre("Boss Encounter", width), "Boss Encounter");
    wrefresh(bossWin);
    sleep(2);
    wrefresh(bossWin);

    keypad(bossWin, true); // Enable arrow keys (and other special keys)

    std::string bossImage[17] = {
        "               +$&&&&$;               ",
        "             &&&&&&&&&&&&+            ",
        "            :&$$&&&$xx$&&$            ",
        "            :$.⊚ .: ⊚ .&&&            ",
        "            .&:..    ::&&&.           ",
        "             &+:...:::;&xX$           ",
        "            :& .;;;.   +&&&$          ",
        "           x$           +&&&$.        ",
        "        ;&&X             .$&&&&$      ",
        "        &&x                &$$&&&     ",
        "      +$&:                  &&X&&X    ",
        "     ++xX                   &&&$XX    ",
        "    .:..:XX               ::&&&&&:    ",
        " :........x&X.           .:.;xx;...   ",
        " :..........:          ;&X:.........::",
        "::...........;X;    +&&&&+:.....::;:  ",
        "        :;;;+;           :xx++:       "
    };
    std::string player = "•ᴗ•";

    std::vector<std::string> choices = {"Attack ⚔ ", "Dodge", "Heal ❤ ", "Flee 🏃 "};
    std::string bossBar = "", healthBar = "";

    keypad(bossWin, true); // Enable arrow keys (and other special keys)

    int choice;
    int highlight = 0;
    std::string msg = "";

    for (int i = 0; i < bossHealth/10; i++) {
        bossBar += "█";
    }
    if (bossHealth%10 >= 5) {
        bossBar += "▌";
    }
    for (int i = 0; i < health/10; i++) {
        healthBar += "█";
    }
    if (bossHealth%10 >= 5) {
        healthBar += "▌";
    }

    while (choice != 27) {
        
        for (int i = 0; i < 17; i++) {
            mvwprintw(bossWin, i+1, 40, "%s", bossImage[i].c_str());
        }
        mvwprintw(bossWin, 2, 35, "Boss: Linux");
        mvwprintw(bossWin, 3, 35, "                ");
        mvwprintw(bossWin, 3, 35, "%s %d%%", bossBar.c_str(), bossHealth);
        if (bossHealth >= 120) mvwprintw(bossWin, 4, 35, "(RAGE)");
        if (depression > 0) mvwprintw(bossWin, 12, 2, "Depression +%d%%", depression);
        mvwprintw(bossWin, 13, 2, "ENGG1340 Victim");
        mvwprintw(bossWin, 14, 2, "                 ");
        mvwprintw(bossWin, 14, 2, "%s %d%", healthBar.c_str(), health);
        mvwprintw(bossWin, 15, 5, "%s", player.c_str());

        for (int i = 0; i < choices.size(); i++) {
            if (i == highlight) {
                wattron(bossWin, A_REVERSE);
            }
            mvwprintw(bossWin, i + 18, 3, choices[i].c_str());
            wattroff(bossWin, A_REVERSE);
            if (i == highlight && i == 0) { msg = "Deal " + std::to_string(10+extraDamage-depression) + " hit point to the boss"; }
            else if (i == highlight && i == 1) { msg = "Dodge the boss's attack"; }
            else if (i == highlight && i == 2) { msg = "Heal 10 hit point"; }
            else if (i == highlight && i == 3) { msg = "Flee the battle (-40HP)"; }
            
            mvwprintw(bossWin, 20, 44, "                                ");
            mvwprintw(bossWin, 20, 44, "%s", msg.c_str());
            wrefresh(bossWin);
            wmove(bossWin, 1, 1);
        }
        choice = wgetch(bossWin);
        switch (choice) {
            case KEY_UP: case 'w': case 'W':
                highlight--;
                if (highlight == -1) {
                    highlight = 0;
                }
                break;
            case KEY_DOWN: case 's': case 'S':
                highlight++;
                if (highlight == choices.size()) {
                    highlight = choices.size()-1;
                }
                break;
            default:
                break;
        }
        if (choice == 10) {
            makeMove(bossHealth, bossBar, health, healthBar, highlight, msg, 10+extraDamage-depression, maxHealth, depression);
            mvwprintw(bossWin, 20, 44, "                              ");
            mvwprintw(bossWin, 20, 44, "%s", msg.c_str());
            wrefresh(bossWin);
            wmove(bossWin, 1, 1);
            sleep(1.5);
        }
        else if (choice == 27) {
            wclear(bossWin);
            wrefresh(bossWin);
            selectionWin(endGame, restart);
            if (endGame) {
                saveGame = true;
                break;
            }
            choice == 0;
            continue;
        }
        if (bossHealth == 0) {
            mvwprintw(bossWin, 3, 35, "               ");
            mvwprintw(bossWin, 20, 44, "                              ");
            mvwprintw(bossWin, 20, 44, "You have defeated the boss!");
            wrefresh(bossWin);
            sleep(2);
            std::string deathMsg[] = {
                "\"You thought it is the end,                 ",
                "but it is just the beginning.               ",
                "My brother Java is waiting for you.         ",
                "Ha Ha Ha HA!\"                               "
            };
            for (int i = 0; i < 4; i++) {
                mvwprintw(bossWin, 2+i, 3, "%s", deathMsg[i].c_str());
                wrefresh(bossWin);
                sleep(2);
            }
            wclear(bossWin);
            mvwprintw(bossWin, 20, 2, "Depression +100%");
            wrefresh(bossWin);
            sleep(4);
            endGame = true;
            break;
        }
        else if (health == 0) {
            mvwprintw(bossWin, 22, 2, "You have been defeated by the boss!");
            wrefresh(bossWin);
            sleep(2);
            endGame = true;
            break;
        }
    }


    endwin();
}

void gameOverWin(std::string & mode) {

    signal(SIGINT, signalHandler);
    signal(SIGTSTP, signalHandler);
    setlocale(LC_ALL, "");
    
    // Ncurse initialization
    initscr();
    cbreak();
    noecho();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int width = 80, height = 13;

    WINDOW * overWin = newwin(height, width, 0, 1);
    box(overWin, 0, 0);
    refresh();
    wrefresh(overWin);

    keypad(overWin, true); // Enable arrow keys (and other special keys

    std::string title[6] = {
        "██████╗ ██╗   ██╗███╗   ██╗ ██████╗ ███████╗ ██████╗ ███╗   ██╗",
        "██╔══██╗██║   ██║████╗  ██║██╔════╝ ██╔════╝██╔═══██╗████╗  ██║",
        "██║  ██║██║   ██║██╔██╗ ██║██║  ███╗█████╗  ██║   ██║██╔██╗ ██║",
        "██║  ██║██║   ██║██║╚██╗██║██║   ██║██╔══╝  ██║   ██║██║╚██╗██║",
        "██████╔╝╚██████╔╝██║ ╚████║╚██████╔╝███████╗╚██████╔╝██║ ╚████║",
        "╚═════╝  ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═══╝"
    };
    std::vector<std::string> choices = {"Play Again", "Quit", "Settings"};
    int choice;
    int highlight = 0;


    while (choice != 27) {
        for (int i = 0; i < 6; i++) {
            mvwprintw(overWin, i+1, 9, "%s", title[i].c_str());
        }
        mvwprintw(overWin, 7, printCentre("ESCAPE", width), "ESCAPE");
        for (int i = 0; i < choices.size(); i++) {
            if (i == highlight) {
                wattron(overWin, A_REVERSE);
            }
            mvwprintw(overWin, i + 9, printCentre(choices[i], width), choices[i].c_str());
            wattroff(overWin, A_REVERSE);
        }
        wmove(overWin, 11, 2);
        refresh();
        choice = wgetch(overWin);
        switch (choice) {
            case KEY_UP: case 'w': case 'W':
                highlight--;
                if (highlight == -1) {
                    highlight = 0;
                }
                break;
            case KEY_DOWN: case 's': case 'S':
                highlight++;
                if (highlight == choices.size()) {
                    highlight = choices.size()-1;
                }
                break;
            default:
                break;
        }
        if (choice == 10) {
            break;
        }
    }

    if (choice == 27) {
    } else if (choices[highlight] == "Play Again") {
        mode = "Start Game";
    } else if (choices[highlight] == "Quit") {
        mode = "Quit";
    } else if (choices[highlight] == "Settings") {
        settingWin();
    }

    endwin();
    return;
}

// g++ selectionWindow.cpp -o selectionWindow.exe -lncurses -DNCURSES_STATIC
// g++ -pedantic-errors -std=c++11 selectionWindow.cpp -o test -lncurses -DNCURSES_STACTIC ; ./test
// g++ -pedantic-errors -std=c++11 inRoom.cpp -o test $(ncursesw5-config --cflags --libs) -lncurses -DNCURSES_STACTIC