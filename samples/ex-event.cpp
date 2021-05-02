/*
 *
 */

#include "gcl.h"
#include <stdio.h>
#include <string.h>

using namespace gcl;
using namespace gcl::time;

// class Runner : public runnable, public GEventEmitter {
// public:
//     void *run()
//     {
//         dmsg("starting runner...\n");
//         while (true) {
//             printf("emiting suspend\n");
//             emit("suspend");
//             sleep(100);
//         }
//         return 0;
//     }
// };

// void eventHandler(GEvent &e)
// {
//     printf("suspend listener...called\n");
// }

class Runner : public runnable, public event_emitter {
public:
    void *run()
    {
        dmsg("starting runner...\n");
        while (true) {
            printf("emiting 'suspend' event.\n");
            emit("suspend");
            sleep(100);
        }
        return 0;
    }
};

void event_handler(event_emitter::event &ev)
{
    printf("'suspend' event listener...called\n");
}

int main()
{
    thread t1;
    Runner runner;
    runner.once("suspend", event_handler, &t1);

    t1.start(runner);
    dmsg("main thread is now to sleep for 1 sec...\n");
    sleep(2000);
    dmsg("main thread is now cancelling the runner thread...");
    t1.cancel();

    dmsg("and waiting for the runner to stop...\n");
    t1.join();
    printf("END of main.\n");
    return 0;
}
