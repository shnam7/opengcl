/*
 *  @package OpenGCL
 *
 *  @module socket - BSD Socket extension and C++ wrappers
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
 *		  isReadable() doesn't. So, receiving side need to check if
 *		  any data can be actually read when isReadable() returns true,
 *		  by checking the return value(actual read count) of peek() or
 *		  other receving functions.
 */

#pragma once
#include "gcldef.h"
#include "gtime.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#endif

using gcl::msec_t;

#define	GSOCK_TIMEOUT	5000	/* msec */

#if defined(_WIN32)
typedef int		socklen_t;
typedef SOCKET	socket_t;
#define SHUT_RD		SD_RECEIVE
#define SHUT_WR		SD_SEND
#define SHUT_RDWR	SD_BOTH

#else
typedef uintptr_t   socket_t;
#define INVALID_SOCKET  ((uintptr_t)-1)
#endif

typedef u_long		ipaddr_t;


/*--- V protocol */
#ifdef _WIN32
#pragma pack(1)
typedef struct _vpacket_t {
	unsigned char	type;
	unsigned char	code;
	unsigned short	dlen;
/*	char			data[0]; */
} vpacket_t;
#pragma pack()
#else
typedef struct _vpacket_t {
	unsigned char	type;
	unsigned char	code;
	unsigned short	dlen;
/*	char			data[0]; */
} __attribute__((packed)) vpacket_t;
#endif

enum enumVProtocolConstants {
	/* predefined vpacket type */
	VPT_REQUEST			= 1,
	VPT_REPLY			= 2,
	VPT_USER			= 32,

	/* predefined request codes */
	VPR_CONNECT			= 1,
	VPR_DISCONNECT		= 2,
	VPR_CHECK			= 3,	/* simple link test */
	VPR_USER			= 32,

	/* basic error code */
	VPE_SUCCESS			= 0,
	VPE_PROTOCOL		= 1,	/* protocol error */
	VPE_UNKNOWNREQUEST	= 2,
	VPE_FAULT			= 3,	/* invalid parameters/data */
	VPE_FAIL			= 4,	/* request failed */
	VPE_INTERNAL		= 5,	/* internal implementation error */
	VPE_PVERSION		= 6,	/* protocol version error */
	VPE_USER			= 32,	/* user defined error: 128-255 */

	VNET_TIMEOUT	= GSOCK_TIMEOUT,	/* default network timeout value (msec) */
};


socket_t gsock_create_socket(int type, unsigned long nbio);

gcl_api int gsock_close_socket(socket_t sock);

gcl_api int gsock_shutdown(socket_t sock, int how);

gcl_api int gsock_connect(socket_t sock, ipaddr_t ipAddr, int port);

gcl_api int gsock_connect_by_name(socket_t sock, const char *addr, int port);

gcl_api int gsock_listen(socket_t sock, ipaddr_t ipAddr, int port, int backlog);

inline socket_t gsock_accept(socket_t sock, struct sockaddr *addr, socklen_t *addrLen)
	{ return accept(sock, addr, addrLen); }

inline socket_t gsock_accept2(socket_t sock, struct sockaddr_in *addr)
	{ socklen_t salen=sizeof(struct sockaddr_in);
	   	return accept(sock, (struct sockaddr *)addr, &salen); }

inline socket_t gsock_accept1(socket_t sock)
	{ struct sockaddr_in sa; socklen_t salen=sizeof(sa);
	   	return accept(sock, (struct sockaddr *)&sa, &salen); }

inline int gsock_peek(socket_t sock, char *buf, int len)
	{ return recv(sock, buf, len, MSG_PEEK); }

inline int gsock_recv(socket_t sock, char *buf, int len)
	{ return recv(sock, buf, len, 0); }

inline int gsock_send(socket_t sock, const char *buf, int len)
	{ return send(sock, buf, len, 0); }

gcl_api int gsock_read(socket_t sock, char *buf, int n,
		msec_t timeout);

gcl_api int gsock_write(socket_t sock, const char *buf,
		int n, msec_t timeout);

inline int gsock_readn(socket_t sock, void *buf, int n,
		msec_t timeout)
	{ return gsock_read(sock, (char *)buf, n, timeout)==n; }

inline int gsock_writen(socket_t sock, const void *buf, int n,
		msec_t timeout)
	{ return gsock_write(sock, (const char *)buf, n, timeout)==n; }

#if defined( _WIN32 )
inline int gsock_getlasterror() { return WSAGetLastError(); }
#else
#include <errno.h>
inline int gsock_getlasterror() { return errno; }
#endif


/*
 *	Attributes
 */
gcl_api int gsock_set_NBIO(socket_t sock, int nbio);

inline int gsock_set_receive_buffer_size(socket_t sock, int size)
	{ return setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *)&size, sizeof(int))==0; }

inline int gsock_set_send_buffer_size(socket_t sock, int size)
	{ return setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&size, sizeof(int))==0; }

inline int gsock_get_socket_name(socket_t sock, struct sockaddr *name, socklen_t *namelen)
	{ return getsockname(sock, name, namelen)==0; }

inline int gsock_get_socket_name2(socket_t sock, struct sockaddr_in *name)
	{ socklen_t namelen=sizeof(struct sockaddr_in);
	  	return gsock_get_socket_name(sock, (struct sockaddr *)name, &namelen); }

inline int gsock_get_peer_name(socket_t sock, struct sockaddr *name, socklen_t *namelen)
	{ return getpeername(sock, name, namelen)==0; }

inline int gsock_get_peer_name2(socket_t sock, struct sockaddr_in *name)
	{ socklen_t namelen=sizeof(struct sockaddr_in);
	  	return gsock_get_peer_name(sock, (struct sockaddr *)name, &namelen); }

gcl_api int gsock_get_receive_buffer_size(socket_t sock);

gcl_api int gsock_get_send_buffer_size(socket_t sock);

gcl_api int gsock_get_readable_count(socket_t sock);

inline int gsock_get_readable_data_count(socket_t sock)
	{ return gsock_get_readable_count(sock); }

inline int gsock_get_error(socket_t sock)
	{ int err; socklen_t size=sizeof(err);
		getsockopt( sock, SOL_SOCKET, SO_ERROR, (char *)&err, &size );
		return err; }

	/* returns -1 on error, positive when readable, 0 otherwise. */
gcl_api int gsock_check_readable(socket_t sock, msec_t timeout);

	/* returns -1 on error, positive when readable, 0 otherwise. */
gcl_api int gsock_check_writable(socket_t sock, msec_t timeout);

inline int gsock_is_readable(socket_t sock, msec_t timeout)
	{ return gsock_check_readable(sock, timeout)>0; }

inline int gsock_is_writable(socket_t sock, msec_t timeout)
	{ return gsock_check_writable(sock, timeout)>0; }

inline int gsock_setopt_reuse_addr(socket_t sock, int reuse)
	{ return setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,
			(char *)&reuse, sizeof(int)) == 0; }

/* V protocol support */
inline int gsock_readvp(socket_t sock, vpacket_t *vp, msec_t timeout)
	{ return gsock_readn(sock, vp, sizeof(vpacket_t), timeout); }

inline int gsock_writevp(socket_t sock, const vpacket_t *vp, msec_t timeout)
	{ return gsock_writen(sock, vp, sizeof(vpacket_t), timeout); }

inline int gsock_writevpn(socket_t sock, const vpacket_t *vp, int len, msec_t timeout)
	{ return gsock_writen(sock, vp, len, timeout); }

inline int gsock_simple_reply(socket_t sock, unsigned int code, msec_t timeout)
{
	vpacket_t vp = { VPT_REPLY, (unsigned char)code, 0 };
	return gsock_writevp(sock, &vp, timeout);
}

/*
 * etc
 */

gcl_api ipaddr_t gsock_hostaddr(const char *hostname);


namespace gcl {

//--------------------------------------------------------------------
//	class Socket
//--------------------------------------------------------------------
class gcl_api Socket {
protected:
	socket_t		m_sock;

private:
	Socket(const Socket& sock);		// copy constructor is not allowed.
	Socket operator=(socket_t sock);	// assignment is not allowed

public:
	enum { DEF_BACKLOG=5 };

public:
	//--- creators
	Socket() : m_sock(INVALID_SOCKET) {}
	Socket(socket_t sock) : m_sock(sock) {}
	~Socket() { close_socket(); }

	//--- manipulations
	bool create_socket(int type, unsigned long nbio);
	bool create_socket(unsigned long nbio)
		{ return create_socket(SOCK_STREAM, nbio); }
	bool close_socket();
	bool shutdown(int how=SHUT_RDWR)
		{ return gsock_shutdown(m_sock, how) != 0; }
	bool attach(socket_t sock);
	bool detach();

	bool connect(ipaddr_t ipAddr, int port) const
		{ return gsock_connect(m_sock, ipAddr, port) != 0; }
	bool connect(const char *addr, int port) const
		{ return gsock_connect_by_name(m_sock, addr, port) != 0; }
	bool listen(ipaddr_t ipAddr, int port, int backlog=DEF_BACKLOG) const
		{ return gsock_listen(m_sock, ipAddr, port, backlog) != 0; }
	socket_t accept(sockaddr *addr, socklen_t *addrLen) const
		{ return gsock_accept(m_sock, addr, addrLen); }
	socket_t accept(sockaddr_in *addr) const
		{ return gsock_accept2(m_sock, addr); }
	socket_t accept() const
		{ return gsock_accept1(m_sock); }
	int peek(char *buf, int len) const
		{ return gsock_peek(m_sock, buf, len); }
	int recv(char *buf, int len) const
		{ return gsock_recv(m_sock, buf, len); }
	int send(const char *buf, int len) const
		{ return gsock_send(m_sock, buf, len); }
	int read(char *buf, int n, msec_t timeout=0) const
		{ return gsock_read(m_sock, buf, n, timeout); }
	int write(const char *buf, int n, msec_t timeout=0) const
		{ return gsock_write(m_sock, buf, n, timeout); }
	bool readn(void *buf, int n, msec_t timeout=5000) const
		{ return gsock_readn(m_sock, buf, n, timeout) != 0; }
	bool writen(const void *buf, int n, msec_t timeout=5000) const
		{ return gsock_writen(m_sock, buf, n, timeout) != 0; }

	//--- V protocol support
	bool readvp(vpacket_t *vp, msec_t timeout=VNET_TIMEOUT) const
		{ return gsock_readvp(m_sock, vp, timeout) != 0; }
	bool writevp(const vpacket_t *vp, msec_t timeout=VNET_TIMEOUT) const
		{ return gsock_writevp(m_sock, vp, timeout) != 0; }
	bool writevp(const vpacket_t *vp, int len,
			msec_t timeout=VNET_TIMEOUT) const
		{ return gsock_writevpn(m_sock, vp, len, timeout) != 0; }
	bool simple_reply(unsigned int code, msec_t timeout=VNET_TIMEOUT)
	{
		return gsock_simple_reply(m_sock, code, timeout) != 0;
	}

	//--- attributes
	bool set_NBIO(bool nbio) const
		{ return gsock_set_NBIO(m_sock, nbio) != 0; }
	int set_receive_buffer_size(int size) const
		{ return gsock_set_receive_buffer_size(m_sock, size); }
	int set_send_buffer_size(int size) const
		{ return gsock_set_send_buffer_size(m_sock, size); }

	bool get_socket_name(sockaddr *name, socklen_t *namelen) const
		{ return gsock_get_socket_name(m_sock, name, namelen) != 0; }
	bool get_socket_name(sockaddr_in *name) const
		{ return gsock_get_socket_name2(m_sock, name) != 0; }
	bool get_peer_name(sockaddr *name, socklen_t *namelen) const
		{ return gsock_get_peer_name(m_sock, name, namelen) != 0; }
	bool get_peer_name(sockaddr_in *name) const
		{ return gsock_get_peer_name2(m_sock, name) != 0; }

	int get_receive_buffer_size() const
		{ return gsock_get_receive_buffer_size(m_sock); }
	int get_send_buffer_size() const
		{ return gsock_get_send_buffer_size(m_sock); }
	int get_readable_count() const
		{ return gsock_get_readable_count(m_sock); }

	int get_error() const { return gsock_get_error(m_sock); }
	int check_readable(msec_t timeout=0) const
		{ return gsock_check_readable(m_sock, timeout); }
	int check_writable(msec_t timeout=0) const
		{ return gsock_check_writable(m_sock, timeout); }
	bool is_readable(msec_t timeout=0) const
   		{ return gsock_is_readable(m_sock, timeout) != 0; }
	bool is_writable(msec_t timeout=0) const
   		{ return gsock_is_writable(m_sock, timeout) != 0; }

	bool setopt_reuse_addr(bool reuse)
		{ return gsock_setopt_reuse_addr(m_sock, (int)reuse) != 0; }

	bool is_valid() const { return m_sock != INVALID_SOCKET; }

	socket_t get_socket() const { return m_sock; }

	//--- operators
	operator socket_t() const { return m_sock; }
};

} // namespace gcl
