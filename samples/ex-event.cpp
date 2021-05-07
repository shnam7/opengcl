/*
 *
 */

#include "gcl.h"
#include <stdio.h>
#include <string.h>

using namespace gcl;

class Runner : public runnable, public event_emitter {
public:
    void *run()
    {
        dmsg("starting runner...\n");
        while (true) {
            printf("emiting 'suspend' event.\n");
            emit("suspend");
            msleep(10);
        }
        return 0;
    }
};

void event_handler(event_emitter::event &)
{
    printf("'suspend' event listener...called\n");
}

void basic_test()
{
    thread t1;
    Runner runner;
    runner.on("suspend", event_handler, &t1);

    t1.start(runner);
    msleep(500);

    dmsg("\n--- Turning off the 'suspend' event handler.\n");
    runner.off("suspend", event_handler);
    msleep(500);

    dmsg("\n--- Cancelling the runner thread...\n");
    t1.cancel();
    t1.join();
    printf("END of main.\n");
}


int main()
{
    basic_test();
    return 0;
}
