#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"

struct bum {
        int a;
        int b;
};

int main(int argc, const char **argv)
{
        srand(time(0));

        printf("Creating new list...\n");
        struct list *a = new_list();

        list_extend(a, 3, sizeof(struct bum));
        for (int i=0; i<a->length; i++) {
                (((struct bum *)list_access(a,i))->a = 100*i);
        }

        struct bum x = {
                .a = 100,
                .b = 200
        };
        struct bum *y;

        list_append(a, &x);
        y = list_access(a, 0);
        printf("%d\n", y->a);
        list_print(a);

        int data;
        for (int i=0; i<a->length; i++) {
                data = ((struct bum *)list_access(a,i))->a;
                printf("%d\n", data);
        }

        return 0;
}
