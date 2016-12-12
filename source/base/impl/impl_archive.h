#ifndef IMPL_ARCHIVE_H
#define IMPL_ARCHIVE_H

#include "interface/iio.h"
#include "bs_datetime.h"

const int aMaxFileNameLength = 256;
struct tArchiveInfos
{
    /// File name.
    bs_string name;
    /// Version created by.
    uint16 version_created;
    /// Version needed to extract.
    uint16 version_needed;
    /// General purpose flags.
    uint16 flags;
    /// Compression method.
    uint16 method;
    /// Last modification date and time.
    bs_datetime datetime;
    /// CRC.
    uint32 crc;
    /// Compressed file size.
    uint64 compressed_size;
    /// Uncompressed file size.
    uint64 uncompressed_size;
    /// Disk number start.
    uint16 disk_start;
    /// Internal file attributes.
    uint16 internal_attr;
    /// External file attributes.
    uint32 external_attr;
    /// Comment.
    bs_string comment;
    /// Extra field.
    bs_vector<char> extra;
};

typedef bs_list<tArchiveInfos> tArchiveInfoLists;

namespace impl {
struct iarchive;
}
class impl_archive :public iio
{
public:
    impl_archive(int mo);
    ~impl_archive();

    bool open(const bs_string& fName, const bs_string &arPassword);
    bool open(const tArchiveInfos& info, const bs_string &arPassword);

    virtual bs_string errors()const;
    virtual bool open(int mode);
    virtual void close();

    virtual int64 tell()const;
    virtual bool seek(int64 pos);
    virtual int64 read(void *buf, int64 len)const;
    virtual int64 write(void *buf, int64 len)const;
    virtual bool at_end()const;
private:
    impl::iarchive *impl;
};

#endif // IMPL_ARCHIVE_H
