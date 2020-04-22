/*
 *	* gmmap.h
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

#pragma once

#include <sys/types.h>
#if defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/mman.h>
#endif

#if defined( _WIN32 ) && defined(_DLL)
#ifdef GCL_EXPORTS
#define gcl_api		__declspec(dllexport)
#else
#define gcl_api		__declspec(dllimport)
#endif
#else
#define gcl_api
#endif

#if defined( _WIN32 )
typedef _off_t		off_t;
#endif

//--------------------------------------------------------------------
//	class GMMap
//--------------------------------------------------------------------
class gcl_api GMMap {
protected:
	char				*m_ptr;
	size_t		        m_size;
	void                *m_hMap;

public:
	GMMap();
	~GMMap();

	//--- manipulators

	// read only mappings
	const char *mmap_ro(int filedes, off_t offset, size_t size, const char *name=0)
		{ return _mmap(filedes, offset, size, true, name); }

		// size becomes to whole file length
	const char *mmap_ro(int filedes, off_t offset, const char *name=0)
		{ return _mmap(filedes, offset, 0, true, name); }	// size becomes to file length

		// offset defaults to zero, size becomes to whole file length
	const char *mmap_ro(int filedes, const char *name=0)
		{ return _mmap(filedes, 0, 0, true, name); }		// size becomes to file length

		// maps to system virtual memory
	const char *mmap_vro(int size, const char *name=0)
		{ return _mmap(-1, 0, size, true, name); }

	// read/write mappings
	char *mmap_rw(int filedes, off_t offset, size_t size, const char *name=0)
		{ return _mmap(filedes, offset, size, false, name); }

		// size becomes to whole file length
	char *mmap_rw(int filedes, off_t offset, const char *name=0)
		{ return _mmap(filedes, offset, 0, false, name); }

		// offset defaults to zero, size becomes to whole file length
	char *mmap_rw(int filedes, const char *name=0)
		{ return _mmap(filedes, 0, 0, false, name); }	// size becomes to file length

		// maps to system virtual memory
	char *mmap_vrw(int size, const char *name=0)
		{ return _mmap(-1, 0, size, false, name); }

	void close();


	//--- operators
	operator char *() { return m_ptr; }
	operator const char *() const { return m_ptr; }
	char& operator [](int index) { return m_ptr[index]; }
	const char& operator [](int index) const { return m_ptr[index]; }

	//--- accessors
	bool isValid() const { return m_ptr!=0; }
	size_t getSize() const { return m_size; }
	const char *begin() const { return m_ptr; }
	const char *end() const { return m_ptr+m_size; }
	char *begin() { return m_ptr; }
	char *end() { return m_ptr+m_size; }

protected:
	char *_mmap(int filedes, off_t offset, size_t size, bool readonly,
			const char *name);
};


//--------------------------------------------------------------------
//	class GMMapFile
//--------------------------------------------------------------------
class gcl_api GMMapFile : public GMMap {
protected:
	int			m_fd;		// file descriptor

public:
	GMMapFile();
	~GMMapFile();

	//--- manipulators
	// read only mappings
	const char *mmap_ro(const char *filePathName, off_t offset,
			size_t size, const char *name=0)
		{ return _mmap(filePathName, offset, size, true, name ); }

	const char *mmap_ro(const char *filePathName, off_t offset,
			const char *name=0)
		{ return _mmap(filePathName, offset, 0, true, name ); }

	const char *mmap_ro(const char *filePathName, const char *name=0)
		{ return _mmap(filePathName, 0, 0, true, name); }

	// read/write mappings
	char *mmap_rw(const char *filePathName, off_t offset,
			size_t size, const char *name=0)
		{ return _mmap(filePathName, offset, size, false, name ); }

	char *mmap_rw(const char *filePathName, off_t offset, const char *name=0)
		{ return _mmap(filePathName, offset, 0, false, name ); }

	char *mmap_rw(const char *filePathName, const char *name=0)
		{ return _mmap(filePathName, 0, 0, false, name); }

	void close();

	//--- accessors
	int getFileDescriptor() const { return m_fd; }
	size_t getFileLength() const;

protected:
	char *_mmap(const char *filePathName, off_t offset,
			size_t size, bool readonly, const char *name);
};
