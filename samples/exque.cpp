//
//		* exque.cpp
//
//		OpenGCL Example: queue operations
//
//		Written by Soo-Hyuk Nam.
//			2001. 1. 9. Tue.
//

#include <gque.h>
#include <stdio.h>
#include <memory.h>

int gque_test()
{
	GQue gq;
	gq.create( sizeof(int), 10 );

	printf( "GQue: capacity=%zd entries=%zd rooms=%zd\n",
			gq.getCapacity(), gq.getEntries(), gq.getRooms() );

	size_t count = 0;
	while ( !gq.isFull() )
	{
		int *pp = (int *)gq.tail();
		*pp = count;
		gq.shiftTail(1);

		printf( "%zd added, capacity=%zd entries=%zd "
			"linear-entries=%zd rooms=%zd linear-rooms=%zd\n",
			count, gq.getCapacity(), gq.getEntries(), gq.getLinearEntries(),
			gq.getRooms(), gq.getLinearRooms() );

		++count;
	}
	printf( "--> GQue filled.\n\n" );

	int i;
	for (i=0; i<5; ++i)
	{
		int *pp = (int *)gq.head();
		int val = *pp;
		gq.shiftHead(1);

		printf( "%d poped, capacity=%zd entries=%zd "
			"linear-entries=%zd rooms=%zd linear-rooms=%zd\n",
			val, gq.getCapacity(), gq.getEntries(), gq.getLinearEntries(),
			gq.getRooms(), gq.getLinearRooms() );
	}
	printf( "--> 5 entries were poped.\n\n" );

	for (i=0; i<5; ++i)
	{
		int *pp = (int *)gq.tail();
		*pp = count;
		gq.shiftTail(1);

		printf( "%zd added, capacity=%zd entries=%zd "
				"linear-entries=%zd rooms=%zd linear-rooms=%zd\n",
			count, gq.getCapacity(), gq.getEntries(), gq.getLinearEntries(),
			gq.getRooms(), gq.getLinearRooms() );
		count++;
	}
	printf( "--> 5 entries were added again.\n\n" );

	printf( "*----- Full list -----*\n" );
	count = 0;
	size_t entries = gq.getEntries();
	size_t linear = gq.getLinearEntries();
	size_t rolled = entries - linear;
	printf( "entries=%zd linear=%zd rolled=%zd\n", entries, linear, rolled );
	int *pp = (int *)gq.head();
	while ( --linear >= 0 )
		printf( "list[%zd]=%d\n", count++, *pp++ );
	pp = (int *)gq.begin();
	while ( --rolled >= 0 )
		printf( "list[%zd]=%d\n", count++, *pp++ );
	return 0;
}


typedef struct _vdata {
	size_t		dataSize;
	char		buf[512];
} vdata;

int gque_variable_size_test()
{
	vdata vdat;

	GQue gq;
	gq.create( 1, 150 );

	size_t i = 1;
	for (; i<50; ++i)
	{
		size_t size = i+4;
		vdat.dataSize = size;
		while ( !gq.requestLinearRooms(size) )
		{
			if ( gq.getCapacity() < size )
			{
				printf( "Error: Single item size exceeds total capacity.\n" );
				return 0;
			}

			// Pop out an item from head to make rooms
			vdata *vd = (vdata *)gq.head();
			size_t dsize = vd->dataSize;
			gq.shiftHead( vd->dataSize );

			printf( "vd of size=%zd was poped. head=%zd, tail=%zd, end=%ld rem=%zd\n",
				dsize, gq.head()-gq.begin(), gq.tail()-gq.begin(),
				gq.end()-gq.begin(), gq.remainder() );
		}

		// Now, we've got enough linear rooms for item of size 'size'.
		// So, put it to queue.
		memcpy( gq.tail(), (char *)&vdat, size );
		gq.shiftTail( size );
		printf( "vd of size=%zd was added -- head=%zd, tail=%zd, end=%zd rem=%zd\n",
				size, gq.head()-gq.begin(), gq.tail()-gq.begin(),
				gq.end()-gq.begin(), gq.remainder() );
	}
	return 0;
}

int gque_buffer_test()
{
	// GQueBuffer can be used as a dynamically allocated buffer
	// in addition to queue operations.
	GQue gqb;
	if ( !gqb.create(1, 100) )
	{
		printf( "Error: cannot init GQueBuffer.\n" );
		return -1;
	}

	char *pbuf = gqb.head();
	memset( gqb.head(), 'A', 100 );
	int i;
	for (i=0; i<100; ++i)
	{
		if ( *pbuf++ != 'A' )
		{
			printf( "Error... Ooops!~\n" );
			return -1;
		}
	}
	printf( "GQueBuffer test ok!.\n" );
	return 0;
}

int main()
{
	//return gque_test();
	return gque_variable_size_test();
	//return gque_buffer_test();
}
