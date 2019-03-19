/** @file list.h
 *
 * @brief An implementation of a doubly-linked list for storing integers.
 *
 */

#ifndef LIST_H
#define LIST_H

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

/* Publc Function Prototypes */

// Creating and destroying lists
struct list new_list(void);
void        del_list(struct list *list);

// Adding to and removing from lists
void list_append(struct list *list, int value);
void list_delete_item(struct list *list, int index);

// Accessing elements
struct list_item *list_goto_index(struct list *list, int index);
int list_set_value(struct list *list, int index, int value);

// Print list
void list_print(struct list *list);

#endif /* LIST_H */
