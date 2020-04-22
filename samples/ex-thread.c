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
#include <stdio.h>


void *foo(void *data)
{
	int val = (int)(uintptr_t)data;
	printf( "Foo=%d\n", val );
	//for ( ;; )
		printf( "Hello %d\n", val );

	return 0;
}

int main()
{
	gthread_t gt;
	int r = gthread_create( &gt, 0, foo, 0 );
	if ( r != 0 )
	{
		printf( "Error: code=%d\n", r );
		return -1;
	}

	printf( "Main\n" );
	gthread_join( gt, 0 );
	return 0;
}
