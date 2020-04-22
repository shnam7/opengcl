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
	const char *cp = gmm.mmap_ro( "../samples/ex-mmap.cpp" );

	size_t size = gmm.getSize();
	printf( "File mapping: r=%p Size=%zu\n", cp, size );
	for (unsigned i=0; i<size; ++i)
		printf( "%c", gmm[i] );

	gmm.close();

	char *p = gmm.mmap_vrw( 1024*1024 );
	size = gmm.getSize();
	printf( "Virtual memory mapping: r=%p Size=%zu\n", p, size );
	memset( p, 'A', size );
	for (unsigned i=0; i<size; ++i)
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

