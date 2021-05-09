#include "gcl.h"
#include <stdio.h>

using namespace gcl;

struct Item : public Linkable {
    char msg[16];
};

void basic_test() {
    List<Item> item_list;

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

void iterator_test() {
    List<Item> item_list;
    for (int i=0; i<10; i++) {
        Item *item = new Item();
        sprintf(item->msg, "Count=%d", i);
        item_list.append(item);
    }

    printf("Printing -----------\n");
    List<Item>::Iterator it = item_list.begin();
    while (it != item_list.end()) {
        printf("glist add #%s\n", it++->msg);
    }

    printf("Printing revirse-----\n");
    it = item_list.rbegin();
    while (it != item_list.rend()) {
        printf("glist add #%s\n", it--->msg);
    }
}

int main() {
    iterator_test();
}
