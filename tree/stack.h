#ifndef STACK_H_
#define STACK_H_

#include <stdlib.h>
#include <string.h>

struct stack {
        int top;
        void *array;
        int data_size;
        int capacity;
};

struct stack *stack_create(size_t data_size, int initial_capacity);
int stack_destroy(struct stack *s);

int stack_push(struct stack *s, void *data);
int stack_pop(struct stack *s, void *data);
int stack_peek(struct stack *s, void *data);
void *stack_top(struct stack *s);

#endif // STACK_H_