#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv)
{
        // Positions for the game window
        int height, width;
        WINDOW *mainwin;

        // Seed the random number generator        
        srand(time(0));

        // Initialise ncurses
        if ((mainwin = initscr()) == NULL) {
                fprintf(stderr, "Error initialising ncurses.\n");
                exit(EXIT_FAILURE);
        }
        noecho();
        keypad(mainwin, TRUE);

        getmaxyx(mainwin, height, width);
        refresh();

        int c;
        int x = 0;
        int y = 0;
        move(y,x);
        int go = 1;
        while (go) {
                getmaxyx(mainwin, height, width);
                c = wgetch(mainwin);
                switch (c) {
                case KEY_UP:
                        if (y>0)
                                y--;
                        break;
                case KEY_DOWN:
                        if (y<height-1)
                                y++;
                        break;
                case KEY_LEFT:
                        if (x>0)
                                x--;
                        break;
                case KEY_RIGHT:
                        if (x<width-1)
                                x++;
                        break;
                case 'q':
                        go = 0;
                        break;
                }
                move(y,x);
                refresh();
        }

        // Clean up after ourselves
        delwin(mainwin);
        endwin();
        refresh();

        return EXIT_SUCCESS;
}
