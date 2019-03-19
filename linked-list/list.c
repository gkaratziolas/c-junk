#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct list_item {
        int value;
        struct list_item *next;
        struct list_item *prev;
};

struct list {
        int length;
        struct list_item *head;
        struct list_item *tail;
};

// Creating and destroying lists
struct list  new_list(void);
void  list_delete_all(struct list *list);

// Adding to and removing from lists
void      list_append(struct list *list, int value);
void list_delete_item(struct list *list, int index);

// Accessing elements
struct list_item *list_goto_index(struct list *list, int index);

// Print list
void list_print(struct list *list);

/*
int list_set_value(struct list_item *list, int index, int value);
struct list_item *list_reverse(struct list_item *list);
int list_length(struct list_item *list);
void list_swap_entries(struct list_item *list, int i0, int i1);
void list_swap_values(struct list_item *list, int i0, int i1);
void list_shuffle(struct list_item *list);
*/

int main(int argc, const char **argv)
{
        srand(time(0));
        struct list a = new_list();

        for (int i=0; i<10; i++) {
                list_append(&a, i*i);
        }

        list_delete_item(&a, 3);
        list_print(&a);

        return 0;
}

struct list new_list(void)
{
        struct list a = {
                .head   = NULL,
                .tail   = NULL,
                .length = 0
        };
        return a;
}

void list_append(struct list *list, int value)
{
        struct list_item *new_item = malloc(sizeof(struct list_item));

        // If this is the first item, append works differently
        if (list->length == 0) {
                // New item is both head and tail of the list
                list->head = new_item;
                // Item is first in list, so prev points to NULL
                new_item->prev = NULL;
        } else {
                struct list_item *old_tail = list->tail;
                old_tail->next = new_item;
                new_item->prev = old_tail;
        }

        new_item->value = value;
        new_item->next  = NULL;
        list->tail = new_item;
        list->length++;
}

void list_delete_item(struct list *list, int index)
{
        struct list_item* item = list_goto_index(list, index);
        if (item->next != NULL)
                (item->next)->prev = item->prev;
        if (item->prev != NULL)
                (item->prev)->next = item->next;
        free(item);
}

void list_delete_all(struct list *list)
{
        struct list_item *a, *b;
        a = list->head;

        // If list has no items, free list and return
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

struct list_item *list_goto_index(struct list *list, int index)
{
        if (list->head == NULL)
                return NULL;

        struct list_item *item = list->head;
        int current = 0;

        while (current<index) {
                if (item->next == NULL) {
                        return NULL;
                }
                item = item->next;
                current++;
        }
        return item;
}

void list_print(struct list *list)
{
        struct list_item *item = list->head;
        while (item != NULL) {
                printf("%d\n", item->value);
                item = item->next;
        }
}


/*
int list_length(struct list_item *list)
{
        int length = 1;
        list = list_goto_start(list);
        while(list->next != 0) {
                list = list->next;
                length++;
        }
        return length;
}

int list_set_value(struct list_item *list, int index, int value)
{
        list = list_goto_index(list, index);
        if (list == 0)
                return -1;
        list->value = value;
}

struct list_item *list_reverse(struct list_item *list)
{
        struct list_item *tmp = list->next;
        while(tmp != 0) {
                list->next = list->prev;
                list->prev = tmp;
                list = list->prev;
                tmp  = list->next;
        }
        list->next = list->prev;
        list->prev = tmp;
        return list;
}

void list_swap_entries(struct list_item *list, int i0, int i1)
{
        struct list_item *b, *tmp;
        b = list_goto_index(list, i0);
        list = list_goto_index(list, i1);

        tmp = b->next;
        b->next = list->next;
        list->next = tmp;

        tmp = b->prev;
        b->prev = list->prev;
        list->prev = tmp;
}

void list_swap_values(struct list_item *list, int i0, int i1)
{
        if (i0 == i1)
                return;

        struct list_item *b = list_goto_index(list, i0);
        list = list_goto_index(list, i1);
        int tmp;

        tmp = b->value;
        b->value = list->value;
        list->value = tmp;
}

void list_shuffle(struct list_item *list)
{
        int j, len;
        len = list_length(list);
        for (int i=0; i<len-1; i++) {
                j = rand()%(len-i) + i;
                list_swap_values(list, i, j);
        }
}
*/
