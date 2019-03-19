#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"

int main(int argc, const char **argv)
{
        srand(time(0));

        printf("Creating new list...\n");
        struct list *a = new_list();

        printf("Adding first 10 squares to list...\n");
        for (int i=0; i<10; i++) {
                list_append(a, i*i);
        }
        list_print(a);

        printf("\nDeleting 3rd item\n");
        list_delete_item(a, 2);
        list_print(a);

        printf("\nAdding 10 random items...\n");
        for (int i=0; i<10; i++) {
                list_append(a, rand());
        }
        list_print(a);

        printf("Setting value manually...\n");
        list_set_value(a, 3, 100);
        list_print(a);

        printf("\nDeleting List\n");
        del_list(a);

        return 0;
}
