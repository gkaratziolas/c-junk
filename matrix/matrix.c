#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <time.h>

typedef struct _matrix_char_t {
        char value;
        int  brightness;
} matrix_char_t;

typedef struct _matrix_droplet_t {
        int x;
        int y;
        int brightness;
} matrix_droplet_t;

typedef struct _special_char_t {
        int x;
        int y;
        char value;
} special_char_t;

matrix_char_t    **matrix;
matrix_droplet_t  *droplets;
special_char_t    *specials;

// Maximum numnber of supported distinct terminal colours
const short kMaxColours       = 8;
const int   kMaxBrightness    = 20;
const int   kCharUpdatePeriod = 10;
const int   kMatrixWidth      = 1000;
const int   kMatrixHeight     = 1000;
const int   kMaxDroplets      = 100;

void randomise_matrix(void);
void move_droplets(int, int, int);
void update_matrix(int, int);
int color_override(void);
char random_char(void);
void print_special_chars(int);
void update_special_chars(int, int, int);

int main(int argc, char **argv)
{
        // Positions for the game window
        int height, width;
        int height_old, width_old;
        WINDOW *mainwin;

        // Allocate memory for the matrix
        matrix = malloc(kMatrixHeight * sizeof(matrix_char_t*));
        for (int i=0; i<kMatrixHeight; i++) {
                matrix[i] = malloc(kMatrixWidth * sizeof(matrix_char_t));
        }

        // Allocate memory for the droplet array
        droplets = malloc(kMaxDroplets * sizeof(matrix_droplet_t));

        // Seed the random number generator        
        srand(time(0));

        // Initialise ncurses
        if ((mainwin = initscr()) == NULL) {
                fprintf(stderr, "Error initialising ncurses.\n");
                exit(EXIT_FAILURE);
        }
        noecho();
        keypad(mainwin, TRUE);

        // Update ncurses colours and set print to bold
        start_color();
        color_override();
        //attron(A_BOLD);
        curs_set(0);

        getmaxyx(mainwin, height, width);
        refresh();
        randomise_matrix();

        for(int i=0; i<kMaxDroplets; i++) {
                droplets[i].x = rand()%width;
                droplets[i].y = rand()%height;
                droplets[i].brightness = kMaxBrightness;
        }

        int len_specials = 0;
        if (argc > 1) {
                len_specials = strlen(argv[1]);
                specials = malloc(len_specials * sizeof(special_char_t));
                for (int i=0; i<len_specials; i++) {
                        specials[i].value = argv[1][i];
                }
                update_special_chars(len_specials, height, width);
        }

        int num_droplets = 0;
        while (1) {
                getmaxyx(mainwin, height, width);
                if (height != height_old || width != width_old) {
                        update_special_chars(len_specials, height, width);
                }
                height_old = height;
                width_old = width;

                move_droplets(height, width, num_droplets);
                update_matrix(height, width);
                print_special_chars(len_specials);

                if (num_droplets < kMaxDroplets) {
                        if (rand()%2 == 0) {
                                num_droplets++;
                        }
                }

                refresh();
                usleep(50000);
        }

        // Clean up after ourselves
        delwin(mainwin);
        endwin();
        refresh();

        return EXIT_SUCCESS;
}

void randomise_matrix()
{
        for (int i=0; i<kMatrixHeight; i++) {
                for (int j=0; j<kMatrixWidth; j++) {
                        matrix[i][j].value = random_char();
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
                init_color(i, 100, (i+2)*100, 100);
        }

        // Select special values for lightest color
        init_color(kMaxColours-1, 500, 1000, 500);
        // Select special values for darkest color
        init_color(0,              100,  100,  100);

        // Initialise all the colour pairs used for printing text
        for (int i=0; i<kMaxColours; i++) {
                init_pair(i+1, i, 0);
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
                        matrix[y][x].brightness = 0;
                        y = (rand() % height) - kMaxBrightness;
                        y = (y<0) ? 0 : y;
                        droplets[i].y = y;
                        droplets[i].x = rand() % width;
                }
        }
}

char random_char()
{
        // Return one of the printable ascii chars
        return 32 + (rand()%95);
}

void update_matrix(int height, int width)
{
        for (int i=0; i<height; i++) {
                for (int j=0; j<width; j++) {
                        if (matrix[i][j].brightness == 0) {
                                continue;
                        }
                        if (rand()%kCharUpdatePeriod == 0) {
                                matrix[i][j].value = random_char();
                        }
                        if (matrix[i][j].brightness > 0) {
                                int color_value = (matrix[i][j].brightness*kMaxColours)/kMaxBrightness;
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

void update_special_chars(int len_specials, int height, int width)
{
        int y = height/2;
        int x0 = (width - len_specials)/2;
        for (int i=0; i<len_specials; i++) {
                specials[i].y = y;
                specials[i].x = x0+i;
        }
}

void print_special_chars(int len_specials)
{
        if (len_specials == 0) {
                return;
        }
        for (int i=0; i<len_specials; i++) {
                int y = specials[i].y;
                int x = specials[i].x;
                if (matrix[y][x].brightness > 0) {
                        matrix[y][x].brightness = kMaxBrightness;
                        matrix[y][x].value      = specials[i].value;
                        attron(COLOR_PAIR(kMaxColours));
                        mvaddch(y, x, specials[i].value);
                }
        }
}
