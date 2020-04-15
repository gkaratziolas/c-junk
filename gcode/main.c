#include <stdio.h>

#include "gcode_reader.h"
#include "fifo.h"

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

        c_pos = 0;
        gcode_string_length = 0;
        while ((c = fgetc(fp)) != EOF) {
                gcode_string[c_pos] = c;
                if (c == '\n') {
                        gcode_string_length = c_pos;
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
