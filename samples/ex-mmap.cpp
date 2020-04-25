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
	gcl::mmapfile mfile;
	const char *cp = mfile.mmap_ro( "../samples/ex-mmap.cpp" );

	size_t size = mfile.getSize();
	printf( "File mapping: r=%p Size=%zu\n", cp, size );
	for (unsigned i=0; i<size; ++i)
		printf( "%c", mfile[i] );

	mfile.close();

	char *p = mfile.mmap_vrw( 1024*1024 );
	size = mfile.getSize();
	printf( "Virtual memory mapping: r=%p Size=%zu\n", p, size );
	memset( p, 'A', size );
	for (unsigned i=0; i<size; ++i)
	{
		if ( mfile[i] != 'A' )
		{
			printf( "-->Error: data was not written.\n" );
			return -1;
		}
	}
	printf( "--> Ok!\n" );
	return 0;
}

