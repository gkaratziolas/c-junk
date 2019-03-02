#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>

#define MATRIX_WIDTH    1000
#define MATRIX_HEIGHT   1000
#define DROPLETS        100
#define MAX_BRIGHTNESS  20

typedef struct _matrix_char_t {
        char value;
        int  brightness;
} matrix_char_t;

typedef struct _matrix_droplet_t {
        int x;
        int y;
        int brightness;
} matrix_droplet_t;

matrix_char_t **matrix;

void randomise_matrix();
void update_matrix(int, int, matrix_droplet_t *, int);

int main(int argc, char **argv)
{
        // Positions for the game window
        int height, width;
        WINDOW *mainwin;
        
        // Initialise ncurses
        if ((mainwin = initscr()) == NULL) {
                fprintf(stderr, "Error initialising ncurses.\n");
                exit(EXIT_FAILURE);
        }

        start_color();
        //init_color(COLOR_RED, 255, 105, 180);
        init_color(COLOR_BLACK, 0, 0, 0);
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);
        attron(A_BOLD);
        curs_set(0);

        matrix = malloc(MATRIX_HEIGHT * sizeof(matrix_char_t*));
        for (int i=0; i<MATRIX_HEIGHT; i++) {
                matrix[i] = malloc(MATRIX_WIDTH * sizeof(matrix_char_t));
        }

        noecho();
        keypad(mainwin, TRUE);
        getmaxyx(mainwin, height, width);

        for (int i=0; i<height; i++) {
                for (int j=0; j<width; j++) {
                        move(i, j);
                        mvaddch(i, j, matrix[i][j].value);
                }
        }

        refresh();
        randomise_matrix();

        matrix_droplet_t droplets[DROPLETS];

        for(int i=0; i<DROPLETS; i++) {
                droplets[i].x = rand()%width;
                droplets[i].y = rand()%height;
                droplets[i].brightness = MAX_BRIGHTNESS;
        }

        srand(time(0));
        int num_droplets = 0;
        while (1) {
                usleep(50000);
                getmaxyx(mainwin, height, width);
                clear();
                update_matrix(height, width, droplets, num_droplets);

                if (num_droplets < DROPLETS) {
                        if (rand()%2 == 0) {
                                num_droplets++;
                        }
                }

                refresh();
        }

        // Clean up after ourselves
        delwin(mainwin);
        endwin();
        refresh();

        return EXIT_SUCCESS;
}

void randomise_matrix()
{
        for (int i=0; i<MATRIX_HEIGHT; i++) {
                for (int j=0; j<MATRIX_WIDTH; j++) {
                        matrix[i][j].value = 32 + (rand()%95);
                        matrix[i][j].brightness = 0;
                }
        }
}

void update_matrix(int height, int width, matrix_droplet_t *droplets, int num_droplets) {
        for (int i=0; i<num_droplets; i++) {
                matrix[droplets[i].y][droplets[i].x].brightness = droplets[i].brightness;
                droplets[i].y++;
                if (droplets[i].y > height) {
                        droplets[i].y = 0;
                        droplets[i].x = rand() % width;
                }
        }

        for (int i=0; i<height; i++) {
                for (int j=0; j<width; j++) {
                        if (rand()%10 == 0) {
                                matrix[i][j].value = 32 + (rand()%95);
                        }
                        if (matrix[i][j].brightness > 0) {
                                move(i, j);
                                if (matrix[i][j].brightness == MAX_BRIGHTNESS) {
                                        attron(COLOR_PAIR(2));
                                        mvaddch(i, j, matrix[i][j].value);
                                        attron(COLOR_PAIR(1));
                                } else {
                                        mvaddch(i, j, matrix[i][j].value);
                                }
                                matrix[i][j].brightness--;
                        }
                }
        }
}
