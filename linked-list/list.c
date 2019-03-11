#include <stdio.h>
#include <stdlib.h>

struct list_item {
        int value;
        struct list_item *next;
        struct list_item *prev;
};

struct list_item *new_list(int len);
void list_print(struct list_item *list);
struct list_item *list_goto_index(struct list_item *list, int index);
struct list_item *list_goto_end(struct list_item *list);
int list_set_value(struct list_item *list, int index, int value);
struct list_item *list_reverse(struct list_item *list);
void list_append(struct list_item *list, int value);
void list_delete_item(struct list_item *list, int index);
void list_delete_all(struct list_item *list);

int main(int argc, const char **argv)
{
        struct list_item *a = new_list(10);
        list_set_value(a, 7, 12);
        list_set_value(a, 1, 67890);
        list_set_value(a, 0, 12345);
        list_set_value(a, 3, 21);
        list_set_value(a, 9, 1);

        a = list_reverse(a);
        list_append(a, 101);

        for (int i=0; i<10; i++) {
                list_append(a, i*i);
        }
        a = list_reverse(a);
        list_delete_item(a, 3);
        list_print(a);
        list_delete_all(a);

        return 0;
}

struct list_item *new_list(int len)
{
        struct list_item *a = malloc(sizeof(struct list_item));
        a->prev = 0;
        a->next = 0;
        a->value = 0;

        for (int i=0; i<len-1; i++) {
                list_append(a, 0);
        }

        return a;
}

struct list_item *list_goto_index(struct list_item *list, int index)
{
        int current = 0;

        while (current<index) {
                if (list->next == 0) {
                        return 0;
                }
                list = list->next;
                current++;
        }
        return list;
}

struct list_item *list_goto_end(struct list_item *list)
{
        while(list->next != 0) {
                list = list->next;
        }
        return list;
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

void list_print(struct list_item *list)
{
        printf("%d\n", list->value);
        while(list->next != 0) {
                list = list->next;
                printf("%d\n", list->value);
        }
}

void list_append(struct list_item *list, int value)
{
        struct list_item *a = malloc(sizeof(struct list_item));
        list = list_goto_end(list);
        list->next = a;
        a->next = 0;
        a->prev = list;
        a->value = value;
}

void list_delete_item(struct list_item *list, int index)
{
        list = list_goto_index(list, index);
        if (list->next != 0)
                (list->next)->prev = list->prev;
        if (list->prev != 0)
                (list->prev)->next = list->next;
        free(list);
}

void list_delete_all(struct list_item *list)
{
        struct list_item *tmp = list->next;

        while(list->next != 0) {
                free(list);
                list = tmp;
                tmp = list->next;
        }
        free(list);
}
