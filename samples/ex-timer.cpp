//
//		* extime.cpp
//
//		OpenGCL Example: gtime operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 7. 1. Tue.
//

#include "gtimer.h"
#include <stdio.h>

#include <time.h>

int main()
{
	int n=0;
	while ( n++<100 )
	{
		printf( "ticks=%lu nanoticks=%llu\n", GTimer::ticks(), GTimer::nanoTicks() );
//		gtime_msleep( 0 );
	}

	nanotick_t tm1 = GTimer::nanoTicks();
	// gtime_msleep( 1 );
    // ::nanosleep(&tm, &tm);
    // usleep(50);
	nanotick_t el = GTimer::nanoElapsed(tm1);

	printf( "tm1=%llu elapsed=%llu\n", tm1, el );
	// printf( "elapsed = %ld\n", elapsed );

	// tm1 = 0xFFFFFFFFUL - 10;
	// gtime_t tm2 = 10;
	// gtime_t v = tm2>tm1 ? tm2-tm1 : 0xFFFFFFFFUL-(tm1-tm2);
	// printf( "V=%ld\n", v );
	return 0;
}
