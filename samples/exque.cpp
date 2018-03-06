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

	printf( "GQue: capacity=%d entries=%d rooms=%d\n",
			gq.getCapacity(), gq.getEntries(), gq.getRooms() );

	int count = 0;
	while ( !gq.isFull() )
	{
		int *pp = (int *)gq.tail();
		*pp = count;
		gq.shiftTail(1);

		printf( "%d added, capacity=%d entries=%d "
			"linear-entries=%d rooms=%d linear-rooms=%d\n",
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

		printf( "%d poped, capacity=%d entries=%d "
			"linear-entries=%d rooms=%d linear-rooms=%d\n",
			val, gq.getCapacity(), gq.getEntries(), gq.getLinearEntries(),
			gq.getRooms(), gq.getLinearRooms() );
	}
	printf( "--> 5 entries were poped.\n\n" );

	for (i=0; i<5; ++i)
	{
		int *pp = (int *)gq.tail();
		*pp = count;
		gq.shiftTail(1);

		printf( "%d added, capacity=%d entries=%d "
				"linear-entries=%d rooms=%d linear-rooms=%d\n",
			count, gq.getCapacity(), gq.getEntries(), gq.getLinearEntries(),
			gq.getRooms(), gq.getLinearRooms() );
		count++;
	}
	printf( "--> 5 entries were added again.\n\n" );

	printf( "*----- Full list -----*\n" );
	count = 0;
	int entries = gq.getEntries();
	int linear = gq.getLinearEntries();
	int rolled = entries - linear;
	printf( "entries=%d linear=%d rolled=%d\n", entries, linear, rolled );
	int *pp = (int *)gq.head();
	while ( --linear >= 0 )
		printf( "list[%d]=%d\n", count++, *pp++ );
	pp = (int *)gq.begin();
	while ( --rolled >= 0 )
		printf( "list[%d]=%d\n", count++, *pp++ );
	return 0;
}


typedef struct _vdata {
	int		dataSize;
	char	buf[512];
} vdata;

int gque_variable_size_test()
{
	vdata vdat;

	GQue gq;
	gq.create( 1, 150 );

	int i = 1;
	for (; i<50; ++i)
	{
		int size = i+4;
		vdat.dataSize = size;
		while ( !gq.requestLinearRooms(size) )
		{
			if ( gq.getCapacity() < (unsigned)size )
			{
				printf( "Error: Single item size exceeds total capacity.\n" );
				return 0;
			}

			// Pop out an item from head to make rooms
			vdata *vd = (vdata *)gq.head();
			int dsize = vd->dataSize;
			gq.shiftHead( vd->dataSize );

			printf( "vd of size=%d was poped. head=%d, tail=%d, end=%d rem=%d\n",
				dsize, gq.head()-gq.begin(), gq.tail()-gq.begin(),
				gq.end()-gq.begin(), gq.remainder() );
		}

		// Now, we've got enough linear rooms for item of size 'size'.
		// So, put it to queue.
		memcpy( gq.tail(), (char *)&vdat, size );
		gq.shiftTail( size );
		printf( "vd of size=%d was added -- head=%d, tail=%d, end=%d rem=%d\n",
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
