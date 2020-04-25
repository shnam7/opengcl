//
//		* exthread_x.cpp
//
//		OpenGCL Example: gcl::thread operations
//
//		Written by Soo-Hyuk Nam.
//			2003. 4. 29 Tue.
//

/* refer to notes in gthread.h and /usr/include/features.h */

#include "gthread.h"
#include <stdio.h>
#include <gtime.h>

/*
//--------------------------------------------------------------------
//	class MyThread
//--------------------------------------------------------------------
class MyThread : public gcl::thread {
protected:
	int		m_val;
	int		m_inc;

public:
	MyThread(int inc) : m_val(0), m_inc(inc) {}

	int getValue() const { return m_val; }

protected:
	virtual int run();
};

int MyThread::run()
{
	for ( ;; )
	{
		testCancel();
		printf( "MyThread with inc %d: %d\n", m_inc, m_val );
		m_val += m_inc;
		gcl::sleep( 1 );
	}
	return 0;
}


//--------------------------------------------------------------------
//	class MsgPrinter
//--------------------------------------------------------------------
class MsgPrinter : public GRunnable {
protected:
	virtual int run(gcl::thread *pThread);
};

int MsgPrinter::run(gcl::thread *pThread)
{
	for ( ;; )
	{
		pThread->testCancel();
		printf( "Hello!!!!\n" );
		gcl::sleep( 1 );
	}
	return 0;
}

//--------------------------------------------------------------------
//	class MsgPrinter
//--------------------------------------------------------------------
class AutoMsgPrinter : public GRunnable {
protected:
	gcl::thread		m_t;

public:
	AutoMsgPrinter() { m_t.start(this); }
	virtual ~AutoMsgPrinter() {}
	virtual int run(gcl::thread *pThread);
};

int AutoMsgPrinter::run(gcl::thread *pThread)
{
	for ( ;; )
	{
		pThread->testCancel();
		printf( "Auto: Hello!!!!\n" );
		gcl::sleep( 1 );

	}
	return 0;
}


GMutex s_mtx;
GSemaphore s_sem;
*/

#include "gsem.h"

gsem_t sem;

void *foo(void *)
{
	gcl::gthread *pT = gcl::gthread::getCurrent();
	for (int i=0; i<5; ++i)
	{
		gthread_testcancel();
		printf( "foo: Thread %ld running.\n", (long)pT->threadID() );
		gcl::sleep( 100 );
	}
	gcl::sleep(1000);

	printf( "SEM post!!\n" );
	gsem_post(&sem);


	//printf( "foo: sleeping for 10 secs...\n" );
	gcl::sleep( 10000 );
	//printf( "foo: woken up...\n" );

	//printf( "foo: waiting for Mutex...(endless)!\n" );
	//bool r = s_mtx.lock();
	//printf( "foo: woken up from mutex:%d!\n", r );

//	printf( "foo: waiting for Semaphore...(endless)!\n" );
//	bool r = s_sem.wait();
//	printf( "foo: woken up from semaphore:%d!\n", r );

	return (void *)12345;
}


//#include <windows.h>
int main()
{
	gsem_init(&sem, 0, 0);
	printf( "Starting...\n" );

	gcl::gthread gt, gt1, gt2;
	printf( "main: creating thread #%ld...\n", (long)gt.threadID() );
	if ( gt.start(foo, 0) != 0 )
	{
		printf( "thread creation failed.\n" );
		return -1;
	}
	gt1.start( foo, 0 );
	gt2.start( foo, 0 );

	gsem_wait(&sem);
	int val;
	gsem_getvalue( &sem, &val );
	printf( "Done.: %d\n", val );


//	printf( "main: sleeping for 1 sec...\n" );
    gcl::sleep( 1000 );
//	printf( "main: canceling thread #%ld1...\n", gt.threadID() );

	void *retval = 0;;
	gt.join( &retval );
	printf( "ret=%p\n", retval );

	retval = 0;;
	gt1.join( &retval );
	printf( "ret=%p\n", retval );

	retval = 0;
	gt2.join( &retval );
	printf( "ret=%p\n", retval );

	gsem_getvalue( &sem, &val );
	printf( "Finally, Sem value=%d\n", val );
	return 0;
	/*

	printf( "main: waiting 3 sec...\n" );
	gcl::sleep( 3000 );
	printf( "main: canceling thread #0...\n" );
	s_sem.post();
	t0.cancel();
	printf( "main: joining to thread #0...\n" );
	t0.join();
	return 0;

	MyThread t1(1), t2(2);
	t1.start();
	t2.start();

	MsgPrinter mp;
	gcl::thread t3;
	t3.start( &mp );

	AutoMsgPrinter amp;

	while ( t1.isRunning() || t2.isRunning() || t3.isRunning() )
	{
		int v = t1.getValue();
		if ( v >= 10 ) t2.cancel();
		if ( v >= 20 ) t3.cancel();
		if ( v >= 30 ) t1.cancel();
		gcl::sleep( 1 );
	}
	*/

	return 0;
}
