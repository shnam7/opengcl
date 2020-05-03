/*
 *	* gthread_self.c
 *
 *	OpenGCL Module : gthread - WIN32 support for thread-aware sleep.
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 1. 15. Thu.
 *
 *	History:
 *		2004/01/15: Originally written.
 */

#include "gcldef.h"

#if defined (_WIN32)

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#include <stdint.h>
#include <windows.h>
#include "_gthread.h"
#include "_gthread_tcb.h"


extern gthread_key_t        _gkey_self;
gcl_api void gthread_sleep(u32_t msec)
{
	__gthread_tcb *tcb = (__gthread_tcb *)TlsGetValue(_gkey_self);
	if ( !tcb ) { Sleep(msec); return; }
	unsigned r = WaitForSingleObject( tcb->h_cancel, msec );

    // sleep() shoud be test cancel check point. So check it here.
    if (r == WAIT_OBJECT_0) gthread_testcancel();   // cancel signaled
}

//-- nano wait is not supported yet
gcl_api void gthread_nanosleep(u64_t nsec) {
    return gthread_sleep((u32_t)(nsec/ 1000000));
}

#else
#include <time.h>

void gthread_nanosleep(u64_t nsec)
{   struct timespec ts = { (long)(nsec / 1000000000L), (long)((nsec % 1000000000))};
    nanosleep(&ts, 0);
}

void gthread_sleep(u32_t msec) {
    return gthread_nanosleep( msec * 1000000);
}

#endif


// void timerFunc(LPVOID lpArgToCompletionRoutine, DWORD dwTimerLowValue, DWORD dwTimerHighValue)
// {
//     __gthread_tcb *tcb = (__gthread_tcb *)lpArgToCompletionRoutine;
//     // dmsg("timeFunc called\n");
//     SetEvent(tcb->h_cancel);
//     // WaitForSingleObjectEx(tcb->h_cancel, INFINITE, TRUE);
// }

//-- nano wait is not supported yet
// gcl_api void gthread_nanosleep(u64_t nsec) {
//     return gthread_sleep((u32_t)(nsec/ 1000000));

//     gthread_sleep( (unsigned)(nsec/1000000));

//     __gthread_tcb *tcb = (__gthread_tcb *)TlsGetValue(_gkey_self);
// 	if ( !tcb ) { Sleep((unsigned)(nsec / 1000000)); return; }

//     LARGE_INTEGER liDueTime;
//     i64_t qwDueTime = -5 * nsec;
//     liDueTime.LowPart  = (DWORD) ( qwDueTime & 0xFFFFFFFF );
//     liDueTime.HighPart = (LONG)  ( qwDueTime >> 32 );

//     // bool bSuccess = SetWaitableTimer(
//     //        hTimer,                 // Handle to the timer object.
//     //        &liDueTime,             // When timer will become signaled.
//     //        2000,                   // Periodic timer interval of 2 seconds.
//     //        timerFunc,           // Completion routine.
//     //        tcb,                // Argument to the completion routine.
//     //        FALSE );                // Do not restore a suspended system.

//     // // wait forever
//     // SleepEx(
//     //     INFINITE,           // Wait forever.
//     //     TRUE );             // IMPORTANT!!! The thread must be in an
//     //                                // alertable state to process the APC.

//     if (!SetWaitableTimer(hTimer, &liDueTime, 0, timerFunc, tcb, FALSE)) {
//         CloseHandle(hTimer);
//         return;
//     };

//     // WaitForSingleObjectEx(hTimer, INFINITE, TRUE);
//     WaitForSingleObjectEx(tcb->h_cancel, INFINITE, FALSE);
// }
