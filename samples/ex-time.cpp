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

int main()
{
	int n=0;
	while ( n++<100 )
	{
		printf( "ticks=%ld\n", gtime_get_ticks() );
//		gtime_msleep( 0 );
	}

	gtime_t tm1 = gtime_get_ticks();
	gtime_msleep( 1 );
	gtime_t elapsed = gtime_get_elapsed( tm1 );

	printf( "tm1 = %ld\n", tm1 );
	printf( "elapsed = %ld\n", elapsed );
	
	tm1 = 0xFFFFFFFFUL - 10;
	gtime_t tm2 = 10;
	gtime_t v = tm2>tm1 ? tm2-tm1 : 0xFFFFFFFFUL-(tm1-tm2);
	printf( "V=%ld\n", v );
	return 0;
}

