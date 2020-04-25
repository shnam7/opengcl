#include "glist.h"
#include <stdio.h>

struct Item : public gcl::slist<>::node {
    char msg[16];
};


int main() {
    gcl::slist<Item> sl;

    Item *pRemove = 0;

    for (int i=0; i<10; i++) {
        Item *item = new Item();
        sprintf(item->msg, "Couunt=%d", i);
        sl.append(item);

        if (i==5) pRemove = item;

        printf("gslist add #%d\n", i);
    }

    printf("Removing item #5\n");
    sl.remove(pRemove);

    int count = 0;
    Item *item = sl.first();
    while (item) {
        printf("entry #%i: %s\n", count++, item->msg);
        item = sl.nextOf(item);
    }
}
