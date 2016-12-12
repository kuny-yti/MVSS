#ifndef IIO_H
#define IIO_H

#include "tools/bs_string.h"
#include "tools/bs_url.h"
#include "bs_ptr.h"

/*!
 * @brief The eIoModes enum
 * @enum eIoModes
 */
typedef enum
{
    Io_Not = -1,
    Io_Append = 1 << 1,    ///< 附加模式打开
    Io_Binary = 1 << 2,    ///< 二进制模式打开
    Io_Text = 1 << 3,      ///< 文本模式打开
    Io_ReadOnly = 1 << 4,  ///< 只读模式打开
    Io_WriteOnly = 1 << 5, ///< 只写模式打开
    Io_ReadWrite = 1 << 6, ///< 读写模式打开
    Io_Raw
}eIoModes;

/*!
 * @brief The eIoErrors enum
 * @enum eIoErrors
 */
typedef enum
{
    Io_NotError,     ///< 没有错误
    Io_StatError,    ///< 状态操作错误
    Io_OpenError,    ///< 打开错误
    Io_ReadError,    ///< 读取错误
    Io_WriteError,   ///< 写入错误
    Io_CloseError,   ///< 关闭错误
    Io_NotOpenError, ///< 未打开错误
    Io_UnknownError, ///< 未知错误
}eIoErrors;

/*!
 * @brief The iio class 统一文件操作接口
 * @class iio
 */
bsPackage iio
{
    virtual ~iio(){}

    //!
    //! \brief path 返回当前操作的io路径-包括io文件名
    //! \return
    //!
    bs_string path() const{return io_url.path ();}
    //!
    //! \brief url 返回当前操作的完整url路径
    //! \return
    //!
    const bs_url& url() const {return io_url;}
    //!
    //! \brief mode 返回当前操作的模式
    //! \return
    //!
    int mode()const{return io_operation_mode;}
    //!
    //! \brief error 返回当前错误代码
    //! \return
    //!
    int error()const{return io_error;}//eIoErrors

    //!
    //! \brief errors 返回当前错误描述
    //! \return
    //!
    virtual bs_string errors()const=0;

    //!
    //! \brief open 打开需要操作的io
    //! \param url
    //! \param mode
    //! \return=0;
    //!
    virtual bool open(const bs_url &u, int  m = Io_ReadWrite)
    {
        io_url  = u;
        return open(m);
    }
    virtual bool open(const bs_string &name, int mode = Io_ReadWrite)
    {
        return open(bs_url(name), mode);
    }
    virtual bool open(int mode = Io_ReadWrite)= 0;
    //!
    //! \brief close 关闭打开的io
    //!
    virtual void close() = 0;
    //!
    //! \brief is_open 当前io是否打开
    //! \return
    //!
    bool is_open() const{return io_open;}

    //!
    //! \brief available 当前打开的io剩余字节数
    //! \return
    //!
    int64 available()const{return io_size - io_pos;}
    //!
    //! \brief tell 当前打开的io位置
    //! \return
    //!
    virtual int64 tell()const{return io_pos;}
    //!
    //! \brief size 当前打开的io大小
    //! \return
    //!
    int64 size()const{return io_size;}
    //!
    //! \brief seek 移动当前打开的io位置
    //! \param pos
    //! \return
    //!
    virtual bool seek(int64 pos)=0;

    //!
    //! \brief read 读取io数据
    //! \param buf 存放地址
    //! \param len 存放大小
    //! \return 返回读取的实际大小
    //!
    virtual int64 read(void *buf, int64 len)const=0;
    virtual int64 read(bs_byte &buf) const{return read(buf.data (), buf.size ());}

    //!
    //! \brief write 写入io数据
    //! \param buf 写入数据的地址
    //! \param len 写入的长度
    //! \return 返回实际写入的长度
    //!
    virtual int64 write(void *buf, int64 len)const=0;
    virtual int64 write(const bs_byte &buf) const{return write(buf.data (), buf.size ());}

    //!
    //! \brief at_end 是否遇到结束标志
    //! \return
    //!
    virtual bool at_end()const { return io_eof; }

    bs_url io_url;
    int io_error;
    int io_operation_mode;
    bool io_open;
    int64 io_size;
    int64 io_pos;
    bool io_eof;
};

#define PIIO bs_ptr(iio)
typedef PIIO piio;
#undef PIIO

#endif // M_DEMUXER_H
