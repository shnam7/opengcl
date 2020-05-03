//
//		* exthread_x.cpp
//
//		OpenGCL Example: gcl::thread operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 4. 29 Tue.
//

/* refer to notes in gthread.h and /usr/include/features.h */

#include "gcl.h"
#include <stdio.h>

GSemaphore sem(2);

void *foo(void *)
{
    int val = 0;
	GThread *pT = GThread::getCurrent();
    int tid = pT->threadID() % 100;

    printf("tid=%03u started\n", tid);
    sem.wait();
    sem.getValue(&val);
    printf("tid=%03u aquired sem. sem value=%d\n", tid, val);
    gsleep(1000);
    sem.post();
	printf( "tid%03u released sem\n", tid );
	return (void *)12345;
}


//#include <windows.h>
int main()
{
	int val = 0;
	void *retval = 0;
	GThread t1, t2, t3;

	printf( "Starting...\n" );
	t1.start( foo, 0 );
	t2.start( foo, 0 );
	t3.start( foo, 0 );

	sem.wait();
	sem.getValue(&val);
    printf("main: acquired sem. sem val=%d.\n", val);
    gsleep(1000);
	printf( "main: sem released\n");
    sem.post();

    t1.join( &retval );
	printf( "tid=%03zu finished. ret=%p\n", t1.threadID(), retval );

	t2.join( &retval );
	printf( "tid=%03zu finished. ret=%p\n", t2.threadID(), retval );

	t3.join( &retval );
    printf( "tid=%03zu finished. ret=%p\n", t3.threadID(), retval );

	sem.getValue(&val);
	printf( "Finally, Sem value=%d\n", val );
	return 0;
}
