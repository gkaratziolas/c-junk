#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"

int main(int argc, const char **argv)
{
        srand(time(0));
        struct list a = new_list();

        for (int i=0; i<10; i++) {
                list_append(&a, i*i);
        }

        list_delete_item(&a, 3);
        list_set_value(&a, 3, 100);
        list_print(&a);

        return 0;
}
