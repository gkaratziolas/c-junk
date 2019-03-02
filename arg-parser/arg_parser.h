#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _argument_type{
        char* name;
        char short_name;
        char* description;
        char* value;
        int present;
} Argument;

typedef enum arguemnt_type_type {
        STORE_TRUE,
        STORE_STRING,
        STORE_NUM
} ArgumentTypeType;

static int len_args = 0;
Argument* arg_list;

void add_arg(char*, char, char*);
int get_arg_index_by_name(char*);
int get_arg_index_by_short_name(char);
int is_arg_set(char*);
void parse_args(int, char**);
void print_arg_help(void);

void add_arg(char* name, char short_name, char* description)
{
        if(len_args == 0) {
                arg_list = malloc(sizeof(Argument));
                len_args++;
        } else {
                len_args++;
                arg_list = realloc(arg_list, sizeof(Argument)*len_args);
        }

        // Check that there isn;t already and arg with that name

        arg_list[len_args-1].name = name;
        arg_list[len_args-1].short_name = short_name;
        arg_list[len_args-1].description = description;
        arg_list[len_args-1].present = 0;
}

int get_arg_index_by_name(char* name)
{
        for (int i=0; i<len_args; i++) {
                if (strcmp(name, arg_list[i].name) == 0) {
                        return i;
                }
        }
        // Fail condition
        return -1;
}

int get_arg_index_by_short_name(char short_name)
{
        for (int i=0; i<len_args; i++) {
                if (short_name == arg_list[i].short_name) {
                        return i;
                }
        }
        // Fail condition
        return -1;
}

int is_arg_set(char* name)
{
        int index = get_arg_index_by_name(name);
        if (index == -1) {
                return index;
        }
        return arg_list[index].present;
}

void print_arg_help(void)
{
        printf("Args Available:\n");
        for (int i=0; i<len_args; i++) {
                if (arg_list[i].short_name != '\0') {
                        printf("-%c", arg_list[i].short_name);
                }
                printf("\t--%s", arg_list[i].name);
                if (arg_list[i].description[0] != '\0') {
                        printf("\t%s", arg_list[i].description);
                }
                printf("\n");
        }
}

void parse_args(int argc, char* argv[])
{
        int index;
        for (int i=0; i<argc; i++) {
                // Is string a long arg (i.e. --help)
                if (argv[i][0] == '-' && argv[i][1] == '-') {
                        // Treat string as arg name
                        // +2 to pointer ignore "--"
                        index = get_arg_index_by_name(argv[i]+2);
                        if (index > -1) {
                                arg_list[index].present = 1;
                        }
                        continue;

                // Is string a short arg (i.e. -h)
                } else if (argv[i][0] == '-') {
                        if (isalpha(argv[i][1])) {
                                int single_letter_index = 1;
                                while (argv[i][single_letter_index] != '\0') {
                                        // Treat as short code
                                        // Index 1 to ignore "-" and dereference pointer
                                        index = get_arg_index_by_short_name(argv[i][1]);
                                        if (index > -1) {
                                                arg_list[index].present = 1;
                                        }
                                        single_letter_index++;
                                }
                                continue;
                        }

                }
                // Treat as raw data -- process here
                printf("RAW DATA: %s\n", argv[i]);
        }
}
