/*
 *
 */

#include "gcl.h"
#include <stdio.h>
#include <string.h>

class Runner : public GThread::runnable, public GEventEmitter {
public:
    void *run()
    {
        while (true) {
            printf("emiting suspend\n");
            emit("suspend");
            gsleep(100);
        }
        return 0;
    }
};

void eventHandler(GEvent &e)
{
    printf("suspend listener...called\n");
}

int main()
{
    GThread t1;
    Runner runner;
    runner.on("suspend", eventHandler, &t1);

    t1.start(&runner);
    gsleep(1000);
    t1.cancel();

    t1.join();
    printf("END of main.\n");
    return 0;
}
