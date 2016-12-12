
#include "interface/iio.h"
#include "tools/bs_datetime.h"

typedef enum
{
    Archive_Not,
    Archive_DeCompress,
    Archive_Compress,
    Archive_Create,
    Archive_Append,
    Archive_Add
}eArchiveModes;

typedef enum
{
    Type_File,
    Type_Dir,
    Type_Link
}eFileTypes;

typedef enum
{
    Permission_ReadOnly = 1,
    Permission_WriteOnly = 2,
    Permission_Excutable = 4,
    Permission_Hidden = 8,
    Permission_ReadWrite = Permission_ReadOnly | Permission_WriteOnly,
    Permission_ReadExcutable = Permission_ReadOnly | Permission_Excutable,
    Permission_WriteExcutable = Permission_WriteOnly | Permission_Excutable,
    Permission_ReadWriteExcutable = Permission_ReadOnly | Permission_WriteOnly | Permission_Excutable
}ePermissions;

/*!
 * @brief The eFileSystems enum
 * @enum eFileSystems
 */
typedef enum
{
    System_File,        ///< 普通文件
    System_VirtualFile, ///< 虚拟文件
    System_MemoryFile,  ///< 内存文件
    System_ArchiveFile, ///< 归档文件

    System_Dynamic,     ///< 动态库文件

    System_Directory,   ///< 目录

}eFileSystems;


class bs_fileinfo
{
public:
    bs_fileinfo(const bs_string &pathfile);
    virtual ~bs_fileinfo();

    bs_string files() const;
    //!
    //! \brief is_exists 检测文件或目录是否存在
    //! \return true 存在，否则不存在
    //!
    bool is_exists()const;
    //!
    //! \brief type 文件的类型，是文件还是目录或是符号链接
    //! \return
    //!
    eFileTypes type()const;
    //!
    //! \brief permission 文件的访问权限
    //! \return
    //!
    ePermissions permission()const;

    //!
    //! \brief drive 当前文件的驱动器
    //! \return
    //!
    bs_string drive()const;
    //!
    //! \brief path 当前文件的路径，不含驱动器
    //! \return
    //!
    bs_string path()const;
    //!
    //! \brief name 文件名称，包含扩展名
    //! \return
    //!
    bs_string name()const;
    //!
    //! \brief name_base 文件名称，不含扩展名
    //! \return
    //!
    bs_string name_base()const;
    //!
    //! \brief suffix 文件的扩展名
    //! \return
    //!
    bs_string suffix()const;
    bs_string suffixs()const;

    //!
    //! \brief size 文件大小
    //! \return
    //!
    int64 size()const;
    //!
    //! \brief access 最后一次访问时间
    //! \return
    //!
    bs_datetime access() const;
    //!
    //! \brief modify 最后一次修改时间
    //! \return
    //!
    bs_datetime modify() const;
    //!
    //! \brief change 最后一次属性改变时间
    //! \return
    //!
    bs_datetime change() const;

private:
    class impl_file *impl;
    bs_string file;

    bs_string drive_;
    bs_string path_;
    bs_string fname_;
    bs_string fext_;
};

class bs_filesystem
{
    // Directory
public:
    //!
    //! \brief scan 返回指定路径下包含的文件
    //! \param path 扫描路径
    //! \param filter 后缀过滤
    //! \return
    //!
    static bs_string_list scan(const bs_string &path, const bs_string &filter = bs_string());
    //!
    //! \brief is_directory 返回是否为目录
    //! \param path
    //! \return
    //!
    static bool is_directory(const bs_string &path);
    //!
    //! \brief current 当前所在的目录
    //! \param size
    //! \return
    //!
    static bs_string current(size_t size = 4096);
    //!
    //! \brief change 改变目录
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool change(const bs_string & path);

    // file Directory
public:
    //!
    //! \brief remove 删除指定文件,目录
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool remove(const bs_string& path, eFileSystems fs = System_File);
    //!
    //! \brief create 创建指定文件,目录
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool create(const bs_string& path, eFileSystems fs = System_File);
    //!
    //! \brief rename 重命名指定文件
    //! \param path_s
    //! \param path_d
    //! \return true 成功，否则失败
    //!
    static bool rename(const bs_string& path_s, const bs_string& path_d, eFileSystems fs = System_File);

    //!
    //! \brief clean 清空指定文件内容
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool clean(const bs_string& path);
    //!
    //! \brief exist 指定文件是否存在
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool exist(const bs_string& path);
    //!
    //! \brief slash 路径中斜杠的处理
    //! \param path
    //! \return
    //!
    static bs_string slash(const bs_string &path);

    // File
public:
    bs_filesystem(const piio &io = piio());
    ~bs_filesystem();
    //!
    //! \brief access 最后一次访问时间
    //! \return
    //!
    bs_datetime access()const;
    //!
    //! \brief modify 最后一次修改时间
    //! \return
    //!
    bs_datetime modify()const;
    //!
    //! \brief change 最后一次属性改变时间
    //! \return
    //!
    bs_datetime change() const;

    // Dynamic
public:
    //!
    //! \brief symbol 文件内的符号地址
    //! \param name 符号名
    //! \return
    //!
    void *symbol(const bs_string &name);
    //!
    //! \brief suffix 文件使用的后缀
    //! \return
    //!
    bs_string suffix()const ;

    // Archive
public:
    //!
    //! \brief uncompress 从归档中解压指定文件
    //! \param io 文件io
    //! \param fn 文件名
    //! \return   文件内容
    //!
    bs_byte uncompress(const piio &io, const bs_string &fn);
    //!
    //! \brief compress 根据模式压缩文件到归档中
    //! \param io  文件io
    //! \param fn  文件名
    //! \param dat 文件内容
    //! \return
    //!
    bool compress(const piio &io, const bs_string &fn, const bs_byte &dat);
    //!
    //! \brief compress 根据模式压缩文件到归档中
    //! \param io 文件io
    //! \param path 文件路径或目录路径
    //! \return
    //!
    bool compress(const piio &io, const bs_string &path);
    /*
    bool passwd(const piio &io, const bs_string &passwd);
    bool crc(const piio &io, int32 crc);*/
public:
    static piio create(eFileSystems fs = System_File, int mode = Archive_DeCompress) ;

private:
    piio ope_io;
};
