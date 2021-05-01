#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <string.h>
#include <ncurses.h>
#include "snake.h"

void cursesInit() {
    initscr();
    cbreak(); // no line buffering
    keypad(stdscr, true); //enable keypad and special keys
    noecho(); // Don't print input
    curs_set(0);
}

void clean() {
    nocbreak(); // enable line buffering
    echo();
    endwin();
}

void drawBox(int x, int y) {
    addch(ACS_ULCORNER);
    for (int i = 0; i < x-2; i++) {
        addch(ACS_HLINE);
    }
    addch(ACS_URCORNER);

    for(int i = 0; i < y-2; i++) {
        mvaddch(getcury(stdscr)+1, 0, ACS_VLINE);
        mvaddch(getcury(stdscr), x-1, ACS_VLINE);
    }
    move (getcury(stdscr)+1, 0);
    addch(ACS_LLCORNER);
    for (int i = 0; i < x-2; i++) {
        addch(ACS_HLINE);
    }
    addch(ACS_LRCORNER);
    getch();
}

void displayMenu() {
    int row, col;
    getmaxyx(stdscr, row, col);
    char welcome[] = "Welcome to C++ Snake!";
    char prsAnyKey[] = "Press any key to start";
    char signature[] = "Created by Danylo Rybchynskyi, 2021";
    mvprintw(row/2, (col-strlen(welcome))/2, "%s", welcome);
    mvprintw(row/2-1, (col-strlen(prsAnyKey))/2, "%s", prsAnyKey);
    mvprintw(row-1, 0, "%s", signature);
    getch();
    clear();
}

void snakeDraw(WINDOW* win, Snake& snake) {
    for (BodyPart part : snake.getBody()) {
        mvwaddch(win, part.y, part.x * 2, 'X'); // Multiplied by 2 due width > height
    }
}

void startGame() {
    int input{};
    Snake snake = Snake(d_down, 0, 0);
    drawBox(34, 18); // Draw the border. box and border methods don't work
    WINDOW* arena = newwin(16, 32, 1, 1);
    nodelay(stdscr, true); // to enable timeout
    wrefresh(arena); // to show window
    bool run = true;
    while (run) {
        wclear(arena);
        snakeDraw(arena, snake);
        wrefresh(arena);
        usleep(80000);
        input = getch();
        switch (input) {
            case 259: // Up
                snake.setDirection(d_up);
                break;
            case 258: // Down
                snake.setDirection(d_down);
                break;
            case 260: // Left
                snake.setDirection(d_left);
                break;
            case 261: // Right
                snake.setDirection(d_right);
                break;
            case 27:
                run = false;

        }
        snake.move();
    }
    delwin(arena);
}



int main() {
    cursesInit();
    displayMenu();
    startGame();
    clean();
}
