/*
 *	* gmmap.cpp
 *
 *	OpenGCL Module : Generic Memory Mapped IO
 *
 *	Written by Soo-Hyuk Nam.
 *		2003. 7. 11. Fri.
 *
 *	History:
 *		2003/07/11: ported to unix(Linux)
 *		2002/07/24: First written for windows.
 *
 *	Notes:
 *		- All the functions returns 0 on success, nonzero code on error,
 *		  unless specified otherwise.
 */

#include "gmmap.h"
#include <fcntl.h>
#include <stdio.h>

#if defined( _WIN32 )
#include <io.h>
#else
#include <unistd.h>
#endif

#if defined( _WIN32 )
#define open		_open
#define O_RDONLY	_O_RDONLY
#define O_RDWR		_O_RDWR
#else
static size_t _filelength(int filedes)
{
	off_t pos = lseek( filedes, 0, SEEK_CUR );
	if ( pos != (off_t)-1 )
	{
		off_t size = lseek( filedes, 0, SEEK_END );
		lseek( filedes, pos, SEEK_SET );
		return (size_t)size;
	}
	return (off_t)-1;
}
#endif

//--------------------------------------------------------------------
//	class gcl::mmap unix impl.
//--------------------------------------------------------------------
gcl_api void gcl::gmmap::close()
{
	if ( !m_ptr ) return;
#if !defined( _WIN32 )
	if ( m_hMap ) munmap( m_hMap, m_size );
#else
	if ( m_hMap )
	{
		UnmapViewOfFile( m_ptr );
		CloseHandle( m_hMap );
	}
#endif
	m_size = 0;
	m_ptr = 0;
}

gcl_api char *gcl::gmmap::_mmap(int filedes, off_t offset, size_t size,
		bool readonly, const char *name)
{
	if ( isValid() ) close();
#if !defined( _WIN32 )
	int flags = MAP_SHARED;
	int prot = readonly ? PROT_READ : (PROT_READ|PROT_WRITE);
	if ( filedes == -1 ) flags |= MAP_ANONYMOUS;
	if ( offset == 0 )
	{
		m_size = size==0 ? _filelength(filedes) : size;
		m_ptr = (char *)::mmap( 0, m_size, prot, flags, filedes, 0 );
		m_hMap = m_ptr;
	}
	else
	{
		int page_size = getpagesize();
		int pos_base = offset / page_size;
		if ( pos_base ) pos_base *= page_size;
		int pos_offs = offset % page_size;
		m_size = size==0 ? _filelength(filedes)-offset : size;
		m_ptr = (char *)::mmap( 0, m_size, prot, flags, filedes, pos_base );
		m_hMap = m_ptr;
		if ( m_ptr ) m_ptr += pos_offs;
	}
	if ( m_ptr == MAP_FAILED )
	{
		m_size = 0;
		m_hMap = m_ptr = 0;
		return 0;
	}
#else
	HANDLE hFile = (filedes == -1) ? INVALID_HANDLE_VALUE : (HANDLE)_get_osfhandle( filedes );
	if ( offset == 0 )
	{
		m_hMap = CreateFileMapping( hFile, NULL, readonly ? PAGE_READONLY
				: PAGE_READWRITE, 0, (DWORD)size, name );   // size is DWORD in windows
		if ( m_hMap==NULL ) return 0;
		m_size = size==0 ? ::GetFileSize(hFile, 0) : size;
		m_ptr = (char *)MapViewOfFile( m_hMap, readonly ? FILE_MAP_READ
				: FILE_MAP_ALL_ACCESS, 0, 0, m_size );
	}
	else
	{
		SYSTEM_INFO si;
		GetSystemInfo( &si );
		int pos_base = (offset/si.dwAllocationGranularity);
		if ( pos_base ) pos_base *= si.dwAllocationGranularity;
		int pos_offs = offset % si.dwAllocationGranularity;
		m_hMap = CreateFileMapping( hFile, NULL, readonly ? PAGE_READONLY
				: PAGE_READWRITE, 0, size==0 ? 0 : offset+(DWORD)size, name );
		if ( m_hMap==NULL ) return 0;
		m_size = size==0 ? ::GetFileSize(hFile, 0)-offset : size;
		m_ptr = (char *)MapViewOfFile( m_hMap, readonly ? FILE_MAP_READ
				: FILE_MAP_ALL_ACCESS, 0, pos_base, pos_offs+m_size );
		if ( m_ptr ) m_ptr += pos_offs;
	}
	if ( !m_ptr )
	{
		CloseHandle( m_hMap );
		m_size = 0;
		return 0;
	}
#endif
	return m_ptr;
}


//--------------------------------------------------------------------
//	class gcl::mmapfile
//--------------------------------------------------------------------
gcl_api void gcl::gmmap_file::close()
{
	gcl::gmmap::close();
	if ( m_fd != -1 )
	{
#if defined ( _MSC_VER )
        ::_close(m_fd);
#else
        ::close( m_fd );
#endif
		m_fd = -1;
	}
}

gcl_api size_t gcl::gmmap_file::getFileLength() const
{
	return m_fd==-1 ? 0 : _filelength( m_fd );
}

gcl_api char *gcl::gmmap_file::_mmap(const char *filePathName, off_t offset, size_t size,
		bool bReadOnly, const char *name)
{
	if ( isValid() ) close();
	m_fd = open( filePathName, bReadOnly ? O_RDONLY : O_RDWR );
	if ( m_fd == -1 ) return 0;
	return gcl::gmmap::_mmap( m_fd, offset, size, bReadOnly, name );
}
