//
//		* exthread.c
//
//		OpenGCL Example: gthread operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 4. 29 Tue.
//
//
//
//

#include "gthread.h"
#include "gtimer.h"
#include <stdio.h>

void *foo(void *data)
{
	int val = (int)(uintptr_t)data;
	printf( "Starting: arg=%d\n", val );
    nanotick_t elapsed = 0;
    for (int i=0; i<10; i++) {
        nanotick_t tm = GTimer::nanoTicks();
        gsleep(300);
        // gnanosleep(500000000);
        elapsed = GTimer::nanoElapsed(tm);
		printf( "tid=%d count=%i nanotick=%zu elapsed=%zu\n", val, i, tm, elapsed);
    }
	return 0;
}

int main()
{
    gcl::gthread t1, t2;
    t1.start(foo, (void *)1);
    t2.start(foo, (void *)2);

	// printf( "Main\n" );
    // for (int i=0; i<5; i++) {
    //     printf("main count=%d\n", i);
    //     gcl::sleep(100);
    // }

    t1.join();
    t2.join();
    printf("End of main\n");
	return 0;
}
