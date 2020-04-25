/*
 *
 */

#include "gmbox.h"
#include "gtime.h"
#include "gthread.h"
#include <stdio.h>
#include <string.h>

typedef struct { char msg[128]; } msg_t;
typedef gcl::msgbox<msg_t> msgbox;

void *foo_put(void *data)
{
    unsigned tid = ((unsigned)(gcl::gthread::getCurrent()->threadID()) % 100);
    msgbox *mb = (msgbox *)data;
    int count = 0;
	for (;;)
	{
		msg_t msg;
		sprintf( msg.msg, "id=%d count=%d", tid, count++ );
		if ( mb->put(msg) ) {
            printf( "putter:%s qsize=%d\n", msg.msg, mb->length() );
        }
        // printf( "putter failed: tid=%d count=%d.\n", tid, count );
		gcl::sleep(0);
		pthread_testcancel();
	}
	return 0;
}

void *foo_get(void *data)
{
    unsigned tid = ((unsigned)(gcl::gthread::getCurrent()->threadID()) % 100);
    msgbox *mb = (msgbox *)data;
    int count = 0;
	for (;;)
	{
		msg_t msg;
		sprintf( msg.msg, "putter id=%d count=%d", tid, count++ );
		if ( mb->get(&msg) ) {
            printf( "getter:%s qsize=%d\n", msg.msg, mb->length() );
        }
		// printf( "getter failed: tid=%d count=%d.\n", tid, count );
		gcl::sleep(0);
		pthread_testcancel();

        // if (count > 100) return 0;
	}
	return 0;
}

int main()
{
    msgbox mbox(100);

	const int MAX_THREAD = 10;
	gcl::gthread getters[MAX_THREAD];
	gcl::gthread putters[MAX_THREAD];
	for (int i=0; i<MAX_THREAD; ++i)
	{
		putters[i].start( foo_put, &mbox );
		getters[i].start( foo_get, &mbox );
	}

    gcl::sleep(2000);
    printf("------------------------------TIMEOUT------------------\n");
	for (int i=0; i<MAX_THREAD; ++i)
	{
		putters[i].cancel();
		getters[i].cancel();
		putters[i].join();
		getters[i].join();
	}
    printf("==> Test Cancel requested\n");
	printf( "END of main.\n" );
	return 0;
}
