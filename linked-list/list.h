/** @file list.h
 *
 * @brief An implementation of a doubly-linked list for storing integers.
 *
 */

#ifndef LIST_H
#define LIST_H

struct list_element {
        int value;
        struct list_element *next;
        struct list_element *prev;
};

struct list {
        int length;
        struct list_element *head;
        struct list_element *tail;
};

/* Publc Function Prototypes */

// Creating and destroying lists
struct list *new_list(void);
void         del_list(struct list *list);

// Adding to and removing from lists
void list_append(struct list *list, int value);
void list_delete_element(struct list *list, int index);

// Accessing elements
struct list_element *list_goto_index(struct list *list, int index);
int list_get_value(struct list *list, int index);
int list_set_value(struct list *list, int index, int value);

// Altering the list
void list_reverse(struct list *list);
void list_swap_elements(struct list *list, int i0, int i1);
void list_swap_values(struct list *list, int i0, int i1);
void list_shuffle(struct list *list);

// Print list
void list_print(struct list *list);

#endif /* LIST_H */
