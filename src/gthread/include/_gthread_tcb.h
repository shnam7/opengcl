#pragma once
#include "_list.h"

typedef struct {
	HANDLE			h_thread;
	HANDLE			h_cancel;		/* cancel notify to wake blocked thread */
	int				canceled;		/* cancel signal */
	list_head		join_node;
	list_head		join_list;
	void            *join_retval;
} __gthread_tcb;
