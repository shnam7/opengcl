/*
***************************************************************************
* This File is a part of OpenGCL.
* Copyright (c) 2004 Soo-Hyuk Nam (shnam7@gmail.com)
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public License
* as published by the Free Software Foundation: either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Library General Public License for more details.
*
* Should this software be used in another application, an acknowledgement
* that OpenGCL code is used would be appreciated, but it is not mandatory.
*
***************************************************************************
*/

/*
 *	* gmbox.cpp
 *
 *	OpenGCL Module : gmbox - General purpose message box
 *
 *	Written by Soo-Hyuk Nam.
 *		2004. 4. 21. Wed.
 *
 *	History:
 *		2004/04/21: First Written.
 *
 */

#include "gmbox.h"
#include "gque.h"
#include "gthread.h"
#include "gtime.h"
#include <stdlib.h>
#include <string.h>


static inline gque_t *_mb(gmbox_t hmb) { return (gque_t *)hmb; }

static inline gthread_mutex_t *_mtx(gmbox_t hmb)
{
	return (gthread_mutex_t *)_mb(hmb)->opt;
}

static inline gthread_cond_t *_cond(gmbox_t hmb)
{
	return (gthread_cond_t *)(((char *)_mb(hmb)->opt)
			+ sizeof(gthread_mutex_t) );
}

static inline void _lock(gmbox_t hmb)
{
	gthread_mutex_t *mtx = _mtx(hmb);
	if ( mtx ) gthread_mutex_lock( mtx );
}

static inline void _unlock(gmbox_t hmb)
{
	gthread_mutex_t *mtx = _mtx(hmb);
	if ( mtx ) gthread_mutex_unlock( mtx );
}

static inline int _wait(gmbox_t hmb, unsigned long msec)
{
	return gthread_cond_timedwait( _cond(hmb), _mtx(hmb), msec );
}

static inline void _bcast(gmbox_t hmb)
{
	gthread_cond_broadcast( _cond(hmb) );
}

static gmbox_t _gmbox_create(unsigned msg_size, unsigned que_size, int use_lock)
{
	gque_t *mb = gque_create(msg_size, que_size);
	if ( !mb ) return 0;

	if ( use_lock )
	{
		mb->opt = malloc( sizeof(gthread_mutex_t) + sizeof(gthread_cond_t) );
		if ( !mb->opt )
		{
			gque_delete( mb );
			return 0;
		}
		gthread_mutex_init( _mtx((gmbox_t)mb), 0 );
		gthread_cond_init( _cond((gmbox_t)mb), 0 );
	}
	return (gmbox_t)mb;
}

static int _gmbox_delete(gmbox_t hmb)
{
	if ( hmb == 0 ) return 0;
	if ( _mb(hmb)->opt )
	{
		gthread_mutex_destroy( _mtx(hmb) );
		gthread_cond_destroy( _cond(hmb) );
		free( _mb(hmb)->opt );
	}
	return gque_delete( _mb(hmb) );
}


/* mbox for fixed size messages */
gcl_api gmboxv_t gmbox_create(unsigned msg_size, unsigned que_size, int use_lock)
{
	return _gmbox_create( msg_size, que_size, use_lock );
}

gcl_api int gmbox_delete(gmbox_t hmb)
{
	return _gmbox_delete(hmb);
}

gcl_api int gmbox_put(gmbox_t hmb, const void *msg, unsigned long timeout)
{
	if ( hmb == 0 ) return 0;
	_lock( hmb );
	while ( gque_is_full(_mb(hmb)) )
	{
		if ( _wait(hmb, timeout) != 0 )
		{
			_unlock( hmb );
			return 0;
		}
	}
	gque_push_tail( _mb(hmb), (const char *)msg );
	_unlock(hmb);
	return 1;
}

static int _gmbox_get(gmbox_t hmb, void *buf, int is_peek)
{
	const char *pos;

	if ( hmb == 0 ) return 0;
	_lock(hmb);
	if ( gque_is_empty(_mb(hmb)) )
	{
		_unlock(hmb);
		return 0;
	}
	pos = gque_head( _mb(hmb) );
	if ( buf ) memcpy( buf, pos, gque_item_size(_mb(hmb)) );
	if ( !is_peek ) gque_pop_head( _mb(hmb) );
	_unlock(hmb);
	_bcast(hmb);
	return 1;
}

gcl_api int gmbox_get(gmbox_t hmb, void *buf)
{
	return _gmbox_get(hmb, buf, 0);
}

gcl_api int gmbox_peek(gmbox_t hmb, void *buf)
{
	return _gmbox_get(hmb, buf, 1);
}


/* mbox for variable size messages */
gmboxv_t gmboxv_create(unsigned mbox_size, int use_lock)
{
	return _gmbox_create( 1, mbox_size, use_lock );
}

int gmboxv_delete(gmboxv_t hmb)
{
	return _gmbox_delete( hmb );
}

int gmboxv_put(gmboxv_t hmb, const void *msg, int msg_size, unsigned long timeout)
{
	int size;

	if ( hmb==0 || msg_size<=0 ) return 0;
	_lock( hmb );
	size = msg_size + sizeof(int);
	while ( !gque_request_linear_rooms(_mb(hmb), size) )
	{
		if ( _wait(hmb, timeout) != 0 )
		{
			_unlock( hmb );
			return 0;
		}
	}
	if ( msg )
	{
		char *pos = gque_tail( _mb(hmb) );
		memcpy( pos, &msg_size, sizeof(int) );
		memcpy( pos+sizeof(int), msg, msg_size );
		gque_shift_tail( _mb(hmb), msg_size+sizeof(int) );
	}
	_unlock(hmb);
	return 1;
}

static int _gmboxv_get(gmboxv_t hmb, void *buf, int bufsize, int *msg_size, int is_peek)
{
	const char *pos;
	int size;

	if ( hmb == 0 ) return 0;
	_lock(hmb);
	if ( gque_is_empty(_mb(hmb)) )
	{
		_unlock(hmb);
		return 0;
	}
	pos = gque_head( _mb(hmb) );
	memcpy( &size, pos, sizeof(int) );
	if ( size<=0 || size>bufsize )
	{
		_unlock(hmb);
		return 0;
	}
	
	memcpy( buf, pos+sizeof(int), size );
	gque_shift_head( _mb(hmb), size+sizeof(int) );
	_unlock(hmb);
	_bcast(hmb);
	if ( msg_size ) *msg_size = size;
	return 1;
}

int gmboxv_get(gmboxv_t hmb, void *buf, int bufsize, int *msg_size)
{
	return _gmboxv_get(hmb, buf, bufsize, msg_size, 0);
}

int gmboxv_peek(gmboxv_t hmb, void *buf, int bufsize, int *msg_size)
{
	return _gmboxv_get(hmb, buf, bufsize, msg_size, 1);
}
