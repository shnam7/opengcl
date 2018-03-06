//
//		* exmmap.cpp
//
//		OpenGCL Example: GMMap/GMMapFile operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 7. 1. Tue.
//

#include "gmmap.h"
#include <stdio.h>
#include <memory.h>

int main()
{
	GMMapFile gmm;
#if defined( _WIN32 )
	const char *cp = gmm.mmap_ro( "../samples/exmmap.cpp" );
#else
	const char *cp = gmm.mmap_ro( "exmmap.cpp" );
#endif

	int sz = gmm.getSize();
	int i;
	printf( "File mapping: r=%p Size=%d\n", cp, sz );
	for (i=0; i<sz; ++i)
		printf( "%c", gmm[i] );

	gmm.close();

	char *p = gmm.mmap_vrw( 1024*1024 );
	sz = gmm.getSize();
	printf( "Virtual memory mapping: r=%p Size=%d\n", p, sz );
	memset( p, 'A', sz );
	for (i=0; i<sz; ++i)
	{
		if ( gmm[i] != 'A' )
		{
			printf( "-->Error: data was not written.\n" );
			return -1;
		}
	}
	printf( "--> Ok!\n" );
	return 0;
}

