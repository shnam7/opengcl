/*
 *
 */

#include "gmbox.h"
#include "gtimer.h"
#include "gthread.h"
#include <stdio.h>
#include <string.h>

typedef struct { char msg[128]; } msg_t;
using MsgBox = gcl::mbox<msg_t>;

void *foo_put(void *data)
{
    unsigned tid = ((unsigned)(gcl::gthread::getCurrent()->threadID()) % 100);
    MsgBox &mbox = *(MsgBox *)data;

    int count = 0;
	for (;;)
	{
		msg_t msg;
		sprintf( msg.msg, "id=%d count=%d", tid, count++ );
		if ( mbox.put(msg) ) {
            printf( "putter:%s qsize=%d\n", msg.msg, mbox.length() );
        }
		gcl::sleep(0);
	}
	return 0;
}

void *foo_get(void *data)
{
    unsigned tid = ((unsigned)(gcl::gthread::getCurrent()->threadID()) % 100);
    MsgBox &mbox = *(MsgBox *)data;

    int count = 0;
	for (;;)
	{
		msg_t msg;
		sprintf( msg.msg, "putter id=%d count=%d", tid, count++ );
		if ( mbox.get(&msg) ) {
            printf( "getter:%s qsize=%d\n", msg.msg, mbox.length() );
        }
		gcl::sleep(0);
	}
	return 0;
}

int main()
{
    MsgBox mbox(100);

	const int MAX_THREAD = 10;
	gcl::gthread getters[MAX_THREAD];
	gcl::gthread putters[MAX_THREAD];
	for (int i=0; i<MAX_THREAD; ++i)
	{
		putters[i].start( foo_put, &mbox );
		getters[i].start( foo_get, &mbox );
	}

    gcl::sleep(1000);
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
