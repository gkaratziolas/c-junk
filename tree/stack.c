#include "stack.h"

struct stack *stack_create(size_t data_size, int initial_capacity)
{
        struct stack *s = malloc(sizeof(struct stack));
        if (s == NULL)
                return NULL;

        s->top = -1;
        s->data_size = data_size;
        s->capacity = initial_capacity;

        s->array = malloc(data_size * initial_capacity);
        if (s->array == NULL) {
                free(s);
                return NULL;
        }

        return s;
}

int stack_destroy(struct stack *s)
{
        if (s == NULL)
                return -1;

        if (s->array != NULL)
                free(s->array);
        
        free(s);
        return 0;
}

int stack_push(struct stack *s, void *data)
{
        if (s == NULL || s->array == NULL || data == NULL)
                return -1;

        // If capacity is exceeded, double stack capacity
        if ((s->top + 1) >= s->capacity) {
                void *tmp = realloc(s->array, s->capacity * s->data_size * 2);
                if (tmp == NULL) {
                        return -1;
                }
                s->array = tmp;
                s->capacity *= 2;
        }

        s->top++;
        memcpy((char *)s->array + (s->top * s->data_size), data, s->data_size);
        return 0;
}

int stack_pop(struct stack *s, void *data)
{
        if (s == NULL || s->array == NULL)
                return -1;
        
        if (s-> top == -1)
                return -1;

        // User can pass NULL if they don't need the popped value
        if (data != NULL)
                memcpy(data, (char *)s->array + (s->top * s->data_size), s->data_size);
        
        s->top--;

        return 0;
}

int stack_peek(struct stack *s, void *data)
{
        if (s == NULL || s->array == NULL || data == NULL)
                return -1;
        
        if (s-> top == -1)
                return -1;

        memcpy(data, (char *)s->array + (s->top * s->data_size), s->data_size);

        return 0;
}

void *stack_top(struct stack *s)
{
        if (s == NULL || s->array == NULL || s->top == -1)
                return NULL;

        return (char *)s->array + (s->top * s->data_size);
}
