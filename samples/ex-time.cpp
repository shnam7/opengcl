//
//		* extime.cpp
//
//		OpenGCL Example: gtime operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 7. 1. Tue.
//

#include "gtime.h"
#include <stdio.h>

using namespace gcl::time;

int main()
{
    tick_t tm1 = ticks();
    sleep(5);
    tick_t el = elapsed_nsec(tm1);
    printf("tm1=%zu elapsed=%zu\n", tm1, el);

    return 0;
}
