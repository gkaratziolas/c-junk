#include <stdio.h>

// Preprocessor substitutions
#define LINE_WIDTH 80

// Constants
const char ON_CHAR   = '#';
const char OFF_CHAR  = ' ';

// Function prototypes
void display_line(int*, int);
int apply_rule(int, int, int, int);

int main(int argc, char* argv[])
{
        int line[LINE_WIDTH]     = {0};
        int old_line[LINE_WIDTH] = {0};
        int a, b, c;

        int wrap_around = 1;
        int rule = 30;

        line[LINE_WIDTH/2] = 1;
        display_line(line, LINE_WIDTH);

        int x = 100;
        while (x--) {
                for (int i=0; i<LINE_WIDTH; i++) {
                        old_line[i] = line[i];
                }

                for (int i=0; i<LINE_WIDTH; i++) {
                        if (i==0) {
                                if (wrap_around) {
                                        a = old_line[LINE_WIDTH-1];
                                } else {
                                        a = 0;
                                }
                        } else {
                                a = old_line[i-1];
                        } 
                        b = old_line[i];
                        if (i==LINE_WIDTH-1) {
                                if (wrap_around) {
                                        c = old_line[LINE_WIDTH-1];
                                } else {
                                        c = 0;
                                }
                        } else {
                                c = old_line[i+1];
                        }
                        line[i] = apply_rule(a, b, c, rule);
                }
                display_line(line, LINE_WIDTH);
        }

        return 0;
}

void display_line(int* data, int data_length)
{
        for (int i=0; i<data_length; i++) {
                if (data[i] == 0) {
                        printf("%c", OFF_CHAR);
                } else {
                        printf("%c", ON_CHAR);
                }
        }
        printf("\n");
}

int apply_rule(int a, int b, int c, int rule)
{
        int index = a*4 + b*2 + c;
        if (((rule >> index) & 1) == 1) {
                return 1;
        }
        return 0;
}
