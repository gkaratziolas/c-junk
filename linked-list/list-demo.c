#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"

struct dude {
        int x;
        int y;
        int health;
        char symbol;
};

int main(int argc, const char **argv)
{
        srand(time(0));

        struct dude *duuude;

        struct list *bad_dudes = new_list(sizeof(struct dude));
        list_extend(bad_dudes, 10);
        for (int i=0; i<10; i++) {
                duuude = ((struct dude *)list_access(bad_dudes, i));
                duuude->x = 0;
                duuude->y = 0;
                duuude->health = 10;
                duuude->symbol = '*';
        }

        ((struct dude *)list_access(bad_dudes, 3))->health--;

        int health;
        health = ((struct dude *)list_access(bad_dudes, 3))->health;
        printf("%d\n", health);

        return 0;
}
