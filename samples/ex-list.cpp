#include "glist.h"
#include <stdio.h>

using gcl::list;

struct Item : public list<>::node {
    char msg[16];
};


int main() {
    list<Item> gl;

    Item *pRemove = 0;

    for (int i=0; i<10; i++) {
        Item *item = new Item();
        sprintf(item->msg, "Couunt=%d", i);
        gl.append(item);

        if (i==5) pRemove = item;

        printf("glist add #%d\n", i);
    }

    printf("Removing item #5\n");
    gl.remove(pRemove);

    int count = 0;
    Item *item = gl.first();
    while (item) {
        printf("entry #%i: %s\n", count++, item->msg);
        item = gl.nextOf(item);
    }

}
