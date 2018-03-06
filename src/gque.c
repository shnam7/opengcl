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
 *	* gque.c
 *
 *	OpenGCL Module : Generic Queue with extraordinary features.
 *
 *	Written by Soo-Hyuk Nam.
 *		2003. 11. 26. Wed.
 *
 *	History:
 *		2003/11/26: First written.
 *		2003/08/25: Prototype designed(IoQue).
 */

#include "gque.h"
#include <stdlib.h>
#include <memory.h>

gque_t *gque_create(size_t item_size, size_t capacity)
{
	char *mem = (char *)malloc( gque_calc_bufsize(item_size, capacity) );
	if ( !mem ) return 0;
	return gque_init( item_size, capacity, mem );
}

int gque_delete(gque_t *gq)
{
	if ( !gq ) return 0;
	free( gq );
	return 1;
}

gque_t *gque_init(size_t item_size, size_t capacity, char *buf)
{
	gque_t *gq = (gque_t *)buf;
	gq->item_size = item_size;
	gq->begin = sizeof(gque_t);
	gq->end   = gq->begin + item_size*(capacity+1);
	gq->head  = gq->begin;
	gq->tail  = gq->begin;
	gq->remainder = 0;
	gq->opt = 0;
	return gq;
}

int gque_push_tail(gque_t *gq, const char *item)
{
	size_t npos = gq->tail + gq->item_size;
	if ( npos == gq->end ) npos = gq->begin;
	if ( npos == gq->head ) return 0;		/* full */
	memcpy( ((char *)gq)+gq->tail, item, gq->item_size );
	gq->tail = npos;
	return 1;
}

int gque_pop_head(gque_t *gq)
{
	if ( gq->head == gq->tail ) return 0;	/* empty */
	gq->head += gq->item_size;
	if ( gq->head == gq->end ) gq->head = gq->begin;
	return 1;
}

int gque_shift_head(gque_t *gq, int n)
{
	gq->head += n*gq->item_size;
	if ( gq->head >= gq->end )
		gq->head = gq->begin + (gq->head-gq->end);
	return 1;
}

int gque_shift_tail(gque_t *gq, int n)
{
	gq->tail += n*gq->item_size;
	if ( gq->tail >= gq->end )
		gq->tail = gq->begin + (gq->tail-gq->end);
	return 1;
}

int gque_is_full(gque_t *gq)
{
	size_t npos = gq->tail + gq->item_size;
	if ( npos == gq->end ) npos = gq->begin;
	return ( npos == gq->head );
}

size_t gque_get_entries(gque_t *gq)
{
	size_t n = ( gq->head<=gq->tail ) ? (gq->tail-gq->head)
			: (gq->end-gq->head) + (gq->tail-gq->begin);
	return n / gq->item_size;
}

size_t gque_get_linear_entries(gque_t *gq)
{
	size_t n = ( gq->head<=gq->tail ) ? (gq->tail-gq->head)
			: (gq->end-gq->head);
	return n / gq->item_size;
}

size_t gque_get_rooms(gque_t *gq)
{
	int n = ( gq->head>gq->tail ) ? (gq->head-gq->tail)
			: (gq->end-gq->tail) + (gq->head-gq->begin);
	return (n / gq->item_size) - 1;
}

size_t gque_get_linear_rooms(gque_t *gq)
{
	size_t n;

	if ( gq->head>gq->tail )
		return (gq->head-gq->tail)/gq->item_size - 1;
	n = (gq->end-gq->tail);
	if ( gq->head == gq->begin ) n -= gq->item_size;
	return (n / gq->item_size);
}

/* #include <stdio.h> */
//gcl_api int gque_request_linear_rooms(gque_t *gq, size_t size, int fix_que)
gcl_api int gque_request_linear_rooms(gque_t *gq, size_t size)
{
	size_t rooms, linear;

	/*
	printf( "---------------GGGGG\n" );
	*/

	linear = gque_get_linear_rooms(gq);
//	if ( !fix_que ) return linear >= size;
	if ( linear >= size ) return 1;
	
	rooms = gque_get_rooms(gq);
	
	/*
	printf( "-- rooms:%d\n", rooms );
	printf( "h:%d t:%d e:%d r:%d\n", gq->head-gq->begin,
			gq->tail-gq->begin,
			gq->end-gq->begin,
			gq->remainder );
	printf( "lin=%d rem=%d size=%d\n", linear, gq->remainder, size );
	*/

	if ( rooms < size ) return 0;

	if ( gq->head == gq->tail )
	{
		gq->head = gq->tail = gq->begin;
		return 1;
	}

	if ( linear+gq->remainder >= size )
	{
		gq->end += gq->remainder * gq->item_size;
		gq->remainder = 0;
		return 1;
	}

	rooms = (gq->head-gq->begin)/gq->item_size - 1;
	if ( rooms < size ) return 0;
	gq->remainder += linear;
	gq->end = gq->tail;
	gq->tail = gq->begin;
	return 1;
}