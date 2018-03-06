/*
 *
 */

#include "gmbox.h"
#include "gtime.h"
#include "gthread.h"
#include <stdio.h>
#include <string.h>


#define USE_MBOXV	1

#if ! USE_MBOXV
typedef struct _gmsg_t {
	char	msg[128];
} gmsg_t;
#endif

static int s_id = 0;
void *foo_put(void *data)
{
	
	int id = s_id++;

	size_t hmb = *(size_t *)data;
	printf( "FOO: %zd\n", hmb );

	const char *hello = "Hello!!!";
	int count = 0;
	for ( ;; )
	{
#if USE_MBOXV
		char buf[256];
		sprintf( buf, "foo[%d]: %s:%d", id, hello, count++ );
		if ( !gmboxv_put(hmb, buf, strlen(buf), 1000) )
#else
		gmsg_t msg;
		sprintf( msg.msg, "foo[%d]: %s:%d", id, hello, count++ );
		if ( !gmbox_put(hmb, &msg, 1000) )
#endif
			printf( "gmbox_put failed.\n" );
		else
			printf( "foo[%d]: msg added.\n", id );
		gtime_msleep(0);
	}
	printf( "End of FOO.\n" );
	return 0;
}

void *foo_get(void *data)
{
	size_t hmb = *(size_t *)data;

	int id = s_id++ + 100;
	for ( ;; )
	{
#if USE_MBOXV
		char buf[256];
		int slen = 0;
		if ( gmboxv_get(hmb, buf, sizeof(buf)-1, &slen) )
		{
			buf[slen] = '\0';
			printf( "foo[%d]: MSG=<%s> slen=%d\n", id, buf, slen );
			continue;
		}
#else
		gmsg_t msg;
		if ( gmbox_get(hmb, &msg) )
		{
			printf( "foo[%d]: MSG=<%s>\n", id, msg.msg );
			continue;
		}
#endif
	//	printf( "que empty.\n" );
		gtime_msleep(0);
	}
	return 0;
}

int main()
{
#if USE_MBOXV
	int hmb = gmboxv_create( 1000, 1 );
#else
	int hmb = gmbox_create( sizeof(gmsg_t), 64, 1 );
#endif
	if ( !hmb )
	{
		printf( "mbox creation failed.\n" );
		return 0;
	}

	const int MAX_THREAD = 10;
	GThread gt[MAX_THREAD];
	GThread pt[MAX_THREAD];
	int i;
	for (i=0; i<MAX_THREAD; ++i)
	{
		pt[i].start( foo_put, (void *)&hmb );
		gt[i].start( foo_get, (void *)&hmb );
	}

	gtime_msleep(10000);
		/*
	for (i=0; i<1000; )
	{
		char buf[256];
		int slen = 0;
		if ( gmbox_get(hmb, buf, sizeof(buf)-1, &slen) )
		{
			buf[slen] = '\0';
			printf( "main: MSG=<%s> slen=%d i=%d\n", buf, slen, i );
			++i;
			continue;
		}
	//	printf( "que empty.\n" );
		gtime_msleep(0);
	}
	*/
	for (i=0; i<MAX_THREAD; ++i)
	{
		pt[i].cancel();
		pt[i].join();
		gt[i].cancel();
		gt[i].join();
	}
	printf( "END of main.\n" );

#if USE_MBOXV
	gmboxv_delete( hmb );
#else
	gmbox_delete( hmb );
#endif
	return 0;
}
