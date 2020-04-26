//
//		* exsocket.cpp
//
//		OpenGCL Example: gsocket operations
//
//		Written by Soo-Hyuk Nam.
//			2002. 6. 4. Tue.
//

#include "gsocket.h"    // socket should be included before window.h
#include "gthread.h"
#include <stdio.h>
#include <string.h>

#if !defined( _WIN32 )
#include <arpa/inet.h>
#include <unistd.h>
#endif

#define EX_PORT		5100

static void *_serv_d(void *data)
{
	GThread *pT = GThread::getCurrent();
	GSocket gs( *(socket_t *)data );

	if ( !gs.isValid() ) { delete pT; return 0; }
	gs.setNBIO( true );

	int count = 0;
	for ( ;; )
	{
		char buf[1024];
		if ( !gs.isWritable(1000) )
		{
			printf( "Socket is not writable.\n" );
			break;
		}

        sprintf(buf, "%d Hello!\n", count++);
        int slen = (int)strlen( buf );
		if ( gs.write(buf, slen, 1000) != slen ) break;
//		if ( gs.send(buf, slen) != slen ) break;
//		if ( ::send( gs.getSocket(), buf, slen, MSG_DONTWAIT )!=slen) break;
		printf( "send=<%s>", buf  );
	}
	gs.closeSocket();
	printf( "connection closed\n" );
	delete pT;
	return 0;
}

void _server()
{
	GSocket gsListener;

	printf( "Starting server...\n" );
	int r = gsListener.createSocket( true );
	if ( !r )
	{
		printf( "socket creation error.\n" );
		return;
	}
	r = gsListener.listen( INADDR_ANY, EX_PORT );
	if ( !r )
	{
		printf( "socket listen error: sock=%d.\n", (int)gsListener.getSocket() );
		return;
	}
	printf( "listen ready!\n" );

	while ( 1 )
	{
		int r = gsListener.checkReadable( 10 );
		if ( r < 0 ) break;		// error
		if ( r== 0 ) continue;
		sockaddr_in sa;
		socket_t sock = gsListener.accept( &sa );
		printf( "r=%d sock=%d\n", r, (int)sock );
		if ( sock == INVALID_SOCKET ) continue;
		printf( "client from %s accepted.\n", inet_ntoa(sa.sin_addr) );

		GThread *pT = new GThread();
		pT->start( _serv_d, (void *)&sock );
/*		pid_t pid = fork();
		if ( pid == 0 )	// child process
		{
			gcl::socket gs(sock);
			if ( !gs.isValid() ) return;
			if ( !gs.setNBIO( true ) ) printf("error at \n");
			char buf[1024];
			for (int i=0; true; ++i)
			{
				if ( !gs.isWritable(1000) )
				{
					printf( "Socket is not writable.\n" );
					break;
				}
				sprintf( buf, "%d Hello!\n", i );
				int slen = strlen( buf );
				if ( gs.write(buf, slen, 1000) != slen ) break;
//				if ( gs.send(buf, slen) != slen ) break;
//				if ( ::send( gs.getSocket(), buf, slen, MSG_DONTWAIT )!=slen) break;
				printf( "send=%d\n", i );
				//sleep( 1 );
			}
			printf( "connection closed\n" );
			gs.closeSocket();
			_exit(0);
		}
		*/
	}
}


void _client()
{
	GSocket gs;

	printf( "Starting client...\n" );

	//--- create socket
	gs.createSocket( true );
	//gs.connect( inet_addr("127.0.0.1"), EX_PORT );
	// gs.connect( "127.0.0.1", EX_PORT );
	// gs.connect( "surf", EX_PORT );
	gs.connect( "localhost", EX_PORT );

	//--- connect to server
	if ( !gs.isWritable(1000) )
	{
		printf( "Connection failed.\n" );
		return;
	}
	printf( "Socket ID=%d\n", (int)gs.getSocket() );


	//--- read/write
	while ( 1 )
	{
		char buf[1024];
		if ( !gs.isReadable(1000) )
		{
			printf( "socket readable timeout 1 sec\n" );
			break;
		}
		int rc = gs.read( buf, sizeof(buf)-1 );
		if ( rc <= 0 )	// --> zero read count means error because isReadable() is true.
		{
			printf( "read error.\n" );
			break;
		}
		buf[rc] = '\0';
		printf( ">>%s", buf );
	}
}

int main(int argc, char *argv[])
{
	/*
	ipaddr_t addr = gsock_hostaddr("empas.com");
	printf( "addr=%lX\n", addr );
	return 0;
	*/

#if defined( _WIN32 )
	WSADATA wsaData;
	if ( WSAStartup(0x0101, &wsaData) != 0 )
	{
		printf( "WSA init failed. exit.\n" );
		return -1;
	};
#endif

	printf( "exsocket test...\n" );
	if ( argc <= 1 )
		_server();
	else
		_client();

#if defined( _WIN32 )
	WSACleanup();
#endif
	return 0;
}
