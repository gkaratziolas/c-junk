#include <stdio.h>

#include "fifo.h"
#include "gcode_reader.h"

void gcode_instruction_print(struct gcode_instruction *x);

int main(int argc, char **argv)
{
        FILE *fp;
        char c;
        int c_pos;
        int gcode_string_length;
        char gcode_string[GCODE_MAX_STRING_LEN];

        struct gcode_instruction x;

        struct gcode_instruction gcode_instructions[GCODE_MAX_INSTRUCTIONS];
        struct fifo gcode_instruction_fifo = fifo_init(&gcode_instructions,
                                                   sizeof(struct gcode_instruction),
                                                   GCODE_MAX_INSTRUCTIONS);

        if (argc == 1) {
                goto err;
        }

        printf("Reading: %s\n", argv[1]);
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
                goto err;
        }

        int i;
        c_pos = 0;
        gcode_string_length = 0;
        while ((c = fgetc(fp)) != EOF) {
                gcode_string[c_pos] = c;
                if (c == '\n') {
                        gcode_string_length = c_pos;

                        printf("read: '");
                        for (i = 0; i < gcode_string_length; i++) {
                            printf("%c", gcode_string[i]);
                        }
                        printf("'\n");

                        gcode_read_line(&gcode_instruction_fifo, gcode_string, gcode_string_length);
                        c_pos = 0;
                        while(fifo_pop(&gcode_instruction_fifo, &x) != FIFO_ERR_EMPTY) {
                                gcode_instruction_print(&x);
                        }
                } else {
                        c_pos++;
                }
        }

        fclose (fp);
        return 0;
err:
        return -1;
}

void gcode_instruction_print(struct gcode_instruction *x)
{
        printf(" %c%d\n", (x->code).variable, (int)((x->code).value + 0.1f));
        if ((x->variables).F_set)
                printf("  F = %f\n", (x->variables).F);
        if ((x->variables).I_set)
                printf("  I = %f\n", (x->variables).I);
        if ((x->variables).J_set)
                printf("  J = %f\n", (x->variables).J);
        if ((x->variables).X_set)
                printf("  X = %f\n", (x->variables).X);
        if ((x->variables).Y_set)
                printf("  Y = %f\n", (x->variables).Y);
        if ((x->variables).Z_set)
                printf("  Z = %f\n", (x->variables).Z);
        printf("\n");
}
