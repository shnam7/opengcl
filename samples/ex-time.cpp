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

#include <time.h>

using namespace gcl;
using namespace gcl::chrono;

int main()
{
	int n=0;
	while ( n++<100 )
	{
		printf( "ticks=%ld nanoticks=%lld\n", ticks(), nanoTicks() );
//		gtime_msleep( 0 );
	}

    timespec tm = { 0, 5000 };
	nanotick_t tm1 = nanoTicks();
	// gtime_msleep( 1 );
    // ::nanosleep(&tm, &tm);
    // usleep(50);
	nanotick_t el = nanoElapsed(tm1);

	printf( "tm1=%lld elapsed=%lld\n", tm1, el );
	// printf( "elapsed = %ld\n", elapsed );

	// tm1 = 0xFFFFFFFFUL - 10;
	// gtime_t tm2 = 10;
	// gtime_t v = tm2>tm1 ? tm2-tm1 : 0xFFFFFFFFUL-(tm1-tm2);
	// printf( "V=%ld\n", v );
	return 0;
}

