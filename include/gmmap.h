/*
 *  @packaage OpenGCL
 *
 *  @module MMap - memory mapped file
 */

#pragma once
#include "gcldef.h"
#include <sys/types.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
typedef _off_t off_t;

#else
#include <sys/mman.h>
#endif

namespace gcl
{

//--------------------------------------------------------------------
//	class MMap
//--------------------------------------------------------------------
class gcl_api MMap
{
protected:
    void        *m_hMap = 0; // handle to mmap
    char        *m_ptr = 0;
    size_t      m_size = 0;

public:
    MMap() {}
    ~MMap() { close(); }

    // read only mappings
    const char *mmap_ro(int filedes, off_t offset, size_t size, const char *name = 0)
    {
        return _mmap(filedes, offset, size, true, name);
    }

    // size becomes to whole file length
    const char *mmap_ro(int filedes, off_t offset, const char *name = 0)
    {
        return _mmap(filedes, offset, 0, true, name);
    }

    // offset defaults to zero, size becomes to whole file length
    const char *mmap_ro(int filedes, const char *name = 0)
    {
        return _mmap(filedes, 0, 0, true, name);
    }

    // maps to system virtual memory
    const char *mmap_vro(int size, const char *name = 0)
    {
        return _mmap(-1, 0, size, true, name);
    }

    // read/write mappings
    char *mmap_rw(int filedes, off_t offset, size_t size, const char *name = 0)
    {
        return _mmap(filedes, offset, size, false, name);
    }

    // size becomes to whole file length
    char *mmap_rw(int filedes, off_t offset, const char *name = 0)
    {
        return _mmap(filedes, offset, 0, false, name);
    }

    // offset defaults to zero, size becomes to whole file length
    char *mmap_rw(int filedes, const char *name = 0)
    {
        return _mmap(filedes, 0, 0, false, name);
    }

    // maps to system virtual memory
    char *mmap_vrw(int size, const char *name = 0)
    {
        return _mmap(-1, 0, size, false, name);
    }

    void close();

    //--- operators
    operator char *() { return m_ptr; }
    operator const char *() const { return m_ptr; }
    char &operator[](int index) { return m_ptr[index]; }
    const char &operator[](int index) const { return m_ptr[index]; }

    //--- accessors
    bool is_valid() const { return m_ptr != 0; }
    size_t get_size() const { return m_size; }
    const char *begin() const { return m_ptr; }
    const char *end() const { return m_ptr + m_size; }
    char *begin() { return m_ptr; }
    char *end() { return m_ptr + m_size; }

protected:
    char *_mmap(int filedes, off_t offset, size_t size, bool readonly, const char *name);
};


//--------------------------------------------------------------------
//	class MMapFile
//--------------------------------------------------------------------
class gcl_api MMapFile : public MMap
{
protected:
    int m_fd = -1; // file descriptor

public:
    MMapFile() {}
    ~MMapFile() { close(); }

    //--- manipulators
    // read only mappings
    const char *mmap_ro(const char *filePathName, off_t offset, size_t size, const char *name = 0)
    {
        return _mmap(filePathName, offset, size, true, name);
    }

    const char *mmap_ro(const char *filePathName, off_t offset, const char *name = 0)
    {
        return _mmap(filePathName, offset, 0, true, name);
    }

    const char *mmap_ro(const char *filePathName, const char *name = 0)
    {
        return _mmap(filePathName, 0, 0, true, name);
    }

    // read/write mappings
    char *mmap_rw(const char *filePathName, off_t offset, size_t size, const char *name = 0)
    {
        return _mmap(filePathName, offset, size, false, name);
    }

    char *mmap_rw(const char *filePathName, off_t offset, const char *name = 0)
    {
        return _mmap(filePathName, offset, 0, false, name);
    }

    char *mmap_rw(const char *filePathName, const char *name = 0)
    {
        return _mmap(filePathName, 0, 0, false, name);
    }

    void close();

    //--- accessors
    int get_file_descriptor() const { return m_fd; }
    size_t get_file_length() const;

protected:
    char *_mmap(const char *filePathName, off_t offset, size_t size, bool readonly, const char *name);
};

} // namespace gcl
