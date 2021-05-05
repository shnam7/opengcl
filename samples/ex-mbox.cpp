/*
 *
 */

#include "gtime.h"
#include "gthread.h"
#include "gque.h"
#include <stdio.h>
#include <string.h>

using namespace gcl;
using namespace gcl::time;

typedef struct { char msg[128]; } msg_t;
typedef queue_mt<msg_t> MsgBox;

void *foo_put(void *data)
{
    unsigned tid = thread::get_current()->thread_id();
    MsgBox &mbox = *(MsgBox *)data;

    int count = 0;
	for (;;)
	{
		msg_t msg;
		sprintf( msg.msg, "id=%d count=%d", tid, count++ );
		if ( mbox.put(msg) ) {
            printf( "putter:%s qsize=%d\n", msg.msg, mbox.length() );
        }
		msleep(0);
	}
}

void *foo_get(void *data)
{
    unsigned tid = thread::get_current()->thread_id();
    MsgBox &mbox = *(MsgBox *)data;

    int count = 0;
	for (;;)
	{
		msg_t msg;
		sprintf( msg.msg, "putter id=%d count=%d", tid, count++ );
		if ( mbox.get(&msg) ) {
            printf( "getter:%s qsize=%d\n", msg.msg, mbox.length() );
        }
		msleep(0);
	}
}

int main()
{
    MsgBox mbox(200);

	const int MAX_THREAD = 10;
	thread getters[MAX_THREAD];
	thread putters[MAX_THREAD];
	for (int i=0; i<MAX_THREAD; ++i)
	{
		putters[i].start( foo_put, &mbox );
		getters[i].start( foo_get, &mbox );
	}

    tick_t tm1 = ticks();
    msleep(5000);
    tick_t elapsed = elapsed_nsec(tm1);
    printf("------------------------------TIMEOUT------------------\n");
    printf("elapsed=%zu\n", elapsed);

	for (int i=0; i<MAX_THREAD; ++i)
	{
		putters[i].cancel();
		getters[i].cancel();
	}

    printf("==> Test Cancel requested\n");
	for (int i=0; i<MAX_THREAD; ++i)
	{
		putters[i].join();
		getters[i].join();
	}
	printf( "END of main.\n" );
	return 0;
}
