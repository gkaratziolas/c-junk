#include <stdio.h>

#include "fifo.h"
#include "gcode_reader.h"

#define GCODE_MAX_COMMANDS 100

int gcode_command_print(struct gcode_command *gcommand);

int main(int argc, char **argv)
{
        FILE *fp;
        char c;
        int c_pos;
        int gcode_string_length;
        char gcode_string[GCODE_MAX_STRING_LEN];

        struct gcode_command x;

        struct gcode_command gcommands[GCODE_MAX_COMMANDS];
        struct fifo gcommand_fifo = fifo_init(gcommands,
                                              sizeof(struct gcode_command),
                                              GCODE_MAX_COMMANDS);

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

                        gcode_read_line(&gcommand_fifo, gcode_string, gcode_string_length);
                        c_pos = 0;
                        while(fifo_pop(&gcommand_fifo, &x) != FIFO_ERR_EMPTY) {
                                gcode_command_print(&x);
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

int gcode_command_print(struct gcode_command *gcommand)
{
        printf("  decode: ");
        switch (gcommand->code) {
        case gcode_G00:
                printf("G00 ");
                break;
        case gcode_G01:
                printf("G01 ");
                break;
        case gcode_G02:
                printf("G02 ");
                break;
        case gcode_G03:
                printf("G03 ");
                break;
        case gcode_G21:
                printf("G21 ");
                break;
        case gcode_M02:
                printf("M02 ");
                break;
        case gcode_M03:
                printf("M03 ");
                break;
        case gcode_M05:
                printf("M05 ");
                break;
        case gcode_MXX:
                printf("Unsupported M command!\n");
                return -1;
        case gcode_GXX:
                printf("Unsupported G command!\n");
                return -1;
        case gcode_NONE:
                printf("Unknown command!\n");
                return -1;
        default:
                printf("Error reading code!\n");
                return -1;
        }

        int i;
        for (i=0; i<GCODE_MAX_VARS; i++) {
                if (gcommand->vars[i].name == 0) {
                        break;
                }
                printf("%c:%f ", gcommand->vars[i].name, gcommand->vars[i].value);
        }
        printf("\n");
        return 0;
}
