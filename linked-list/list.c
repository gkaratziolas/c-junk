#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct list *new_list(void)
{
        struct list *a = malloc(sizeof(struct list));
        a->head = NULL;
        a->tail = NULL;
        a->length = 0;

        return a;
}

void del_list(struct list *list)
{
        struct list_element *a, *b;
        a = list->head;

        // If list has no elements, free list and return
        if (a == NULL) {
                free(list);
                return;
        }

        while(a->next != NULL) {
                b = a->next;
                free(a);
                a = b;
        }
        free(a);
        free(list);
}

void list_append(struct list *list, int value)
{
        struct list_element *new_element = malloc(sizeof(struct list_element));

        // If this is the first element, append works differently
        if (list->length == 0) {
                // New element is both head and tail of the list
                list->head = new_element;
                // element is first in list, so prev points to NULL
                new_element->prev = NULL;
        } else {
                struct list_element *old_tail = list->tail;
                old_tail->next = new_element;
                new_element->prev = old_tail;
        }

        new_element->value = value;
        new_element->next  = NULL;
        list->tail = new_element;
        (list->length)++;
}

void list_delete_element(struct list *list, int index)
{
        struct list_element* element = list_goto_index(list, index);
        if (element->next != NULL)
                (element->next)->prev = element->prev;
        if (element->prev != NULL)
                (element->prev)->next = element->next;
        free(element);
        (list->length)--;
}

struct list_element *list_goto_index(struct list *list, int index)
{
        if (list->head == NULL)
                return NULL;

        struct list_element *element = list->head;
        int current = 0;

        while (current<index) {
                if (element->next == NULL) {
                        return NULL;
                }
                element = element->next;
                current++;
        }
        return element;
}

int list_get_value(struct list *list, int index)
{
        struct list_element *element = list_goto_index(list, index);
        // TODO: Return something more sensible here! 
        if (element == NULL)
                return -1;
        return element->value;
}

int list_set_value(struct list *list, int index, int value)
{
        struct list_element *element = list_goto_index(list, index);
        if (element == NULL)
                return -1;
        element->value = value;
        return 0;
}

void list_swap_elements(struct list *list, int i0, int i1)
{
        struct list_element *a, *b, *tmp;
        a = list_goto_index(list, i0);
        b = list_goto_index(list, i1);

        if (a->prev != NULL)
                (a->prev)->next = b;
        if (a->next != NULL)
                (a->next)->prev = b;
        if (b->prev != NULL)
                (b->prev)->next = a;
        if (b->next != NULL)
                (b->next)->prev = a;

        tmp = a->next;
        a->next = b->next;
        b->next = tmp;

        tmp = a->prev;
        a->prev = b->prev;
        b->prev = tmp;

        if (a->prev == NULL)
                list->head = a;
        if (a->next == NULL)
                list->tail = a;
        if (b->prev == NULL)
                list->head = b;
        if (b->next == NULL)
                list->tail = b;
}

void list_swap_values(struct list *list, int i0, int i1)
{
        if (i0 == i1)
                return;

        struct list_element *a, *b;
        int tmp;

        a = list_goto_index(list, i0);
        b = list_goto_index(list, i1);
        
        tmp = a->value;
        a->value = b->value;
        b->value = tmp;
}

void list_reverse(struct list *list)
{
        struct list_element *element, *tmp;
        element = list->head;

        while(element != NULL) {
                tmp = element->next;
                element->next = element->prev;
                element->prev = tmp;

                // Go to the "next" (now prev) element
                element = element->prev;
        }
        tmp = list->head;
        list->head = list->tail;
        list->tail = tmp;
}

void list_shuffle(struct list *list)
{
        int j, len;
        len = list->length;
        for (int i=0; i<len-1; i++) {
                j = rand()%(len-i) + i;
                list_swap_values(list, i, j);
        }
}

void list_print(struct list *list)
{
        struct list_element *element = list->head;
        while (element != NULL) {
                printf("%d\n", element->value);
                element = element->next;
        }
}
