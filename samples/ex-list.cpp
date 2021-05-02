#include "gcl.h"
#include <stdio.h>

using namespace gcl;

struct Item : public linkable {
    char msg[16];
};


int main() {
    list<Item> item_list;

    Item *item_to_remove = 0;

    for (int i=0; i<10; i++) {
        Item *item = new Item();
        sprintf(item->msg, "Couunt=%d", i);
        item_list.append(item);

        if (i==5) item_to_remove = item;

        printf("glist add #%d\n", i);
    }

    printf("Removing item #5\n");
    delete item_to_remove;

    unsigned len = item_list.length();
    printf("item list length: %d\n", len);

    int count = 0;
    Item *item = item_list.first();
    while (item) {
        printf("entry #%i: %s\n", count++, item->msg);
        item = item_list.next_of(item);
    }

    // delete all
    item = item_list.first();
    while (item) {
        delete item;
        item = item_list.first();
    }
}
