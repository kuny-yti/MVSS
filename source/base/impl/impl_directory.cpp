#include "impl_directory.h"
#include "bs_filesystem.h"

impl_directory::impl_directory()
{
    dir_handle = NULL;
    this->io_eof = false;
    this->io_error = Io_NotError;
    this->io_open = false;
    this->io_operation_mode = Io_Not;
    this->io_pos = 0;
    this->io_size = 0;
}
impl_directory::~impl_directory()
{
    close ();
    dir_handle = NULL;
    this->io_eof = false;
    this->io_error = Io_NotError;
    this->io_open = false;
    this->io_operation_mode = Io_Not;
    this->io_pos = 0;
    this->io_size = 0;
}

bool impl_directory::open(int)
{
    if (io_url.is_empty())
        return false;

    if (is_open ())
        close ();

    bs_string tpath;
    if (io_url.protocol () == Protocol_File)
        tpath = bs_filesystem::slash (io_url.url ());
    else
        io_error = Io_OpenError;


#   if bsPlatform != bsPlatform_Win32
    dir_handle = dirOpen((char*)tpath.to_latin1());
#   else
    dir_handle = dirOpen(tpath.to_wchar());
#   endif
    io_open = dir_handle != NULL ? true: false;
    return io_open;
}
void impl_directory::close()
{
    if (dir_handle)
        dirClose(dir_handle);

    dir_handle = NULL;
    this->io_eof = false;
    this->io_error = Io_NotError;
    this->io_open = false;
    this->io_operation_mode = Io_Not;
    this->io_pos = 0;
    this->io_size = 0;
}

bs_string impl_directory::next()
{
    while (is_open () && !at_end ())
    {
        struct dirDirent *ptr = dirRead(dir_handle);
        if (ptr == NULL)
        {
            io_eof = true;
            return bs_string();
        }

        offset.push (dirTell(dir_handle));
        if (strcmp(ptr->d_name, ".") && strcmp(ptr->d_name,  ".."))
#             if bsPlatform != bsPlatform_Win32
            return (io_url.path ()+"/"+ ptr->d_name);
#             else
            return (io_url.path () +"\\"+ ptr->d_name);
#             endif
    }
    return bs_string();
}
bs_string impl_directory::prev()
{
    while (is_open () && offset.count ())
    {
        dirSeek(dir_handle, offset.pop ());
        struct dirDirent *ptr = dirRead(dir_handle);
        io_eof = false;
        if (strcmp(ptr->d_name, ".") && strcmp(ptr->d_name,  ".."))
#             if bsPlatform != bsPlatform_Win32
            return (io_url.path ()+"/"+ ptr->d_name);
#             else
            return (io_url.path () +"\\"+ ptr->d_name);
#             endif
    }
    return bs_string();
}
static bs_string filter_suffix;
static int filter_fn(const struct dirent *ent)
{
    if (ent->d_type != DT_REG)
        return 0;
    return (strcmp(ent->d_name, filter_suffix.to_latin1 ()) == 0);
}
bs_string_list impl_directory::scan(const bs_string& dir, const bs_string &filter)
{
    bs_string_list strout;
    if (!dir.is_empty ())
    {
        struct dirDirent **ptr;
        filter_suffix = filter;
        int count = dirScan((char*)dir.to_utf8 ().data (), &ptr, filter.is_empty () ? 0 : filter_fn, alphasort);
        while (count --)
#             if bsPlatform != bsPlatform_Win32
                strout.append(dir+"/"+ ptr[count]->d_name);
#             else
                strout.append(dir +"\\"+ ptr[count]->d_name);
#             endif
    }
    return strout;
}

bool impl_directory::is_directory(const bs_string &path)
{
    bool rc = false;

    if (!path.is_empty ())
    {
        bs_string buffer = path;
        if ( '\\' == buffer.at (buffer.length () -1)||
             '/' == buffer.at (buffer.length () -1) )
        {
            if ( buffer.length() <= 0 )
                return false;
        }

        // this works on Apple and gcc implentations.
        struct stat buf;
        memset(&buf,0,sizeof(buf));
        int stat_errno = stat( buffer, &buf );
        if ( 0 == stat_errno && S_ISDIR(buf.st_mode) )
            rc = true;
    }

    return rc;
}

bool impl_directory::remove(const bs_string& dir)
{
    bs_string tpath = bs_filesystem::slash (dir);
#if bsPlatform != bsPlatform_Win32
    return ::dirRm ((char*)tpath.to_latin1()) == 0 ;
#else
    return ::dirRm (tpath.to_wchar()) == 0 ;
#endif
}
bool impl_directory::create(const bs_string& dir)
{
    bs_string tpath  = bs_filesystem::slash (dir);
#if bsPlatform != bsPlatform_Win32
    return ::dirMk ((char*)tpath.to_latin1(), 777) == 0 ;
#else
    return ::dirMk (tpath.to_wchar()) == 0 ;
#endif
}
bool impl_directory::change(const bs_string & dir)
{
    bs_string tpath = bs_filesystem::slash (dir);
#if bsPlatform != bsPlatform_Win32
    return ::dirCh ((char*)tpath.to_latin1()) == 0 ;
#else
    return ::dirCh (tpath.to_wchar()) == 0 ;
#endif
}
bs_string impl_directory::current(size_t size)
{
#if bsPlatform != bsPlatform_Win32
    size = size;
    return bs_string(get_current_dir_name());
#else
    wchar_t buff[size] = {0};
    _wgetcwd(buff, size);
    return bs_string(buff);
#endif

}
