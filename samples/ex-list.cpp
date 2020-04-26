#include "gcl.h"
#include <stdio.h>

struct Item : public gcl::list<>::node {
    char msg[16];
};
using ItemList =gcl::list<Item>;


int main() {
    ItemList itemList;

    Item *pRemove = 0;

    for (int i=0; i<10; i++) {
        Item *item = new Item();
        sprintf(item->msg, "Couunt=%d", i);
        itemList.append(item);

        if (i==5) pRemove = item;

        printf("glist add #%d\n", i);
    }

    printf("Removing item #5\n");
    itemList.remove(pRemove);

    int count = 0;
    Item *item = itemList.first();
    while (item) {
        printf("entry #%i: %s\n", count++, item->msg);
        item = itemList.nextOf(item);
    }

}
