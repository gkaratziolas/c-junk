#ifndef GCODE_READER_H_
#define GCODE_READER_H_

#include "fifo.h"

#define GCODE_MAX_STRING_LEN 256
#define GCODE_MAX_CODES_PER_LINE 100
#define GCODE_MAX_INSTRUCTIONS 100

struct gcode_code {
        char variable;
        float value;
};

// Contains all supported variables
// Potential alternative would be to store fixed-length array of generic variables
struct gcode_variables {
        float F; // feed rate
        float I; // defines X-axis arc centre for cut curve
        float J; // defines X-axis arc centre for cut curve 
        float X; // X-position
        float Y; // Y-position
        float Z; // Z-position

        unsigned int F_set : 1;
        unsigned int I_set : 1;
        unsigned int J_set : 1;
        unsigned int X_set : 1;
        unsigned int Y_set : 1;
        unsigned int Z_set : 1;
        unsigned int reserved : 2;
};

struct gcode_instruction {
        struct gcode_code code;
        struct gcode_variables variables;
};

int gcode_read_line(struct fifo *gcode_instruction_fifo,
                    char *gcode_string, int str_length);
int gcode_read_chunk(struct gcode_code *code, char *gcode_string, int i0, int i1);
int gcode_process_codes(struct fifo *gcode_instruction_fifo,
                        struct fifo *gcode_code_fifo);

void gcode_instruction_print(struct gcode_instruction *x);

#endif // GCODE_READER_H_
