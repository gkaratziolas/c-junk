#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <time.h>

int load_map(char *name);
int player_x, player_y;

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
        timeout(-1);
        keypad(mainwin, TRUE);

        getmaxyx(mainwin, height, width);
        refresh();

        if (load_map("level1.map") != 0) {
                printf("Couldn't load map!\n");
        }

        int c;
        int x = player_x;
        int y = player_y;
        move(y,x);
        int go = 1;
        while (go) {
                getmaxyx(mainwin, height, width);

                if (y>height-1)
                        y = height-1;
                if (x>width-1)
                        x = width-1;

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
                usleep(16667);
        }

        // Clean up after ourselves
        delwin(mainwin);
        endwin();
        refresh();

        return EXIT_SUCCESS;
}

int load_map(char *name)
{
        FILE *fp = fopen(name, "r");
        if (fp == NULL)
                return -1;
        char c = (char)fgetc(fp);
        int x, y;

        x = 0;
        y = 0;

        while (c != EOF) {
                if (c == '\n') {
                        x = 0;
                        y++;
                        c = (char)fgetc(fp);
                        continue;
                }
                if (c == 'P') {
                        player_y = y;
                        player_x = x;
                        c = ' ';
                }
                mvaddch(y, x, c);
                c = (char)fgetc(fp);
                x++;
        }
        return 0;
}
