/*
 *	* gsocket.c
 *
 *	OpenGCL Module : gsocket - BSD Socket extension and C++ wrappers
 *
 *	Written by Soo-Hyuk Nam.
 *		2001. 11. 27. Tue.
 *
 *	History:
 *		2001/11/27: Originally written.
 *
 *	Notes:
 *		- All the functions return 0 on error, non-zero on success
 *		  unless specified otherwise.
 *		- Use gsock_getlasterror() to retrive recent error code.
 *		- Every timeout values are in millisecond unit.
 *		- In NBIO mode, use isWritable() - not isReadable() - to check
 *		  the result of connect().
 *		- isWritable() returns false when peer socket closed, but
 *		  isReadable() does'nt. So, receiving side need to check if
 *		  any data can be actually read when isReadable() returns true,
 *		  by checking the return value(actual read count) of peek() or
 *		  other receving functions.
 */

#include "gsocket.h"
#include <string.h>
#include "gtime.h"

#if !defined(_WIN32)
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netdb.h>
#endif

#if defined( _WIN32 )
#define ioctl			ioctlsocket
#define close			closesocket
#define MSG_DONTWAIT	0
// #define EINPROGRESS		WSAEWOULDBLOCK
#define __err			WSAGetLastError()
// #define EAGAIN			WSAEWOULDBLOCK
#else
#define __err			errno
#endif

// #if defined( __CYGWIN__ )
// #define MSG_DONTWAIT	0
// #endif


socket_t gsock_create_socket(int type, unsigned long nbio)
{
	socket_t sock = socket(AF_INET, type, 0);
	if ( sock == -1 ) return -1;
	if ( nbio && ioctl(sock, FIONBIO, &nbio)!=0 ) return -1;
	return sock;
}

int gsock_close_socket(socket_t sock)
{
	if ( sock == -1 ) return 0;
	return close(sock) == 0;
}

int gsock_shutdown(socket_t sock, int how)
{
	if ( sock == -1 ) return 0;
	return shutdown(sock, how) == 0;
}

int gsock_connect(socket_t sock, ipaddr_t ipAddr, int port)
{
	int r;
	struct sockaddr_in sa;

	if ( sock == -1 ) return 0;
	memset ( &sa, '\0', sizeof(sa) );
	sa.sin_family      = AF_INET;
	sa.sin_addr.s_addr = ipAddr;				/* server address */
	sa.sin_port        = htons( (short)port );	/* server port number */
	r = connect( sock, (struct sockaddr *)&sa, sizeof(sa) );
	return ( r >= 0 || __err==EINPROGRESS );
}

int gsock_connect_by_name(socket_t sock, const char *addr, int port)
{
	struct hostent *host;
	ipaddr_t ipAddr = inet_addr(addr);

	if ( ipAddr != INADDR_NONE )		/* hostName is in xxx.xxx.xxx.xxx format */
		return gsock_connect( sock, ipAddr, port );
	host = gethostbyname( addr );
	if ( !host ) return gsock_connect( sock, 0, 0 );
				/* this is necessary because check_writable() returns true(readable)
				   for newly created socket which did not call connect().
				   Once connect() is called, check_writable() returns correct answer. */

	/* hostName is in "domainname.net" format */
	return gsock_connect( sock, *(u_long *)host->h_addr, port );
}

int gsock_listen(socket_t sock, ipaddr_t ipAddr, int port, int backlog)
{
	struct sockaddr_in sa_serv;

	if ( sock == -1 ) return 0;
	memset ( &sa_serv, '\0', sizeof(sa_serv) );
	sa_serv.sin_family      = AF_INET;
	sa_serv.sin_addr.s_addr = ipAddr;				// server address
	sa_serv.sin_port        = htons( (short)port );	// server port number

	if ( bind(sock, (struct sockaddr*)&sa_serv, sizeof(sa_serv)) != 0
		|| listen(sock, backlog) != 0 )
	{
		return 0;
	}
	return 1;
}

int gsock_read(socket_t sock, char *buf, int n, unsigned long timeout)
{
	int count;
	gtime_t tm1, elapsed;

	count = recv( sock, buf, n, 0 );
	if ( count == -1 )
	{
		if ( __err != EAGAIN ) return -1;
		count = 0;
	}
	if ( count == n || timeout==0 ) return count;
	tm1 = gtime_get_ticks();
	for ( ;; )
	{
		int rc = recv( sock, buf+count, n-count, 0 );
		if ( rc != -1 )
			count += rc;
		else
		{
			if ( __err != EAGAIN ) return -1;
		}
		if ( count >= n ) break;
		elapsed = gtime_get_elapsed( tm1 );
		if ( elapsed > timeout ) break;
		if ( !gsock_is_readable(sock, timeout-elapsed) ) break;
	}
	return count;
}

int gsock_write(socket_t sock, const char *buf, int n, unsigned long timeout)
{
	int count;
	gtime_t tm1, elapsed;

	count = send( sock, buf, n, MSG_DONTWAIT );
	if ( count == -1 )
	{
		if ( __err != EAGAIN ) return -1;
		count = 0;
	}
	if ( count == n || timeout==0 ) return count;
	tm1 = gtime_get_ticks();
	for ( ;; )
	{
		int wc = send( sock, buf+count, n-count, MSG_DONTWAIT );
		if ( wc != -1 )
			count += wc;
		else
		{
			if ( __err != EAGAIN ) return -1;
		}
		if ( count >= n ) break;
		elapsed = gtime_get_elapsed(tm1);
		if ( elapsed > timeout ) break;
		if ( !gsock_is_writable(sock, timeout-elapsed) ) break;
	}
	return count;
}

int gsock_set_NBIO(socket_t sock, int nbio)
{
	unsigned long val = nbio;
	return ioctl( sock, FIONBIO, &val ) == 0;

	/*
	int flags = fcntl( sock, F_GETFL, 0 );
	if ( flags < 0 ) return 0;
	flags = ( nbio ) ? (flags|O_NONBLOCK) : (flags&~O_NONBLOCK);
	flags |= O_NONBLOCK;
	return fcntl(sock, F_SETFL, flags) >= 0;
	*/

}

int gsock_get_receive_buffer_size(socket_t sock)
{
	int length;
	socklen_t size = sizeof(int);
	return ( getsockopt(sock, SOL_SOCKET, SO_RCVBUF,
				(char *)&length, &size) ==0 ) ? length : 0;
}

int gsock_get_send_buffer_size(socket_t sock)
{
	int length;
	socklen_t size = sizeof(int);
	return ( getsockopt(sock, SOL_SOCKET, SO_SNDBUF,
				(char *)&length, &size) ==0 ) ? length : 0;
}

int gsock_get_readable_count(socket_t sock)
{
#if defined( _WIN32 )
	unsigned long val;
#else
	int val;
#endif

	if ( ioctl(sock, FIONREAD, &val) != 0 ) return -1;
	return val;
}

gcl_api int gsock_check_readable(socket_t sock, unsigned long timeout)
{
	int r;
	fd_set fdsRead;
	fd_set fdsExcept;
	struct timeval tv = { timeout/1000, (timeout%1000)*1000 };

	FD_ZERO( &fdsRead );
	FD_ZERO( &fdsExcept );
	FD_SET( sock, &fdsRead );
	FD_SET( sock, &fdsExcept );
	r = select( (int)sock+1, &fdsRead, NULL, &fdsExcept, &tv );
	if ( r<0 || FD_ISSET(sock, &fdsExcept) || gsock_get_error(sock)!=0)
		return -1;		// error
	return FD_ISSET(sock, &fdsRead);
}

gcl_api int gsock_check_writable(socket_t sock, unsigned long timeout)
{
	int r;
	fd_set fdsWrite;
	fd_set fdsExcept;
	struct timeval tv = { timeout/1000, (timeout%1000)*1000 };

	FD_ZERO( &fdsWrite );
	FD_ZERO( &fdsExcept );
	FD_SET( sock, &fdsWrite );
	FD_SET( sock, &fdsExcept );
	r = select( (int)sock+1, NULL, &fdsWrite, &fdsExcept, &tv );
	if ( r<0 || FD_ISSET(sock, &fdsExcept) ) return -1;		// error
	return ( FD_ISSET(sock, &fdsWrite) && gsock_get_error(sock)==0 );
}

ipaddr_t gsock_hostaddr(const char *hostname)
{
	struct hostent *host;
	ipaddr_t ipAddr = inet_addr(hostname);

	if ( ipAddr != INADDR_NONE ) return ipAddr;
	host = gethostbyname( hostname );
	if ( !host ) return INADDR_NONE;
	return (ipaddr_t)( *(u_long *)host->h_addr );
}
