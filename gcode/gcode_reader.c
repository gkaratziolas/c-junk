#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gcode_reader.h"

char is_letter(char x);
char is_numerical(char x);
char upper(char x);

enum gcode_read_state {
        IDLE,
        IN_COMMENT,
        IN_COMMAND,
        END
};

// TODO: what happens if fifo overflows? 
int gcode_read_line(struct fifo *gcode_instruction_fifo,
                    char *gcode_string, int str_length)
{
        char c;
        int i, i0, i1;
        int in_comment = 0;
        int in_command = 0;

        int syntax_error = 0;

        enum gcode_read_state state = IDLE;

        struct gcode_code tmp_code;

        struct gcode_code codes[GCODE_MAX_CODES_PER_LINE];
        struct fifo gcode_code_fifo = fifo_init(&codes,
                                                sizeof(struct gcode_code),
                                                GCODE_MAX_CODES_PER_LINE);

        printf("read: '");
        for (i = 0; i < str_length; i++) {
                printf("%c", gcode_string[i]);
        }
        printf("'\n");

        for (i = 0; i < str_length; i++) {
                c = gcode_string[i];

                switch (state) {
                case IDLE:
                        if (is_letter(c)) {
                                state = IN_COMMAND;
                                i0 = i;
                        } else if (c == '(') {
                                state = IN_COMMENT;
                        } else if (c == ';') {
                                state = END;
                        }
                        break;
                case IN_COMMENT:
                        if (c == ')') {
                                state = IDLE;
                        }
                        break;
                case IN_COMMAND:
                        if (i == (str_length - 1)) {
                                // new command found
                                i1 = i;
                                syntax_error = gcode_read_chunk(&tmp_code, gcode_string, i0, i1);
                                fifo_push(&gcode_code_fifo, &tmp_code);
                                state = IDLE;
                        } else if (!is_numerical(gcode_string[i+1])) {
                                // new command found
                                i1 = i;
                                syntax_error = gcode_read_chunk(&tmp_code, gcode_string, i0, i1);
                                fifo_push(&gcode_code_fifo, &tmp_code);
                                state = IDLE;
                        }
                        break;
                case END:
                default:
                        break;
                }
                if (syntax_error != 0) {
                        return syntax_error;
                }
        }
        gcode_process_codes(gcode_instruction_fifo, &gcode_code_fifo);
}

/*
 * Takes a chunk of gcode (e.g "G 01.2 412 ") fills in a code struct
 * Returns 0 for sucess, otherwise returns 1 + location of syntax error
 */
int gcode_read_chunk(struct gcode_code *code, char *gcode_string, int i0, int i1)
{
        int i = i0;
        float sign = 1;

        code->variable = gcode_string[i0];
        code->value    = 0;
        i++;

        // Ignore any leading whitespace
        while (gcode_string[i] == ' ') {
                i++;
        }

        // Look for exactly one leading plus or minus symbol
        if (gcode_string[i] == '+') {
                i++;
        } else if (gcode_string[i] == '-') {
                sign = -1;
                i++;
        }

        int val = 0;
        int div = 0;
        while (i < i1+1) {
                // allow for one or fewer decimal points
                if (gcode_string[i] == '.') {
                        if (div == 0) {
                                div = 1;
                        } else {
                                return i+1;
                        }
                // allow many [0-9]
                } else if (gcode_string[i] >= '0' && gcode_string[i] <= '9') {
                        val = val * 10;
                        val += (int)(gcode_string[i] - '0');
                        if (div > 0) {
                                div *= 10;
                        }
                // ignore any spaces
                } else if (gcode_string[i] == ' ') {
                        ;;
                // anything else is illegal
                } else {
                        return i+1;
                }
                i++;
        }
        if (div == 0)
                div = 1;
        code->value = (float) (sign * val) / (float) div;
        return 0;
}

int gcode_process_codes(struct fifo *gcode_instruction_fifo,
                        struct fifo *gcode_code_fifo)
{
        struct gcode_instruction tmp_instruction;
        struct gcode_code tmp_code;

        (tmp_instruction.variables).F_set = 0;
        (tmp_instruction.variables).I_set = 0;
        (tmp_instruction.variables).J_set = 0;
        (tmp_instruction.variables).X_set = 0;
        (tmp_instruction.variables).Y_set = 0;
        (tmp_instruction.variables).Z_set = 0;

        // Decode first code
        if (fifo_pop(gcode_code_fifo, &tmp_code) == FIFO_ERR_EMPTY) {
                return 0;
        }
        // ignore line numbers
        // look for next code
        if (tmp_code.variable == 'N') {
                if (fifo_pop(gcode_code_fifo, &tmp_code) == FIFO_ERR_EMPTY) {
                        return 0;
                }
        }
        // if M or G, start new command
        if ((tmp_code.variable == 'G') || (tmp_code.variable == 'M')) {
                memcpy(&(tmp_instruction.code), &tmp_code, sizeof(struct gcode_code));
        } else {
                // if the line starts with a variable, dump everything
                return -1;
        }

        // At this point, tmp_instruction contains a valid code
        while (!fifo_empty(gcode_code_fifo)) {
                fifo_pop(gcode_code_fifo, &tmp_code);
                switch(tmp_code.variable) {
                case 'G':
                case 'M':
                        fifo_push(gcode_instruction_fifo, &tmp_instruction);
                        memcpy(&(tmp_instruction.code), &tmp_code, sizeof(struct gcode_code));
                        (tmp_instruction.variables).F_set = 0;
                        (tmp_instruction.variables).I_set = 0;
                        (tmp_instruction.variables).J_set = 0;
                        (tmp_instruction.variables).X_set = 0;
                        (tmp_instruction.variables).Y_set = 0;
                        (tmp_instruction.variables).Z_set = 0;
                        break;
                case 'N':
                        // syntax error, can't have line number in middle of line!
                        return -1;
                case 'F':
                        if (((tmp_instruction.variables).F_set == 1) ||
                            ((tmp_instruction.code).variable != 'G'))
                                return -1;
                        (tmp_instruction.variables).F = tmp_code.value;
                        (tmp_instruction.variables).F_set = 1;
                        break;
                case 'I':
                        if (((tmp_instruction.variables).I_set == 1) ||
                            ((tmp_instruction.code).variable != 'G'))
                                return -1;
                        (tmp_instruction.variables).I = tmp_code.value;
                        (tmp_instruction.variables).I_set = 1;
                        break;
                case 'J':
                        if (((tmp_instruction.variables).J_set == 1) ||
                            ((tmp_instruction.code).variable != 'G'))
                                return -1;
                        (tmp_instruction.variables).J = tmp_code.value;
                        (tmp_instruction.variables).J_set = 1;
                        break;
                case 'X':
                        if (((tmp_instruction.variables).X_set == 1) ||
                            ((tmp_instruction.code).variable != 'G'))
                                return -1;
                        (tmp_instruction.variables).X = tmp_code.value;
                        (tmp_instruction.variables).X_set = 1;
                        break;
                case 'Y':
                        if (((tmp_instruction.variables).Y_set == 1) ||
                            ((tmp_instruction.code).variable != 'G'))
                                return -1;
                        (tmp_instruction.variables).Y = tmp_code.value;
                        (tmp_instruction.variables).Y_set = 1;
                        break;
                case 'Z':
                        if (((tmp_instruction.variables).Z_set == 1) ||
                            ((tmp_instruction.code).variable != 'G'))
                                return -1;
                        (tmp_instruction.variables).Z = tmp_code.value;
                        (tmp_instruction.variables).Z_set = 1;
                        break;
                default:
                        // unsupported variable
                        return -1;
                }
        }
        fifo_push(gcode_instruction_fifo, &tmp_instruction);
        memcpy(&(tmp_instruction.code), &tmp_code, sizeof(struct gcode_code));

        return 0;
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

// Returns 1 for ascii letter [a-zA-Z], 0 otherwise
char is_letter(char x)
{
        if ((x >= 'A') && (x <= 'Z'))
                return 1;
        if ((x >= 'a') && (x <= 'z'))
                return 1;
        return 0;
}

// Returns 1 for ascii symbols used for writing numbers [0-1\+\-\.], 0 otherwise
char is_numerical(char x)
{
        if ((x >= '0') && (x <= '9'))
                return 1;
        if ((x == '-') || (x == '+') || (x == '.') || (x == ' '))
                return 1;
        return 0;
}

char upper(char x)
{
        if ((x >= 'A') && (x <= 'Z'))
                return x;
        if ((x >= 'a') && (x <= 'z'))
                return (x - 'a') + 'A';
        return x;    
}
