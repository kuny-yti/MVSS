#ifndef BS_STRING
#define BS_STRING

#include "bs_define.h"
#include "bs_vector.h"
#include "bs_list.h"
#include "bs_array.h"

class bs_string;

typedef enum
{
    UTF_UnSet=0, // 不是UTF BOM
    UTF_Not,     // 不是UTF 编码
    UTF_8,       // UTF-8 本机 CPU 字节序
    UTF_16,      // UTF-16 本机 CPU 字节序
    UTF_16be,    // UTF-16 大端字节序
    UTF_16le,    // UTF-16 小端字节序
    UTF_32,      // UTF-32 本机 CPU 字节序
    UTF_32be,    // UTF-32 大端字节序
    UTF_32le     // UTF-32 小端字节序
}eUnicodeEncodings;

class bs_char
{
public:
    bs_char();
    bs_char( const bs_char &);
    bs_char( char);
    bs_char( wchar_t);
    bs_char(uint16);

    ~bs_char();

    int digit() const ;
    char to_latin1() const;
    bs_char to_lower() const ;
    bs_char to_upper() const ;
    bs_char to_title_case() const ;
    bs_char to_case_folded() const ;
    bs_char from_latin1(char c);

    // 作为字符
    operator char () const;
    operator wchar_t () const;
    operator uint16 ()const;
    operator uint16 &();

    uint16 unicode()const;
    uint16 &unicode() ;

    //! 判断是否是空子符'\0'
    bool is_valid()const;
    //! 判断是否是字母或数字
    bool is_letter_number() const;
    bool is_title_case() const;
    bool is_non_char() const ;
    bool is_high_surrogate() const ;
    bool is_low_surrogate() const ;
    bool is_surrogate() const ;
    //! 判断是否是十进制数字（'0' - '9'）
    bool is_digit() const;
    //! 判断是否是数字，包括正负号、小数点等
    bool is_number() const;
    //! 判断是否是小写字母
    bool is_lower() const;
    //! 判断是否是大写字母
    bool is_upper() const;
    //! 判断是否是字母
    bool is_letter() const;
    //! 判断是否是分隔符，包括空格等
    bool is_space() const;
    bool is_punct() const;
    //! 判断是否是可打印字符
    bool is_print() const;
    bool is_symbol() const;
    bool is_mark()const;
    bool is_unicode() const;

    bs_char& operator=(uint16);
    bs_char& operator=(char);
    bs_char& operator=(wchar_t);
    bs_char& operator=(const bs_char &);

    bool operator==(char) const;
    bool operator==(wchar_t)const ;
    bool operator==(const bs_char &)const ;

    bool operator!=(const bs_char&)const ;
    bool operator!=(wchar_t)const ;
    bool operator!=(char)const ;

    void lower();
    void upper();
    void reverse();
private:
    uint16  data;
};


typedef bs_list<bs_string> bs_string_list;
typedef bs_array<uint8, bs_alloc<uint8> > bs_utf8;
typedef bs_array<uint16, bs_alloc<uint16> > bs_utf16;
typedef bs_array<uint32, bs_alloc<uint32> > bs_utf32;

class bs_string : public bs_utf16
{
public:
    bs_string();

    bs_string( const bs_string& );
    bs_string( const std::string& );
    bs_string( const std::wstring& );
    bs_string( const bs_char*, int /*length*/ );
    bs_string( const bs_utf16 &array);
    bs_string( const char* );
    bs_string( const char*, int /*length*/ );
    bs_string( const wchar_t* src );
    bs_string( const wchar_t* src, int length );

    ~bs_string();

    std::string std_string()const;
    std::wstring std_wstring()const;

     //! 作为字符串
public:
    bs_char* data();
    const bs_char* data() const;

    bs_char& operator[](int);
    bs_char operator[](int) const;
    bs_char at(int) const;
    void at(int, bs_char);

    operator bs_char* () const;
    operator char* () const;
    operator wchar_t* () const;

public:
    //!字符串长度
    size_t length() const;
    int count() const;

    //!是否为拉丁字符串
    bool is_latin1() const;
    //!是否为国际字符集
    bool is_unicode() const;
    //!是否为数字字符串
    bool is_number() const;

    //!包含指定字符串
public:
    bool contains(const char* )const;
    bool contains(const wchar_t* )const;
    bool contains(const bs_string &)const;
    bool contains(const std::string &)const;
    bool contains(const std::wstring &)const;

    //!字符串连接
public:
    bs_string& append( const char*, int =-1);
    bs_string& append( const bs_char*, int );
    bs_string& append( const wchar_t*, int =-1);
    bs_string& append( const bs_string& );
    bs_string& append( const std::string& );
    bs_string& append( const std::wstring& );

    //!字符串插入
public:
    bs_string& insert(int pos,  const char*, int =-1);
    bs_string& insert(int pos,  const bs_char*, int );
    bs_string& insert(int pos,  const wchar_t*, int =-1);
    bs_string& insert(int pos,  const bs_string&);
    bs_string& insert(int pos,  const std::string&);
    bs_string& insert(int pos,  const std::wstring&);

    //!字符串删除
public:
    bs_string& remove(int pos, int len);
    bs_string& remove(const char*, int =-1);
    bs_string& remove(const bs_char*, int );
    bs_string& remove(const wchar_t*, int =-1);
    bs_string& remove(const bs_string &str);
    bs_string& remove(const std::string &str);
    bs_string& remove(const std::wstring &str);

    //!字符串替换
public:
    int replace( int pos, int len, const bs_string &token );
    int replace( const char* token1, const char* token2 );
    int replace( const wchar_t* token1, const wchar_t* token2 );
    int replace( const bs_string &token1, const bs_string &token2 );
    int replace( const std::string &token1, const std::string &token2 );
    int replace( const std::wstring &token1, const std::wstring &token2 );

    //!字符串提取，截断
public:
    //! count 负数时根据pos位置向前取，此时顺序不为逆
    bs_string extract( int pos, int count)const;
    //! 根据pos位置将后面的全部提取
    bs_string extract( int pos )const ;
    //! 从开头提取count个字符
    bs_string start( int count)const;
    //! 从结尾提取count个字符，此时不为逆
    bs_string end( int count) const ;
    //! 修饰字符串
    bs_string trimmed() const;

    //!字符串查找
public:
    int find(const char*) const;
    int find(const wchar_t*) const;
    int find(const bs_string&) const;
    int find(const std::string &) const;
    int find(const std::wstring &) const;

    //!字符串分割
public:
    bs_string_list split(const char*)const;
    bs_string_list split(const wchar_t*)const;
    bs_string_list split(const bs_string&)const;
    bs_string_list split(const std::string &)const;
    bs_string_list split(const std::wstring &)const;

    //!字符串比较
public:
   int compare(const char*)const;
   int compare(const wchar_t*)const;
   int compare(const bs_string&)const;
   int compare(const std::string &)const;
   int compare(const std::wstring &)const;

   //! 格式输入
public:
   bs_string& format( const char*, ...) ;
   bs_string& format( const wchar_t*, ...);

   static bs_string formats( const char*, ...) ;
   static bs_string formats( const wchar_t*, ...);

   //! ? and * 通配符
public:

   //! 重载 =
public:
   bs_string& operator=(const char*);
   bs_string& operator=(const wchar_t* );
   bs_string& operator=(const bs_string&);
   bs_string& operator=(const std::string &);
   bs_string& operator=(const std::wstring &);

   //! 重载 ==
public:
   bool operator==(const char*str)const {return compare(str) == 0;}
   bool operator==(const wchar_t* str)const {return compare(str) == 0;}
   bool operator==(const bs_string&str)const {return compare(str) == 0;}
   bool operator==(const std::string &str)const {return compare(str) == 0;}
   bool operator==(const std::wstring &str)const {return compare(str) == 0;}

   //! 重载 !=
public:
   bool operator!=(const char*str)const {return !((*this) == str);}
   bool operator!=(const wchar_t* str)const{return !((*this) == str);}
   bool operator!=(const bs_string&str)const{return !((*this) == str);}
   bool operator!=(const std::string &str)const{return !((*this) == str);}
   bool operator!=(const std::wstring &str)const{return !((*this) == str);}

   //! 重载 +
public:
   bs_string operator+(const char*) const;
   bs_string operator+(const wchar_t*) const;
   bs_string operator+(const bs_string&) const;
   bs_string operator+(const std::string&) const;
   bs_string operator+(const std::wstring&) const;

   //! 重载 +=
public:
   const bs_string& operator+=(const char*str){return append(str);}
   const bs_string& operator+=(const wchar_t*str){return append(str);}
   const bs_string& operator+=(const bs_string&str){return append(str);}
   const bs_string& operator+=(const std::string&str){return append(str);}
   const bs_string& operator+=(const std::wstring&str){return append(str);}

public:
   bool operator < (const bs_string&str)const;
   bool operator > (const bs_string&str)const;
   //!转换字符串
public:
   char* to_latin1()const;
   wchar_t* to_wchar()const;
   bs_utf8 to_utf8()const;
   bs_utf16 to_utf16()const;
   bs_utf32 to_utf32()const;
   bs_byte to_byte()const;

   void upper();
   void lower();
   void reverse();
   //!
public:
   bool form_latin1(const char*);
   bool form_wchar(const wchar_t*);
   bool form_utf8(const uint8*, int );
   bool form_utf16(const uint16*, int );
   bool form_utf32(const uint32*, int );

   static bs_utf8 to_utf8s(uint16*, int) ;
   static bs_utf8 to_utf8s(uint32*, int) ;
   static bs_utf16 to_utf16s(uint8*, int) ;
   static bs_utf16 to_utf16s(uint32 *, int) ;
   static bs_utf32 to_utf32s(uint8*, int) ;
   static bs_utf32 to_utf32s(uint16*, int) ;

   //! 数字输入, radix 代表需要存储为什么进制, prec 代表精度
public:
   bs_string &form(bool, int radix = 10);
   bs_string &form(char, int radix = 10);
   bs_string &form(uchar, int radix = 10);
   bs_string &form(short, int radix = 10);
   bs_string &form(uint16, int radix = 10);
   bs_string &form(int, int radix = 10);
   bs_string &form(uint, int radix = 10);
   bs_string &form(long, int radix = 10);
   bs_string &form(ulong, int radix = 10);
   bs_string &form(longlong, int radix = 10);
   bs_string &form(ulonglong, int radix = 10);
   bs_string &form(float, int prec=10);
   bs_string &form(real, int prec=10);

   //! 数字输出 radix 代表存储的为什么进制
public:
   bool      to_bool(int radix = 10);
   char      to_char(int radix = 10);
   uchar     to_uchar(int radix = 10);
   short     to_short(int radix = 10);
   ushort    to_ushort(int radix = 10);
   int       to_int(int radix = 10);
   uint      to_uint(int radix = 10);
   long      to_long(int radix = 10);
   ulong     to_ulong(int radix = 10);
   longlong  to_longlong(int radix = 10);
   ulonglong to_ulonglong(int radix = 10);
   float     to_float();
   real      to_real();
};

#endif // BS_STRING

