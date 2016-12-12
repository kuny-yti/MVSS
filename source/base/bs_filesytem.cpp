
#include "bs_filesystem.h"

#include "impl/impl_dynamic.h"
#include "impl/impl_file.h"
#include "impl/impl_directory.h"
#include "impl/impl_archive.h"

piio bs_filesystem::create(eFileSystems fs ,int mode )
{
    switch ((int)fs)
    {
    case System_VirtualFile:
        return piio();
    case System_MemoryFile:
        return piio();
    case System_File:
        return bs_malloc(impl_file);
    case System_Directory:
        return bs_malloc(impl_directory);
    case System_ArchiveFile:
        return bs_malloc(impl_archive, mode);
    case System_Dynamic:
        return bs_malloc(impl_dynamic);
    }
    return piio();
}

bs_filesystem::bs_filesystem(const piio &io):
    ope_io(io)
{

}
bs_filesystem::~bs_filesystem()
{

}

bs_byte bs_filesystem::uncompress(const piio &io, const bs_string &fn)
{
    static bs_string passwd;
    impl_archive *archive = io.as<impl_archive>().get ();
    if (archive && archive->is_open ())
    {
        if (archive->open (fn, passwd))
        {
            bs_byte arFile;
            arFile.resize (archive->size ());
            archive->read (arFile.data (), arFile.size ());
            archive->close ();
            return arFile;
        }
    }
    return bs_byte();
}

bool bs_filesystem::compress(const piio &io, const bs_string &fn, const bs_byte &dat)
{
    static bs_string passwd;
    static int32 crc;
    tArchiveInfos info;
    info.crc = crc;
    info.datetime = bs_datetime::current_time ();
    info.name = fn;
    info.uncompressed_size = dat.size ();

    impl_archive *archive = io.as<impl_archive>().get ();
    if (archive && archive->is_open ())
    {
        if (archive->open (info, passwd))
        {
            archive->write (dat.data (), dat.size ());
            archive->close ();
            return true;
        }
    }
    return false;
}

bool bs_filesystem::compress(const piio &io, const bs_string &path)
{
    static bs_string passwd;
    static int32 crc;

    impl_archive *archive = io.as<impl_archive>().get ();
    if (archive && archive->is_open ())
    {
        if (is_directory (path))
        {
            bool dirt = true;
            bs_string_list list = scan (path);
            foreach (const bs_string &arf, list)
                 dirt &= compress (io, arf);
            return dirt;
        }
        else
        {
            tArchiveInfos info;
            bs_byte rdat;

            piio fio = create();

            if (fio->open (path))
            {
                bs_fileinfo finfo(path);
                rdat.resize (fio->size ());
                fio->read (rdat.data (), rdat.size ());

                info.crc = crc;
                info.datetime = finfo.change ();
                info.name = finfo.name ();
                info.uncompressed_size = rdat.size ();
                fio->close ();
            }

            if (archive->open (info, passwd))
            {
                archive->write (rdat.data (), rdat.size ());
                archive->close ();
                return true;
            }
        }
    }

    return false;
}

bool bs_filesystem::create(const bs_string& path, eFileSystems fs)
{
    if (path.is_empty ())
        return false;
    if (fs == System_File && impl::fileio::defio)
        return impl::fileio::defio->create (path, 0777 );
    else if (fs == System_Directory)
        return impl_directory::create (path);
    return false;
}
bool bs_filesystem::remove(const bs_string& path, eFileSystems fs)
{
    if (path.is_empty ())
        return false;
    if (fs == System_File && impl::fileio::defio)
        return impl::fileio::defio->remove(path) ;
    else if (fs == System_Directory)
        return impl_directory::remove (path);

    return false;
}
bool bs_filesystem::rename(const bs_string& path_s, const bs_string &path_d, eFileSystems fs)
{
    if ((path_s.is_empty () && path_d.is_empty ()) || (impl::fileio::defio == NULL))
        return false;
    if (path_s == path_d)
        return true;
    if (fs == System_File && impl::fileio::defio)
        return impl::fileio::defio->rename(path_s, path_d);
    else if (fs == System_Directory)
        return false;

    return false;
}
bool bs_filesystem::clean(const bs_string& path)
{
    if (path.is_empty () || impl::fileio::defio == NULL)
        return false;

    if (impl::fileio::defio->open(path, Io_WriteOnly))
        return impl::fileio::defio->close();

    return false;
}
bool bs_filesystem::exist(const bs_string& path)
{
    if (impl::fileio::defio == NULL)
        return false;
    return impl::fileio::defio->access(path);
}
bs_string bs_filesystem::slash(const bs_string &s)
{
    bs_string out =s;
#   if bsPlatform != bsPlatform_Win32
    out.replace("\\", "/");
#   else
    out.replace("/", "\\");
#   endif
    return out;
}

bs_datetime bs_filesystem::access()const
{
    impl_file * file = ope_io.as<impl_file>().get ();
    if (file && !file->is_open())
        return bs_datetime();

    return bs_datetime(file->impl->file_stat.st_atime * 1000*1000);
}
bs_datetime bs_filesystem::modify()const
{
    impl_file * file = ope_io.as<impl_file>().get ();
    if (file && !file->is_open())
        return bs_datetime();
    return bs_datetime(file->impl->file_stat.st_mtime * 1000*1000);
}
bs_datetime bs_filesystem::change() const
{
    impl_file * file = ope_io.as<impl_file>().get ();
    if (file && !file->is_open())
        return bs_datetime();
    return bs_datetime(file->impl->file_stat.st_ctime * 1000*1000);
}

void *bs_filesystem::symbol(const bs_string &name)
{
    impl_dynamic *dy = ope_io.as<impl_dynamic>().get ();
    if (dy && !dy->is_open())
        return NULL;
    return dy->symbol (name);
}
bs_string bs_filesystem::suffix()const
{
    impl_dynamic *dy = ope_io.as<impl_dynamic>().get ();
    if (dy && !dy->is_open())
        return bs_string();
    return dy->suffix ();
}
bool bs_filesystem::is_directory(const bs_string &path)
{
    return impl_directory::is_directory (path);
}
bs_string_list bs_filesystem::scan(const bs_string &path, const bs_string &filter)
{
    return impl_directory::scan (path, filter);
}
bs_string bs_filesystem::current(size_t s)
{
    return impl_directory::current (s);
}
bool bs_filesystem::change(const bs_string & path)
{
    return impl_directory::change (path);
}


static void splitpath( ushort* path, ushort *sEnd,
                       ushort** drive, ushort** dir, ushort** fname, ushort** ext , ushort** end)
{
    const ushort slash1 = '/';
    const ushort slash2 = '\\';
    ushort* f_file_;
    ushort* f_ext_;
    ushort* s;

    if ( 0 != drive )
        *drive = 0;
    if ( 0 != dir )
        *dir = 0;
    if ( 0 != fname )
        *fname = 0;
    if ( 0 != ext )
        *ext = 0;
    if ( 0 != end )
        *end = 0;

    if ( NULL != path && 0 != (ushort)*path )
    {
        if ((':' == (ushort)path[1]) && bs_char(path[0]).is_letter() )
        {
            if ( drive )
                *drive = path;
            path += 2;
            if ( 0 == (ushort)*path )
                return;

        }
    }

    if ( 0 != path && 0 != (ushort)*path )
    {
        f_ext_ = 0;
        f_file_ = 0;

        s = (sEnd > path) ? sEnd - 1 : path;
        *end = s+1;

        //从字符串最后查找'.'-'/'-'\\'字符 ，不是将s指针向前进
        while ( (s > path) && ('.' != (ushort)*s) && (slash1 != *s) && (slash2 != *s) )
            s--;

        if ( ('.' == (ushort)*s) && (0 != (ushort)s[1]) )
        {
            // 若扩展文件是以'.'做扩展名后缀则取出扩展名
            f_ext_ = s;
            sEnd = f_ext_;
            s--;
        }

        //从字符串最后查找'/'-'\\'字符 ，不是将s指针向前进
        while ( (s > path) && (slash1 != *s) && (slash2 != *s) )
            s--;

        // 取出文件名
        if ( (s >= path) && (s < sEnd) )
        {
            if ((slash1 == *s) || (slash2 == *s ))
            {
                if ( (s+1) < sEnd )
                    f_file_ = s+1;
            }
            else if ( s == path )
            {
                f_file_ = s;
            }
        }

        if ( 0 == f_file_ )
        {
            // must have a non-empty filename in order to have and "extension"
            f_file_ = f_ext_;
            f_ext_ = 0;
        }

        if ( (0 != dir) && (0 == f_file_ || path < f_file_) )
            *dir = path;

        if ( (0 != f_file_) && (0 != fname) )
            *fname = f_file_;

        if ( (0 != f_ext_) && (0 != ext) )
            *ext = f_ext_;
    }

}

static bool split_path( const bs_string &path,
                        bs_string& drive, bs_string& dir,
                        bs_string& fname, bs_string& fext )
{
    ushort *rdrive = NULL;
    ushort *rdir = NULL;
    ushort *rfile = NULL;
    ushort *rext = NULL;
    ushort *rend = NULL;
    splitpath((ushort*)path.data(), (ushort*)path.data()+path.length(), &rdrive, &rdir, &rfile, &rext, &rend);

    if (  NULL != rdrive )
    {
        if ( NULL != rdir )
            drive = bs_string((bs_char*)rdrive, (int)(rdir - rdrive));
        else if ( NULL != rfile )
            drive = bs_string((bs_char*)rdrive, (int)(rfile - rdrive));
        else if ( NULL != rext )
            drive = bs_string((bs_char*)rdrive, (int)(rext - rdrive));
        else
            drive = bs_string((bs_char*)rdrive, path.length());

    }
    else
        drive = bs_string();

    if ( NULL != rdir )
    {
        if ( NULL != rfile )
            dir = bs_string((bs_char*)rdir, (int)(rfile - rdir));
        else if ( NULL != rext )
            dir = bs_string((bs_char*)rdir, (int)(rext - rdir));
        else
            dir = bs_string((bs_char*)rdir, path.length());

    }
    else
        dir = bs_string();


    if ( NULL != rfile )
    {
        if ( NULL != rext )
            fname = bs_string((bs_char*)rfile, (int)(rext - rfile));
        else
            fname = bs_string((bs_char*)rfile, path.length());
    }
    else
        fname = bs_string();


    if ( NULL != rext )
    {
        fext = bs_string((bs_char*)rext, (int)(rend - rext));
    }
    if (drive.is_empty() && dir.is_empty() && fname.is_empty() && fext.is_empty())
        return false;
    return true;
}

bs_fileinfo::bs_fileinfo(const bs_string &pathfile)
{
    impl = bsNew (impl_file);

    impl->impl->stat(pathfile);
    split_path(pathfile, drive_, path_, fname_, fext_);
    file = pathfile;
}
bs_fileinfo::~bs_fileinfo()
{
    bsDelete (impl);
}
bs_string bs_fileinfo::files() const
{
    return file;
}
bool bs_fileinfo::is_exists()const
{
    return impl->impl->access(file);
}

eFileTypes bs_fileinfo::type()const
{
    if (S_ISDIR(impl->impl->file_stat.st_mode))
        return Type_Dir;
    return Type_File;
}

ePermissions bs_fileinfo::permission()const
{
    return Permission_ReadOnly;
}


bs_string bs_fileinfo::drive()const
{
    return drive_;
}

bs_string bs_fileinfo::path()const
{
    return path_;
}

bs_string bs_fileinfo::name()const
{
    return fname_ + fext_;
}

bs_string bs_fileinfo::name_base()const
{
    return fname_;
}

bs_string bs_fileinfo::suffix()const
{
    return fext_;
}
bs_string bs_fileinfo::suffixs()const
{
    bs_string out = fext_;
    out.remove (".");
    return out;
}

int64 bs_fileinfo::size()const
{
    return impl->impl->file_stat.st_size;;
}
bs_datetime bs_fileinfo::access() const
{
    return bs_datetime(impl->impl->file_stat.st_atime * 1000*1000);
}
bs_datetime bs_fileinfo::modify() const
{
    return bs_datetime(impl->impl->file_stat.st_mtime * 1000*1000);
}
bs_datetime bs_fileinfo::change() const
{
    return bs_datetime(impl->impl->file_stat.st_ctime * 1000*1000);
}
