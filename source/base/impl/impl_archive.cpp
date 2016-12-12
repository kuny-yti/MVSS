#include "impl_archive.h"
#include "bs_filesystem.h"

#include <zip/zip.h>
#include <zip/unzip.h>

namespace impl {
struct iarchive : public iio
{
    virtual ~iarchive(){}
    virtual bs_string errors()const{return bs_string();}

    virtual bool open(int mode) = 0;
    virtual bool open(const bs_string& , const bs_string &) {return false;}
    virtual bool open(const tArchiveInfos& , const bs_string &) {return false;}

    tArchiveInfos info(const bs_string &fn)
    {
        tArchiveInfos aiout;
        if (!is_open ())
            return aiout;

        tArchiveInfoLists ail = info();
        foreach (tArchiveInfos &ai , ail)
        {
            if (ai.name == fn)
                return ai;
        }
        return aiout;
    }
    virtual tArchiveInfoLists info() {return tArchiveInfoLists();}
};
struct archive_zip :public iarchive
{
    bs_string comment;
    voidp handle;

    archive_zip(const bs_string &f,eArchiveModes m)
    {
        io_operation_mode = m;
        io_eof  = false;
        io_error = 0;
        io_open = false;
        io_pos = 0;
        io_size = 0;
        io_url = bs_url(f);

        handle = NULL;
        if (!f.is_empty ())
            zip_open(f, NULL);
    }
    ~archive_zip()
    {
        close();
    }

    virtual bool open(int m = Archive_DeCompress)
    {
        io_operation_mode = m;
        return zip_open(io_url.path (), NULL);
    }
    virtual void close()
    {
        if (!is_open())
            return ;
        io_error = UNZ_OK;
        if (io_operation_mode ==  Archive_DeCompress)
            io_error = unzClose(handle);
        else
            io_error = zipClose(handle, comment.is_empty() ? NULL : (char*)comment.to_utf8().data());
        if (io_error == UNZ_OK)
        {
            io_open = false;
            handle = NULL;
        }
    }

    virtual int64 read(void *, int64 )const
    {
        return -1;
    }
    virtual int64 write(void *, int64 )const
    {
        return -1;
    }
    virtual bool seek(int64 )
    {
        return false;
    }
protected:
    bool zip_open(const bs_string &fpn, zlib_filefunc_def* ioApi)
    {
        if(fpn.is_empty())
            return false;
        if (mode() == Archive_DeCompress)
        {
            if (ioApi == NULL)
                handle = unzOpen2_64(fpn.to_utf8().data(), NULL);
            else
                handle = unzOpen2(fpn.to_utf8().data(), ioApi);

            if(handle == NULL)
                return false;
        }
        else
        {
            if (ioApi == NULL)
            {
                handle = zipOpen2_64(fpn.to_utf8().data(), mode() == Archive_Create ? APPEND_STATUS_CREATE:
                                                           mode() == Archive_Append ? APPEND_STATUS_CREATEAFTER:
                                                                                      APPEND_STATUS_ADDINZIP,
                                     NULL, NULL);
            } else {
                handle = zipOpen2(fpn.to_utf8().data(), mode() == Archive_Create ? APPEND_STATUS_CREATE:
                                                        mode() == Archive_Append ? APPEND_STATUS_CREATEAFTER:
                                                                                   APPEND_STATUS_ADDINZIP,
                                  NULL, ioApi);
            }
            if(handle == NULL)
                return false;
        }

        if (mode() == Archive_DeCompress)
        {
            tArchiveInfos ai = info(fpn);
            io_size =  ai.compressed_size;
        }
        io_open = true;
        return true;
    }

};
struct zip_decompress :public archive_zip
{
    bool unz_open;
    unz64_file_pos unz_last_map;
    bs_map<bs_string, unz64_file_pos> unz_map;

    zip_decompress(const bs_string &zName):
        archive_zip(zName, Archive_DeCompress)
    {
        unz_open = false;
        unz_last_map.num_of_file = 0;
        unz_last_map.pos_in_zip_directory = 0;
        unz_map.clear ();
    }
    ~zip_decompress()
    {
        close();
        archive_zip::close ();
    }

    virtual bool open(const bs_string& fName, const bs_string &arPassword)
    {
        return read_open(fName, arPassword.is_empty () ? NULL : (const char *)arPassword.to_utf8 ().data ());
    }
    virtual void close()
    {
        if (unz_open)
        {
            io_error = (unzCloseCurrentFile(handle));
            if(io_error != UNZ_OK)
                return;
        }

        unz_open = false;
        unz_last_map.num_of_file = 0;
        unz_last_map.pos_in_zip_directory = 0;
        unz_map.clear ();
    }

    int64 read(char *buff, int64 max_len)const
    {
        if (!unz_open)
            return -1;

        int64 bytesRead=  unzReadCurrentFile(handle, buff, (unsigned)max_len);
        if (bytesRead < 0)
            return -1;

        return bytesRead;
    }

    virtual int64 tell()const
    {
        zip_decompress * fake = (zip_decompress *)(this);
        if(!is_open() || !unz_open)
            return -1;
        fake->io_pos = unztell(handle);
        return io_pos;
    }

    virtual bool at_end()const
    {
        zip_decompress * fake = (zip_decompress *)(this);
        if(!is_open() || !unz_open)
            return true;
        fake->io_eof = unzeof(handle ) == 1;
        return io_eof;
    }

    tArchiveInfoLists info()
    {
        tArchiveInfoLists result;
        if (!is_open() || unz_open)
            return result;
        if (first_file())
        {
            do
            {
                tArchiveInfos ai;
                bool ok = info(&ai);
                result.append(ai);
                if (!ok)
                    return result;
            } while (next_file());
        }
        return result;
    }
protected:
    bool read_open(const bs_string &f_name, const char *password, int *method = NULL, int *level = NULL)
    {
        if(!select(f_name))
        {
            close();
            return false;
        }

        io_error = UNZ_OK;
        io_error = (unzOpenCurrentFile3(handle, method, level, (int)io_error, password));
        if(io_error != UNZ_OK)
            return false;

        unz_open = true;
        return true;
    }

    /////////
    bool first_unmapped_file()
    {
        io_error = UNZ_OK;

        if (this->unz_last_map.pos_in_zip_directory == 0)
            io_error = unzGoToFirstFile(handle);
        else
        {
            io_error = unzGoToFilePos64(handle, &this->unz_last_map);
            io_error = unzGoToNextFile(handle);
        }

        return io_error == UNZ_OK;
    }
    bool first_file()
    {
        io_error = UNZ_OK;
        io_error = unzGoToFirstFile(handle);
        return io_error == UNZ_OK;
    }
    bool next_file()
    {
        io_error = UNZ_OK;
        io_error = unzGoToNextFile(handle);
        return io_error == UNZ_OK;
    }
    void add_map(const bs_string &fileName)
    {
        if (fileName.is_empty())
            return;

        unz64_file_pos fdPos;
        unzGetFilePos64(handle, &fdPos);
        unz_map.insert(fileName, fdPos);

        if (fdPos.pos_in_zip_directory > unz_last_map.pos_in_zip_directory)
            unz_last_map = fdPos;
    }

    bool select(const bs_string& fileName)
    {
        io_error = UNZ_OK;
        if(fileName.is_empty())
            return false;

        if(handle == NULL)
            return false;

        if(fileName.length() > aMaxFileNameLength)
            return false;

        bs_string current;
        unz64_file_pos fileDirPos;
        fileDirPos.pos_in_zip_directory = 0;
        bs_map<bs_string, unz64_file_pos>::iterator it;
        for (it = unz_map.begin (); it != unz_map.end (); ++it)
        {
            if (it.key() == fileName)
                fileDirPos =  it.value();
        }

        if (fileDirPos.pos_in_zip_directory != 0)
        {
            io_error = unzGoToFilePos64(handle, &fileDirPos);
            if (io_error == UNZ_OK)
                return true;
        }

        for(bool more = first_unmapped_file(); more; more = next_file())
        {
            current = select();
            if(current.is_empty())
                return false;

            if(current == fileName)
                break;
        }
        return io_error == UNZ_OK;
    }
    bs_string select()
    {
        io_error = UNZ_OK;
        if(!is_open() || !unz_open)
            return bs_string();

        bs_vector<char> fileName;
        fileName.resize(aMaxFileNameLength, 0);
        if((io_error = unzGetCurrentFileInfo64(handle, NULL, fileName.data(), fileName.count(),
                                               NULL, 0, NULL, 0)) != UNZ_OK)
            return bs_string();
        bs_string result = bs_string(fileName.data());
        if (result.is_empty())
            return result;
        this->add_map(result);
        return result;
    }

    bool info(tArchiveInfos *info)
    {
        io_error = UNZ_OK;
        unz_file_info64 info_z;
        bs_vector<char> fileName, extra, comment;

        if ((info == NULL) || !handle)
            return false;

        if((io_error = unzGetCurrentFileInfo64(handle, &info_z, NULL, 0, NULL, 0, NULL, 0)) != UNZ_OK)
            return false;
        fileName.resize(info_z.size_filename);
        extra.resize(info_z.size_file_extra);
        comment.resize(info_z.size_file_comment);
        if((io_error = unzGetCurrentFileInfo64(handle, NULL, fileName.data(), fileName.count(),
                                               extra.data(), extra.count(), comment.data(), comment.count()))!=UNZ_OK)
            return false;
        info->version_created = info_z.version;
        info->version_needed = info_z.version_needed;
        info->flags = info_z.flag;
        info->method = info_z.compression_method;
        info->crc = info_z.crc;
        info->compressed_size = info_z.compressed_size;
        info->uncompressed_size = info_z.uncompressed_size;
        info->disk_start = info_z.disk_num_start;
        info->internal_attr = info_z.internal_fa;
        info->external_attr = info_z.external_fa;
        info->name = bs_string(fileName.data(), fileName.count());
        info->comment = bs_string(comment.data(), comment.count());
        info->extra = extra;
        info->datetime = bs_datetime(
                    info_z.tmu_date.tm_year, info_z.tmu_date.tm_mon+1, info_z.tmu_date.tm_mday,
                    info_z.tmu_date.tm_hour, info_z.tmu_date.tm_min, info_z.tmu_date.tm_sec);
        // Add to directory map
        this->add_map(info->name);
        return true;
    }

    bs_string _comment_()
    {
        io_error = UNZ_OK;
        unz_global_info64 globalInfo;
        bs_vector<char> comment;
        if((io_error = unzGetGlobalInfo64(handle, &globalInfo)) != UNZ_OK)
            return bs_string();
        comment.resize(globalInfo.size_comment);
        if((io_error = unzGetGlobalComment(handle, comment.data(), comment.count())) < 0)
            return bs_string();
        return bs_string(comment.data(), comment.count());
    }

};
struct zip_compress :public archive_zip
{
    int32 zip_crc;
    bool zip_open;
    bool is_raw;

    zip_compress(const bs_string &zName, eArchiveModes m):
        archive_zip(zName, m)
    {
        is_raw = false;
        zip_crc = 0;
        zip_open = false;
    }
    ~zip_compress()
    {
        close();
        archive_zip::close ();
    }
    virtual bool open(const tArchiveInfos& info, const bs_string &arPassword)
    {
        return write_open(info, arPassword.is_empty () ? NULL : (const char *)arPassword.to_utf8 ().data ());
    }

    virtual void close()
    {
        if (zip_open)
        {
            if(is_raw)
                io_error = zipCloseFileInZipRaw64(handle, io_size, zip_crc);
            else
                io_error = zipCloseFileInZip(handle);

            if(io_error != UNZ_OK)
                return;
        }

        is_raw = false;
        zip_crc = 0;
        zip_open = false;
    }
    int64 write(char *buff, int64 max_len)const
    {
        if (!is_open() || !zip_open)
            return -1;

        zip_compress * fake = (zip_compress *)(this);
        fake->io_error = ZIP_OK;
        fake->io_error = zipWriteInFileInZip(handle, buff, (uint)max_len);
        if(io_error != ZIP_OK)
            return -1;

        fake->io_pos += max_len;
        return max_len;
    }
protected:
    bool write_open(const tArchiveInfos& info,
                    const char *password, bool DDWE = false,
                    uint32 crc = 0, int method = Z_DEFLATED,
                    int level = Z_DEFAULT_COMPRESSION, bool raw = false,
                    int windowBits = -MAX_WBITS, int memLevel = DEF_MEM_LEVEL,
                    int strategy = Z_DEFAULT_STRATEGY)
    {
        zip_fileinfo info_z;
        io_error = ZIP_OK;
        if(!is_open() || zip_open)
            return false;

        if((io_operation_mode != Archive_Create) &&
                (io_operation_mode != Archive_Append) &&
                (io_operation_mode != Archive_Add))
            return false;

        info_z.tmz_date.tm_year = info.datetime.year();
        info_z.tmz_date.tm_mon = info.datetime.month() - 1;
        info_z.tmz_date.tm_mday = info.datetime.day();
        info_z.tmz_date.tm_hour = info.datetime.hour();
        info_z.tmz_date.tm_min = info.datetime.minute();
        info_z.tmz_date.tm_sec = info.datetime.second();
        info_z.dosDate = 0;
        info_z.internal_fa = (uLong)info.internal_attr;
        info_z.external_fa = (uLong)info.external_attr;

        if (!DDWE)
            zipClearFlags(handle, ZIP_WRITE_DATA_DESCRIPTOR);

        io_error = zipOpenNewFileInZip3_64(handle,
                                        (char*)info.name.to_utf8().data(), &info_z,
                                        info.extra.data(), info.extra.size(),
                                        info.extra.data(), info.extra.size(),
                                        (char*)info.comment.to_utf8().data(),
                                        method, level, (int)raw,
                                        windowBits, memLevel, strategy,
                                        password, (uLong)crc, true);
        if(io_error == UNZ_OK)
        {
            io_pos = 0;

            if(raw) {
                zip_crc = crc;
                io_size = info.uncompressed_size;
            }
            zip_open = true;
            return true;
        }

        return false;
    }


};
}

impl_archive::impl_archive(int mo)
{
    io_error = 0;
    io_open = false;
    io_size = 0;
    io_pos = 0;
    io_eof = false;
    io_operation_mode = mo;

    if (mo == Archive_DeCompress)
        impl = bsNew(impl::zip_decompress(bs_string()));
    else
        impl = bsNew(impl::zip_compress(bs_string(), (eArchiveModes)mo));
}
impl_archive::~impl_archive()
{
    bsDelete (impl);
}
bs_string impl_archive::errors()const
{
    return impl->errors ();
}
bool impl_archive::open(const bs_string& fName, const bs_string &arPassword)
{
    return impl->open(fName, arPassword);
}
bool impl_archive::open(const tArchiveInfos& info, const bs_string &arPassword)
{
    return impl->open (info, arPassword);
}
bool impl_archive::open(int mode)
{
    return impl->open (mode);
}
void impl_archive::close()
{
    return impl->close();
}

int64 impl_archive::tell()const
{
    return impl->tell();
}
bool impl_archive::seek(int64 pos)
{
    return impl->seek (pos);
}
int64 impl_archive::read(void *buf, int64 len)const
{
    return impl->read (buf, len);
}
int64 impl_archive::write(void *buf, int64 len)const
{
    return impl->write (buf, len);
}
bool impl_archive::at_end()const
{
    return impl->at_end ();
}
