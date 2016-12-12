#ifndef IMPL_FILE_H
#define IMPL_FILE_H

#include "interface/iio.h"

namespace impl {
struct fileio
{
    FILE *file_fd;
#if bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux
    struct stat file_stat;
#else
    struct _stat64i32 file_stat;
#endif
    static fileio *defio;

    virtual ~fileio(){}
    virtual bool create (const bs_string &file, int  mode) = 0;
    virtual bool remove (const bs_string &file) = 0;
    virtual bool rename (const bs_string &oldfile, const bs_string& newfile) = 0;

    virtual bool open (const bs_string &file, int mode) = 0;
    virtual bool close () = 0;
    virtual bool access (const bs_string &file) = 0;
    virtual bool stat (const bs_string &file) = 0;
    virtual int64 tell () = 0;
    virtual int64 read (void *vbuf, int64 len) = 0;
    virtual int64 write (void *vbuf, int64 len) = 0;
    virtual int64 seek (int64 size, int flag) = 0;
    virtual bool eof () = 0;

    char* return_mode(int mode)
    {
        static char retb[4] = {'\0'};

        if (mode & Io_WriteOnly)
            retb[0] = mode & Io_Append ? 'a' : 'w';
        else if (mode & Io_ReadWrite)
            retb[0] = mode & Io_Append ? 'a' : 'w';
        else
            retb[0] = 'r';

        if (mode & Io_Text)
            retb[1] = 't';
        else
            retb[1] = 'b';

        if (mode & Io_ReadWrite)
            retb[3] = '+';

        return retb;
    }
};
}
class impl_file : public iio
{
    friend class bs_filesystem;
    friend class bs_fileinfo;
public:
    impl_file();
    ~impl_file();

    virtual bs_string errors()const;
    virtual bool open(int mode = Io_ReadWrite);
    virtual void close();

    virtual int64 tell()const;
    virtual bool seek(int64 pos);
    virtual int64 read(void *buf, int64 len)const;
    virtual int64 write(void *buf, int64 len)const;
    virtual bool at_end()const;
private:
    impl::fileio *impl;
};

#endif // IMPL_FILE_H
