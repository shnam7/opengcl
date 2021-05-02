/*
 *  @packaage OpenGCL
 *
 *  @module gutil
 */

#include "gutil.h"

#ifdef _WIN32
#include <io.h>

#else
#include <fcntl.h>
#include <unistd.h>

gcl_api size_t gcl::util::_filelength(int filedes)
{
    off_t pos = lseek(filedes, 0, SEEK_CUR);
    if (pos != (off_t)-1)
    {
        off_t size = lseek(filedes, 0, SEEK_END);
        lseek(filedes, pos, SEEK_SET);
        return (size_t)size;
    }
    return (off_t)-1;
}
#endif
