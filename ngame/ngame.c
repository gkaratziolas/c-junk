#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <time.h>

#include "list.h"

struct coord {
        int x;
        int y;
};

struct actor {
        struct coord position;
        int health;
        char symbol;
};

char **map;
struct list *bad_dudes;

const int  kMapWidth        = 1000;
const int  kMapHeight       = 1000;
const int  kMaxPlayerHealth = 10;
const char kPlayerSymbol    = '@';

struct actor load_map(char *name);
int player_input(struct actor *player, int c);

int player_x, player_y;

int main(int argc, char **argv)
{
        // Positions for the game window
        int height, width;
        WINDOW *mainwin;

        // Allocate memory for the map
        map = malloc(kMapHeight * sizeof(char*));
        for (int y=0; y<kMapHeight; y++) {
                map[y] = malloc(kMapWidth * sizeof(char));
        }

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
        curs_set(0);

        getmaxyx(mainwin, height, width);

        struct actor player = load_map("level1.map");
        refresh();

        int input;
        while (1) {
                getmaxyx(mainwin, height, width);

                input = wgetch(mainwin);
                if (input == 'q') {
                        break;
                }

                player_input(&player, input);

                if (map[player.position.y][player.position.x] == 'V') {
                        player = load_map("level2.map");
                }

                refresh();
                usleep(16667);
        }

        // Clean up after ourselves
        delwin(mainwin);
        endwin();
        refresh();

        return EXIT_SUCCESS;
}

int player_input(struct actor *player, int c)
{
        int y = player->position.y;
        int x = player->position.x;

        mvaddch(y, x, map[y][x]);

        switch (c) {
        case KEY_UP:
                if (y>0 && map[y-1][x] != '#')
                        y--;
                break;
        case KEY_DOWN:
                if (y<kMapHeight-1 && map[y+1][x] != '#')
                        y++;
                break;
        case KEY_LEFT:
                if (x>0 && map[y][x-1] != '#')
                        x--;
                break;
        case KEY_RIGHT:
                if (x<kMapWidth-1 && map[y][x+1] != '#')
                        x++;
                break;
        }

        mvaddch(y, x, player->symbol);
        player->position.y = y;
        player->position.x = x;
}

struct actor load_map(char *name)
{
        FILE *fp = fopen(name, "r");
        if (fp == NULL)
                printf("Can't load map file: %s\n", name);

        int x, y;
        char c = (char)fgetc(fp);
        struct actor player = {
                .position = {0,0},
                .health   = kMaxPlayerHealth,
                .symbol   = kPlayerSymbol
        };

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
                        player.position.y = y;
                        player.position.x = x;
                        c = ' ';
                }
                map[y][x] = c;
                mvaddch(y, x, c);
                c = (char)fgetc(fp);
                x++;
        }

        mvaddch(player.position.y, player.position.x, player.symbol);
        return player;
}
