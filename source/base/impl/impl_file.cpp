#include "impl_file.h"
#include "bs_filesystem.h"

#if bsCompiler == bsCompiler_GNUC || bsCompiler == bsCompiler_CLANG
#if bsPlatform == bsPlatform_Win32
#include <io.h>
#else
#include<unistd.h>
#endif

#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#endif

namespace impl {

#if bsPlatform == bsPlatform_Win32
struct win32io : fileio
{
    win32io()
    {
        file_fd = NULL;
    }
    virtual bool create (const bs_string &path, int  mode )
    {
        int fil_fd = ::_wcreat(path.to_wchar(), mode);
        if (fil_fd != -1)
            return ::close(fil_fd) == 0;
        return false;
    }
    virtual bool remove (const bs_string &path)
    {
        return ::_wunlink (path.to_wchar()) == 0 ;
    }
    virtual bool rename (const bs_string &path_s, const bs_string& path_d)
    {
        return ::_wrename (path_s.to_wchar(), path_d.to_wchar()) == 0 ;
    }

    virtual bool open (const bs_string &file, int mode)
    {
        file_fd = ::_wfopen(file.to_wchar (), return_mode (mode));
        return file_fd != NULL;
    }
    virtual bool close ()
    {
        return ::fclose(file_fd) == 0;
    }
    virtual bool access (const bs_string &path)
    {
        return ::_waccess(path.to_wchar(), 0) == 0;
    }
    virtual bool stat (const bs_string &file)
    {
        return _wstat(file.to_wchar(), &file_stat) == 0;
    }
    virtual int64 tell ()
    {
        return ::ftell(file_fd);
    }
    virtual int64 read (void *vbuf, int64 len)
    {
        return ::fread( vbuf, len, 1, file_fd);
    }
    virtual int64 write (void *vbuf, int64 len)
    {
         return ::fwrite ( vbuf, len, 1, file_fd);
    }
    virtual int64 seek (int64 size, int flag)
    {
        return fseek(file_fd, size, flag);
    }
    virtual bool eof ()
    {
        return ::feof (file_fd);
    }

    static win32io iohandle;
};
win32io win32io::iohandle;
fileio *fileio::defio = &win32io::iohandle;
#elif bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux
struct unixio : fileio
{
    unixio()
    {
        file_fd = NULL;
    }
    virtual bool create (const bs_string &path, int  mode )
    {
        int fil_fd = ::creat((const char*)path.to_utf8().data(), mode);
        if (fil_fd != -1)
            return ::close(fil_fd) == 0;
        return false;
    }
    virtual bool remove (const bs_string &path)
    {
        return ::unlink ((const char*)path.to_utf8().data()) == 0 ;
    }
    virtual bool rename (const bs_string &path_s, const bs_string& path_d)
    {
        return ::rename ((const char*)path_s.to_utf8().data(),
                         (const char*)path_d.to_utf8().data()) == 0 ;
    }

    virtual bool open (const bs_string &path, int mode)
    {
        file_fd = ::fopen((const char*)path.to_utf8 ().data (), return_mode (mode));
        return file_fd != NULL;
    }
    virtual bool close ()
    {
        return ::fclose(file_fd) == 0;
    }
    virtual bool access (const bs_string &path)
    {
        return ::access((const char*)path.to_utf8().data(), 0) == 0;
    }
    virtual bool stat (const bs_string &path)
    {
        return ::stat((const char*)path.to_utf8().data(), &file_stat) == 0;
    }
    virtual int64 tell ()
    {
        return ::ftell(file_fd);
    }
    virtual int64 read (void *vbuf, int64 len)
    {
        return ::fread( vbuf, 1, len, file_fd);
    }
    virtual int64 write (void *vbuf, int64 len)
    {
         return ::fwrite(vbuf, 1, len, file_fd);
    }
    virtual int64 seek (int64 size, int flag)
    {
        return ::fseek(file_fd, size, flag);
    }
    virtual bool eof ()
    {
        return ::feof (file_fd) != 0;
    }

    static unixio iohandle;
};
unixio unixio::iohandle;
fileio *fileio::defio = &unixio::iohandle;
#endif
}

impl_file::impl_file()
{
    impl =
#if bsPlatform == bsPlatform_Win32
            bsNew (impl::win32io());
#elif bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux
            bsNew (impl::unixio());
#endif

    this->io_eof = false;
    this->io_error = Io_NotError;
    this->io_open = false;
    this->io_operation_mode = Io_Not;
    this->io_pos = 0;
    this->io_size = 0;
}
impl_file::~impl_file()
{
    close();
    bsDelete (impl);
}
static const struct tFileErrors
{
    int error;
    const char *errors;
}gfError[] =
{
{Io_NotError, ""},
{Io_StatError, "Get file status error"},
{Io_OpenError, "Open file error"},
{Io_ReadError, "An error occurred while reading the file content"},
{Io_WriteError, "An error occurred while writing to the file content"},
{Io_CloseError, ""},
{Io_NotOpenError, "File is not open"},
{Io_UnknownError, "Unknown error"}
};

bs_string impl_file::errors()const
{
    return gfError[error()].errors;
}
bool impl_file::open(int mode)
{
    if (io_url.is_empty ())
        return false;

    if (is_open ())
        close ();

    bs_string tpath;
    if (io_url.protocol () == Protocol_File)
        tpath = bs_filesystem::slash (io_url.url ());
    else
        io_error = Io_OpenError;

    if (!impl->open(tpath, mode))
        io_error = Io_OpenError;

    if (io_error != Io_NotError)
        return false;
    //! 获取打开句柄的状态信息，获取错误返回
    //! 是目录返回
    //! 不是常规文件返回
    if (!impl->stat(tpath))
        io_error = Io_StatError;

    if (S_ISDIR(impl->file_stat.st_mode) || !S_ISREG(impl->file_stat.st_mode))
    {
        close();
        io_error = Io_StatError;
    }
    if (io_error != Io_NotError)
        return false;

    io_open = true;
    io_size = impl->file_stat.st_size;
    io_operation_mode = mode;
    return true;
}
void impl_file::close()
{
    if (impl->file_fd != NULL && this->io_open)
    {
        if (!impl->close())
            this->io_error = Io_CloseError;
        else
            this->io_error = Io_NotError;

        this->io_eof = false;
        impl->file_fd = NULL;
        this->io_pos = 0;
        this->io_size = 0;
        this->io_open = false;
    }
}
int64 impl_file::tell()const
{
    impl_file *take = (impl_file *)this;
    if (!is_open ())
    {
        take->io_error = Io_NotOpenError;
        return -1;
    }

    take->io_pos = impl->tell();
    return iio::tell ();
}
bool impl_file::seek(int64 pos)
{
    if (!is_open ())
    {
        io_error = Io_NotOpenError;
        return false;
    }

    if (impl->seek(pos > size() ? size() : pos, SEEK_SET) == 0)
    {
        io_pos = pos;
        return true;
    }
    return false;
}
int64 impl_file::read(void *vbuf, int64 len)const
{
    char *buf = (char*)vbuf;
    impl_file *take = (impl_file *)this;
    if (!is_open ())
    {
        take->io_error = Io_NotOpenError;
        return -1;
    }

    int64 left = len;
    while ((left > 0) && (buf != NULL))
    {
        const ssize_t amount = impl->read(buf, left);

        if (amount == 0)
        {
            take->io_eof = true;
            break;
        }

        if (amount == -1)
        {
            take->io_error = Io_ReadError;
            return -1;
        }

        buf += (int64)(amount);
        left -= (int64)(amount);
        memset(buf, 0, left);
    }

    return len - left;
}
int64 impl_file::write(void *vbuf, int64 len)const
{
    char *buf = (char*)vbuf;
    impl_file *take = (impl_file *)this;
    if (!is_open ())
    {
        take->io_error = Io_NotOpenError;
        return -1;
    }

    int64 size =0;
    if (len > 0)
        size = len;
    while ((size > 0) && (buf != NULL))
    {
        const ssize_t amount = impl->write(buf, size);
        if (amount == -1)
        {
            take->io_error = Io_WriteError;
            return false;
        }

        buf += (size_t)(amount);
        size -= (size_t)(amount);
    }
    return len - size;
}
bool impl_file::at_end()const
{
    impl_file *take = (impl_file *)this;
    if (!iio::at_end ())
        take->io_eof = impl->eof ();
    return iio::at_end ();
}
