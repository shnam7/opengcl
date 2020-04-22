/*
 *	* gsocket_x.c
 *
 *	OpenGCL Module : gsocket - gsocket C++ Wrapper
 *
 *	Written by Soo-Hyuk Nam.
 *		2003. 03. 04. Tue.
 *
 *	History:
 *		2003/03/04: Originally written.
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
#if !defined( _WIN32 )
#include <unistd.h>
#endif

#if defined( _WIN32 )
#define ioctl			ioctlsocket
#define close			closesocket
#endif

//--------------------------------------------------------------------
//	class GSocket
//--------------------------------------------------------------------
bool GSocket::createSocket(int type, unsigned long nbio)
{
    if (m_sock != (socket_t)-1) close(m_sock);
    m_sock = socket(AF_INET, type, 0);
    if (m_sock == (socket_t)-1) return 0;
    return (nbio) ? ioctl(m_sock, FIONBIO, &nbio) == 0 : true;
}

bool GSocket::closeSocket()
{
	if ( m_sock == (socket_t)-1 ) return false;
	if ( close(m_sock) != 0 ) return false;
	m_sock = -1;
	return true;
}

bool GSocket::attach(socket_t sock)
{
	if ( sock == (socket_t)-1 ) return false;
	if ( m_sock == sock ) return true;
	if ( m_sock != (socket_t)-1) close( m_sock );
	m_sock = sock;
	return true;
}

bool GSocket::detach()
{
	m_sock = -1;
	return true;
}
