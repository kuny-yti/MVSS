#ifndef IMPL_DIRECTORY_H
#define IMPL_DIRECTORY_H

#include "interface/iio.h"

#if bsCompiler == bsCompiler_GNUC || bsCompiler == bsCompiler_CLANG
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif
namespace impl {
#if bsPlatform != bsPlatform_Win32
#   define dirHandle  DIR
#   define dirDirent dirent
#   define dirOpen  opendir
#   define dirRead  readdir
#   define dirScan  scandir
#   define dirTell  telldir
#   define dirSeek  seekdir
#   define dirClose closedir
#   define dirRm rmdir
#   define dirMk mkdir
#   define dirCh chdir
#else
#   define dirHandle  _WDIR
#   define dirDirent _wdirent
#   define dirOpen  _wopendir
#   define dirRead  _wreaddir
#   define dirScan  _wscandir
#   define dirTell  _wtelldir
#   define dirSeek  _wseekdir
#   define dirClose _wclosedir
#   define dirRm _wrmdir
#   define dirMk _wmkdir
#   define dirCh _wchdir
#endif
}
class impl_directory :public iio
{
    friend class bs_filesystem;
public:
    impl_directory();
    ~impl_directory();

    virtual bool open(int mode = Io_ReadWrite);
    virtual void close();

    virtual bs_string errors()const{return bs_string();}

    bs_string next();
    bs_string prev();

    static bs_string_list scan(const bs_string& dir, const bs_string &filter = bs_string());

protected:
    static bool remove(const bs_string& dir);
    static bool create(const bs_string& dir);
    static bool change(const bs_string & dir);
    static bs_string current(size_t size);
    static bool is_directory(const bs_string &path);

private:
    virtual bool seek(int64){return false;}
    virtual int64 read(void *, int64 )const{return -1;}
    virtual int64 write(void *, int64 )const{return -1;}

    dirHandle    *dir_handle;
    bs_stack<int64> offset;
};

#endif // IMPL_DIRECTORY_H
