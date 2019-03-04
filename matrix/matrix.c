#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>

#define MATRIX_WIDTH    1000
#define MATRIX_HEIGHT   1000
#define DROPLETS        100

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
matrix_droplet_t droplets[DROPLETS];

// Maximum numnber of supported distinct terminal colours
const short kMaxColours = 8;
const int   kMaxBrightness = 30;

void randomise_matrix();
void move_droplets(int, int, int);
void update_matrix(int, int);
int color_override();

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

        start_color();
        color_override();
        attron(A_BOLD);
        curs_set(0);

        // Allocate memory for the matrix
        matrix = malloc(MATRIX_HEIGHT * sizeof(matrix_char_t*));
        for (int i=0; i<MATRIX_HEIGHT; i++) {
                matrix[i] = malloc(MATRIX_WIDTH * sizeof(matrix_char_t));
        }

        getmaxyx(mainwin, height, width);
        refresh();
        randomise_matrix();

        for(int i=0; i<DROPLETS; i++) {
                droplets[i].x = rand()%width;
                droplets[i].y = rand()%height;
                droplets[i].brightness = kMaxBrightness;
        }

        int num_droplets = 0;
        while (1) {
                usleep(50000);
                getmaxyx(mainwin, height, width);

                move_droplets(height, width, num_droplets);
                update_matrix(height, width);

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

/*
 * Sets up 8 colors and 8 color pairs.
 * The colors are white, black and 6 shades of green.
 * Each colour pair consists of one colour against a black background.
 */
int color_override()
{
        // Initialise set of colors
        for (int i=0; i<kMaxColours; i++) {
                init_color(i, 0, i*100, 0);
        };
        // Select special values for lightest color
        init_color(kMaxColours-1, 1000, 1000, 1000);
        
        // Initialise all the colour pairs used for printing text
        for(int i=0; i<kMaxColours; i++) {
                init_pair(i+1, 0, i);
        }
}

void move_droplets(int height, int width, int num_droplets)
{
        int x, y;
        for (int i=0; i<num_droplets; i++) {
                x = droplets[i].x;
                y = droplets[i].y;
                matrix[y][x].brightness = droplets[i].brightness;
                droplets[i].y++;
                if (droplets[i].y > height) {
                        y = (rand() % height) - kMaxBrightness;
                        y = (y<0) ? 0 : y;
                        droplets[i].y = y;
                        droplets[i].x = rand() % width;
                }
        }
}

void update_matrix(int height, int width)
{
        for (int i=0; i<height; i++) {
                for (int j=0; j<width; j++) {
                        if (rand()%10 == 0) {
                                matrix[i][j].value = 32 + (rand()%95);
                        }
                        if (matrix[i][j].brightness == 1) {
                                attron(COLOR_PAIR(1));
                                mvaddch(i, j, ' ');
                                matrix[i][j].brightness = 0;
                        }
                        if (matrix[i][j].brightness > 0) {
                                int color_value = (matrix[i][j].brightness*8)/kMaxBrightness;
                                // Avoid using color pair 0
                                if (color_value == 0) {
                                        color_value = 1;
                                }

                                attron(COLOR_PAIR(color_value));
                                mvaddch(i, j, matrix[i][j].value);
                                matrix[i][j].brightness--;
                        }
                }
        }
}
