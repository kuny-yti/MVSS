
#include "bs_string.h"
#include "unicode.h"
#include "bs_ucsprivate.h"
#include "bs_number_double.h"
#include "bs_number_longlong.h"
#include "bs_debug.h"


/////////////////////////bs_string////////////////////////////////
bs_string::bs_string():
    bs_utf16()
{
}
bs_string::bs_string( const bs_string& str):
    bs_utf16(str)
{
}

bs_string::bs_string( const std::string& str):
    bs_utf16()
{
    *this = str;
}

bs_string::bs_string( const std::wstring& str):
    bs_utf16()
{
    *this = str;
}

bs_string::bs_string( const bs_char* str, int len ):
    bs_utf16((uint16*)str, len)
{
}
bs_string::bs_string( const bs_utf16 &array):
    bs_utf16(array)
{

}
bs_string::bs_string( const char* str):
    bs_utf16()
{
    *this = str;
}

bs_string::bs_string( const char*str, int len ):
    bs_utf16()
{
    this->append(str, len);
}
bs_string::bs_string( const wchar_t* str ):
    bs_utf16()
{
    *this = str;
}

bs_string::bs_string( const wchar_t* str, int len ):
    bs_utf16()
{
    this->append(str, len);
}
bs_string::~bs_string()
{
}
std::string bs_string::std_string() const
{
    return std::string((char*)to_utf8().data ());
}
std::wstring bs_string::std_wstring()const
{
    return std::wstring(to_wchar ());
}
/////////////////////return////////////////////////////
bs_char* bs_string::data()
{
    bs_utf16::detach();
    return (bs_char* )bs_utf16::data();
}
const bs_char* bs_string::data() const
{
    return (bs_char* )bs_utf16::data();
}

bs_char& bs_string::operator[](int index)
{
    bs_utf16::detach();
    bsASSERT((size_t)index >= length(), "bs_string [] index illegal");
    return (*(bs_char*)(data()+ index));
}
bs_char bs_string::operator[](int index) const
{
    bsASSERT((size_t)index >= length(), "bs_string [] illegal");
    return (*(bs_char*)(data()+ index));
}
bs_char bs_string::at(int index) const
{
    return this->operator [](index);
}
void bs_string::at(int index, bs_char c)
{
    bs_utf16::detach();
    bsASSERT((size_t)index >= length(), "bs_string at() illegal");
    this->data()[index] = c.unicode();
}

bs_string::operator bs_char* () const
{
    bs_string *take = (bs_string *)this;
    return take->data();
}
bs_string::operator char* () const
{
    return (char*)to_utf8().data();
}
bs_string::operator wchar_t* () const
{
    return to_wchar();
}

////////////////////////is_//////////////////////////////////

size_t bs_string::length() const
{
    return this->size();
}
int bs_string::count() const
{
    return (int)length();
}

bool bs_string::is_latin1() const
{
    bool rB = true;
    for (size_t i = 0; i < length(); ++i)
    {
        if (!this->data()[i].is_letter())
            rB = false;
    }
    return rB;
}
bool bs_string::is_unicode() const
{
    bool rB = true;
    for (size_t i = 0; i < length(); ++i)
    {
        if (!this->data()[i].is_unicode())
            rB = false;
    }
    return rB;
}
bool bs_string::is_number() const
{
    bool rB = true;
    for (size_t i = 0; i < length(); ++i)
    {
        if (!this->data()[i].is_number())
            rB = false;
    }
    return rB;
}
///////////////////////////contains///////////////////////////////////////
bool bs_string::contains(const char* str)const
{
    int fs = find(str);
    return fs >= 0;
}
bool bs_string::contains(const wchar_t* str)const
{
    int fs = find(str);
    return fs >= 0;
}
bool bs_string::contains(const bs_string &str)const
{
    int fs = find(str);
    return fs >= 0;
}
bool bs_string::contains(const std::string &str)const
{
    int fs = find(str);
    return fs >= 0;
}
bool bs_string::contains(const std::wstring &str)const
{
    int fs = find(str);
    return fs >= 0;
}

//////////////////////////append///////////////////////////////
bs_string& bs_string::append( const char*str, int len)
{
    if (str == NULL)
        return *this;
    if (len == -1)
        len = strlen(str);
    uint16 tmp[len+1] ;
    ::from_latin1(str, len, tmp);
    bs_utf16::append(tmp, len);
    return *this;
}
bs_string& bs_string::append( const bs_char* str, int len)
{
    if (str == NULL)
        return *this;
    bs_utf16::append((uint16*)str, len);
    return *this;
}
bs_string& bs_string::append( const wchar_t* str, int len)
{
    if (str == NULL)
        return *this;
    if (len == -1)
        len = wcslen(str);
    uint16 tmp[len+1];
    if (!bsHasUTF32)
        ::from_utf16(reinterpret_cast<const uint16 *>(str), len, tmp);
    else
        ::from_ucs4(reinterpret_cast<const uint32 *>(str), len, tmp);

    bs_utf16::append(tmp, len);
    return *this;
}
bs_string& bs_string::append( const bs_string& rhs)
{
    if (!rhs.length())
        return *this;

    bs_utf16::append(rhs);
    return *this;
}
bs_string& bs_string::append( const std::string& rhs)
{
    return append(rhs.data(), rhs.length());
}
bs_string& bs_string::append( const std::wstring& rhs)
{
    return append(rhs.data(), rhs.length());
}

/////////////////////////insert////////////////////////////////
bs_string& bs_string::insert(int pos,  const char*str, int len)
{
    if (pos < 0 || !str || !(*str))
        return *this;
    if (len == -1)
        len = strlen(str);
    uint16 tmp[len+1] ;
    ::from_latin1(str, len, tmp);
    bs_utf16::insert(pos, tmp, len);
    return *this;
}

bs_string& bs_string::insert(int pos,  const bs_char*str, int len)
{
    if (pos < 0 || len <= 0 || !str)
        return *this;

    bs_utf16::insert(pos, (uint16*)str, len);
    return *this;
}

bs_string& bs_string::insert(int pos,  const wchar_t*str, int len)
{
    if (len == -1)
        len = wcslen(str);

    uint16 tmp[len+1] ;
    if (!bsHasUTF32)
        ::from_utf16(reinterpret_cast<const uint16 *>(str), len, tmp);
    else
        ::from_ucs4(reinterpret_cast<const uint *>(str), len, tmp);

    return insert(pos, (bs_char*)tmp, len);
}

bs_string& bs_string::insert(int pos,  const bs_string& str)
{
    return insert(pos, str.data(), str.length());
}

bs_string& bs_string::insert(int pos,  const std::string&str)
{
    return insert(pos, str.data(), str.length());
}

bs_string& bs_string::insert(int pos,  const std::wstring& str)
{
    return insert(pos, str.data(), str.length());
}

///////////////////////remove//////////////////////////////////
bs_string& bs_string::remove(int pos, int len)
{
    bs_utf16::remove(pos, len);
    return *this;
}
bs_string& bs_string::remove(const char*str, int len)
{
    if (str != NULL)
    {
        if (len == -1)
            len = strlen(str);
        int i = find(str);
        if (i >= 0)
            remove(i, len);
    }
    return *this;
}
bs_string& bs_string::remove(const bs_char* str, int len)
{
    if (str != NULL)
    {
        int i = find(bs_string(str, len));
        if (i >= 0)
            remove(i, len);
    }
    return *this;
}
bs_string& bs_string::remove(const wchar_t*str, int len)
{
    if (str)
    {
        if (len == -1)
            len = wcslen(str);
        int i = find(str);
        if (i >= 0)
            remove(i, len);
    }
    return *this;
}
bs_string& bs_string::remove(const bs_string &str)
{
    if (str.length())
    {
        int i = find(str);
        if (i >= 0)
            remove(i, str.length());
    }
    return *this;
}
bs_string& bs_string::remove(const std::string &str)
{
    return remove(str.data(), str.length());
}
bs_string& bs_string::remove(const std::wstring &str)
{
    return remove(str.data(), str.length());
}

////////////////////////////replace////////////////////////////////////
int bs_string::replace( int pos, int len , const bs_string &str)
{
    bs_utf16::replace(pos, len, str);
    return contains(str) ? pos : -1;
}
int bs_string::replace( const char* token1, const char* token2 )
{
    int i = find(token1);
    if (i >= 0)
        return replace(i, strlen(token1), bs_string(token2));
    return -1;
}

int bs_string::replace( const wchar_t* token1, const wchar_t* token2 )
{
    int i = find(token1);
    if (i >= 0)
        return replace(i, wcslen(token1), bs_string(token2));
    return -1;
}
int bs_string::replace( const bs_string &token1, const bs_string &token2 )
{
    int i = find(token1);
    if (i >= 0)
        return replace(i, token1.length(), token2);
    return -1;
}
int bs_string::replace( const std::string &token1, const std::string &token2 )
{
    int i = find(token1);
    if (i >= 0)
        return replace(i, token1.length(), bs_string(token2));
    return -1;
}
int bs_string::replace( const std::wstring &token1, const std::wstring &token2 )
{
    int i = find(token1);
    if (i >= 0)
        return replace(i, token1.length(), bs_string(token2));
    return -1;
}
/////////////////////////extract////////////////////////////////
//! count 负数时根据pos位置向前取，此时顺序不为逆
bs_string bs_string::extract( int pos, int count)const
{
    return bs_utf16::extract(pos, count);
}
//! 根据pos位置将后面的全部提取
bs_string bs_string::extract( int pos )const
{
    return  bs_utf16::extract(pos);
}
//! 从开头提取count个字符
bs_string bs_string::start( int count)const
{
    return bs_utf16::start(count);
}
//! 从结尾提取count个字符，此时不为逆
bs_string bs_string::end( int count)const
{
    return bs_utf16::end(count);
}
bs_string bs_string::trimmed()const
{
    const bs_char *dh = data();
    bs_char *dc = (bs_char *)dh;
    bs_char *de = dc + count();
    while ((dc != de) && dc->is_space ())
        dc++;
    dh = dc;
    while ((dh != de) && !dh->is_space ())
        dh++;

    return bs_string(dc, dh - dc);
}

/////////////////////////find/////////////////////////////////////////
int bs_string::find(const char* str) const
{
    return find(bs_string(str));
}
int bs_string::find(const wchar_t*str) const
{
    return find(bs_string(str));
}
int bs_string::find(const bs_string& str) const
{
    return findString((bs_char*)data(),length(), (bs_char*)str.data (), str.length ());
}
int bs_string::find(const std::string &str) const
{
    return find(bs_string(str));
}
int bs_string::find(const std::wstring &str) const
{
    return find(bs_string(str));
}

//////////////////////////////////////////////////////////////
bs_string_list bs_string::split(const char*str)const
{
    return split(bs_string(str));
}
bs_string_list bs_string::split(const wchar_t*str)const
{
    return split(bs_string(str));
}

bs_string_list bs_string::split(const bs_string&rhs)const
{
    bs_string_list substr;
    int i = 0;
    int back_i = 0;
    int len = length();
    while ((i = findString((bs_char*)data()+back_i, len-back_i, (bs_char*)rhs.data(), rhs.length())) != -1)
    {
        substr.append(bs_string(data()+back_i, i));

        back_i += i + rhs.length ();
    }
    substr.append(bs_string(data()+back_i, len - back_i));
    return substr;
}
bs_string_list bs_string::split(const std::string &str)const
{
    return split(bs_string(str));
}
bs_string_list bs_string::split(const std::wstring &str)const
{
    return split(bs_string(str));
}

///////////////////////split_path//////////////////////////////

///////////////////////////compare////////////////////////
int bs_string::compare(const char*str)const
{
    return compare(bs_string(str));
}
int bs_string::compare(const wchar_t*str)const
{
    return compare(bs_string(str));
}
int bs_string::compare(const bs_string&str)const
{
    return compare_helper(data(), length(), str.data(), str.length());
}
int bs_string::compare(const std::string &str)const
{
    return compare(bs_string(str));
}
int bs_string::compare(const std::wstring &str)const
{
    return compare(bs_string(str));
}

#include <stdarg.h>
//////////////////////////////////////////////////////
#define MAX_MSG_LENGTH 2048
bs_string& bs_string::format( const char*sFormat, ...)
{
    char sMessage[MAX_MSG_LENGTH];
    va_list args;

    /* put formatted message in sMessage */
    bs_memory::set_c(sMessage,'\0',sizeof(sMessage));
    if (sFormat)
    {
        va_start(args, sFormat);
        vsnprintf(sMessage, MAX_MSG_LENGTH-1, sFormat, args);
        sMessage[MAX_MSG_LENGTH-1] = '\0';
        va_end(args);
    }

    return this->append(sMessage, strlen(sMessage));
}
bs_string& bs_string::format( const wchar_t*sFormat, ...)
{
    wchar_t sMessage[MAX_MSG_LENGTH];
    va_list args;

    /* put formatted message in sMessage */
    bs_memory::set_c(sMessage,'\0',sizeof(sMessage));
    if (sFormat) {
        va_start(args, sFormat);
        vswprintf(sMessage, MAX_MSG_LENGTH-1, sFormat, args);
        sMessage[MAX_MSG_LENGTH-1] = '\0';
        va_end(args);
    }
    return this->append(sMessage, wcslen(sMessage));
}
bs_string bs_string::formats( const char*sFormat, ...)
{
    char sMessage[MAX_MSG_LENGTH];
    va_list args;

    /* put formatted message in sMessage */
    bs_memory::set_c(sMessage, '\0', sizeof(sMessage));
    if (sFormat)
    {
        va_start(args, sFormat);
        vsnprintf(sMessage, MAX_MSG_LENGTH-1, sFormat, args);
        sMessage[MAX_MSG_LENGTH-1] = '\0';
        va_end(args);
    }
    return bs_string(sMessage);
}
bs_string bs_string::formats( const wchar_t*sFormat, ...)
{
    wchar_t sMessage[MAX_MSG_LENGTH] = {'\0'};
    va_list args;

    /* put formatted message in sMessage */
    bs_memory::set_c(sMessage, '\0',sizeof(sMessage));
    if (sFormat) {
        va_start(args, sFormat);
        vswprintf(sMessage, MAX_MSG_LENGTH-1, sFormat, args);
        sMessage[MAX_MSG_LENGTH-1] = '\0';
        va_end(args);
    }
    return bs_string(sMessage);
}
#undef MAX_MSG_LENGTH
/////////////////////////////////////////
bs_string& bs_string::operator=(const char*str)
{
    *this = bs_string(str, -1);
    return *this;

}
bs_string& bs_string::operator=(const wchar_t* str)
{
    *this = bs_string(str, -1);
    return *this;
}

bs_string& bs_string::operator=(const bs_string& str)
{
    bs_utf16::operator =(str);
    return *this;
}
bs_string& bs_string::operator=(const std::string &str)
{
    *this = bs_string(str.data(), str.length());

    return *this;
}
bs_string& bs_string::operator=(const std::wstring &str)
{
    *this = bs_string(str.data(), str.length());
    return *this;
}
///////////////////////////////////////////////////////

bs_string bs_string::operator+(const char*str) const
{
    return *this + bs_string(str);
}
bs_string bs_string::operator+(const wchar_t*str) const
{
    return *this + bs_string(str);
}

bs_string bs_string::operator+(const bs_string&rhs) const
{
    bs_string out(*this);
    out.append(rhs);
    return out;
}
bs_string bs_string::operator+(const std::string&str) const
{
    return *this + bs_string(str);
}
bs_string bs_string::operator+(const std::wstring&str) const
{
    return *this + bs_string(str);
}
bool bs_string::operator < (const bs_string&str)const
{
    return this->compare(str) < 0;
}
bool bs_string::operator > (const bs_string&str)const
{
    return this->compare(str) > 0;
}
////////////////////////to_//////////////////////////////////////////////
char* bs_string::to_latin1()const
{
    static bs_array<char> sTmpLatinl;
    sTmpLatinl.resize(this->length());
    ::to_latin1((uchar*)sTmpLatinl.data(), (uint16*)this->data(), this->length());
    return sTmpLatinl.data();
}
wchar_t* bs_string::to_wchar()const
{
    static bs_array<wchar_t> sTmpWchar;
    sTmpWchar.resize(this->length());

    if (bsHasUTF32)
        ::to_wchar(sTmpWchar.data(), (uint16*)this->data(), this->length());
    else
        bs_memory::copy_sse(sTmpWchar.data(), this->data(), this->bytecount());

    return sTmpWchar.data();
}
bs_utf8 bs_string::to_utf8()const
{
    return to_utf8s((uint16*)this->data(), length());
}
bs_utf16 bs_string::to_utf16()const
{
    return *this;
}
bs_utf32 bs_string::to_utf32()const
{
    return to_utf32s((uint16*)this->data(), length());
}
bs_byte bs_string::to_byte()const
{
    bs_byte outs;
    outs.resize(sizeOf * length());
    memcpy(outs.data(), this->data(), length()*sizeOf);
    return outs;
}
void bs_string::upper()
{
    bs_utf16::detach();
    for (size_t i = 0; i < length(); ++i)
        this->data()[i].upper();
}
void bs_string::lower()
{
    bs_utf16::detach();
    for (size_t i = 0; i < length(); ++i)
        this->data()[i].lower();
}
void bs_string::reverse()
{
    bs_utf16::detach();
    for (size_t i = 0; i < length(); ++i)
        this->data()[i].reverse();
}
//////////////////////////////////////////////////////////////////
bool bs_string::form_latin1(const char*str)
{
    *this = bs_string(str, -1);
    return true;
}
bool bs_string::form_wchar(const wchar_t*str)
{
    *this = bs_string(str, -1);
    return true;
}
bool bs_string::form_utf8(const uint8 *utf8, int len)
{
    uint16 utf16[len+1];
    size_t rc = 0;
    uint error_status = 0;
    uint error_mask = 0xFFFFFFFF;
    uint error_code_point = 0xFFFD;
    const uchar* p1 = 0;
    if (!utf8 || len < 1)
        return false;

    rc = unicode::convert_UTF8_UTF16(false, utf8, len, utf16, len,
                                &error_status, error_mask, error_code_point, &p1);

    if (error_status)
        return false;

    if (rc > 0 && rc <= (uint)len)
        utf16[rc] = 0;
    else
    {
        utf16[len-1] = 0;
        rc = 0;
        return false;
    }
    *this = bs_string((bs_char*)utf16, rc);
    return true;
}
bool bs_string::form_utf16(const uint16*utf16, int len)
{
    *this = bs_string((bs_char*)utf16, len);
    return true;
}
bool bs_string::form_utf32(const uint32*utf32, int len )
{
    int utf16len = len*(sizeof(uint)/ sizeof(uint16));
    uint16 utf16[utf16len+1];
    size_t rc = 0;
    uint error_status = 0;
    uint error_mask = 0xFFFFFFFF;
    uint error_code_point = 0xFFFD;
    const uint* p1 = 0;
    if (!utf32)
        return false;

    rc = unicode::convert_UTF32_UTF16(false, utf32, len, utf16, utf16len,
                                &error_status, error_mask, error_code_point, &p1);

    if ( error_status )
        return false;

    if ( rc > 0 && rc <= (uint)utf16len )
        utf16[rc] = 0;
    else
    {
        utf16[len-1] = 0;
        rc = 0;
        return false;
    }
    *this = bs_string((bs_char*)utf16, len);
    return true;
}

/////////////////////////////////////////////////////////////////
bs_utf8 bs_string::to_utf8s(uint16* utf16, int len)
{
    static uchar tempbuf[6] = {0};
    int f16_byte = 0;
    if (len <= 0)
        return bs_utf8();
    for (int i = 0; i < len; ++i)
    {
        int bytes = unicode::encode_UTF8(utf16[i], tempbuf);
        if (f16_byte < bytes)
            f16_byte = bytes;
    }

    static bs_utf8 oututf8;
    oututf8.resize(len * f16_byte, '\0');
    size_t rc = 0;
    uint error_status = 0;
    uint error_mask = 0xFFFFFFFF;
    uint error_code_point = 0xFFFD;
    const uint16* p1 = 0;

    if (!utf16)
        return bs_utf8();

    rc = unicode::convert_UTF16_UTF8(false, utf16, len, oututf8.data(), oututf8.size(),
                                &error_status, error_mask, error_code_point, &p1);

    if ( error_status )
        return bs_utf8();

    if ( rc > 0 && rc <= oututf8.size())
        return oututf8;
    return bs_utf8();
}
bs_utf8 bs_string::to_utf8s(uint32* utf32, int len)
{
    static uchar tempbuf[6] = {0};
    int f16_byte = 0;
    if (len <= 0)
        return bs_utf8();
    for (int i = 0; i < len; ++i)
    {
        int bytes = unicode::encode_UTF8(utf32[i], tempbuf);
        if (f16_byte < bytes)
            f16_byte = bytes;
    }
    static bs_utf8 oututf8;
    oututf8.resize(len*f16_byte, '\0');
    size_t rc = 0;
    uint error_status = 0;
    uint error_mask = 0xFFFFFFFF;
    uint error_code_point = 0xFFFD;
    const uint* p1 = 0;
    if (!utf32)
        return bs_utf8();

    rc = unicode::convert_UTF32_UTF8(false, utf32, len, oututf8.data(), oututf8.size(),
                                &error_status, error_mask, error_code_point, &p1);

    if ( error_status )
        return bs_utf8();

    if ( rc > 0 && rc <= (uint)oututf8.size() )
        return oututf8;
    return bs_utf8();
}
bs_utf16 bs_string::to_utf16s(uint8 *utf8, int len)
{
    static uint16 tempbuf[2] = {0};
    int f16_byte = 0;
    if (len <= 0)
        return bs_utf16();
    for (int i = 0; i < len; ++i)
    {
        int bytes = unicode::encode_UTF16(utf8[i], tempbuf);
        if (f16_byte < bytes)
            f16_byte = bytes;
    }
    static bs_utf16 oututf16;
    oututf16.resize(len*f16_byte, '\0');
    size_t rc = 0;
    uint error_status = 0;
    uint error_mask = 0xFFFFFFFF;
    uint error_code_point = 0xFFFD;
    const uchar* p1 = 0;
    if (!utf8)
        return bs_utf16();

    rc = unicode::convert_UTF8_UTF16(false, utf8, len, oututf16.data(), oututf16.size(),
                                &error_status, error_mask, error_code_point, &p1);

    if ( error_status )
        return bs_utf16();

    if ( rc > 0 && rc <= (uint)oututf16.size() )
        return oututf16;
    return bs_utf16();
}

bs_utf16 bs_string::to_utf16s(uint32* utf32, int len)
{
    static uint16 tempbuf[2] = {0};
    int f16_byte = 0;
    if (len <= 0)
        return bs_utf16();
    for (int i = 0; i < len; ++i)
    {
        int bytes = unicode::encode_UTF16(utf32[i], tempbuf);
        if (f16_byte < bytes)
            f16_byte = bytes;
    }
    static bs_utf16 oututf16;
    oututf16.resize(len*f16_byte, '\0');
    size_t rc = 0;
    uint error_status = 0;
    uint error_mask = 0xFFFFFFFF;
    uint error_code_point = 0xFFFD;
    const uint* p1 = 0;
    if (!utf32)
        return bs_utf16();

    rc = unicode::convert_UTF32_UTF16(false, utf32, len, oututf16.data(), oututf16.size(),
                                &error_status, error_mask, error_code_point, &p1);

    if ( error_status )
        return bs_utf16();

    if ( rc > 0 && rc <= (uint)oututf16.size() )
        return oututf16;
    return bs_utf16();
}
bs_utf32 bs_string::to_utf32s(uint8 *utf8, int len)
{
    static bs_utf32 oututf32;
    if (len <= 0)
        return bs_utf32();
    oututf32.resize(len, '\0');
    size_t rc = 0;
    uint error_status = 0;
    uint error_mask = 0xFFFFFFFF;
    uint error_code_point = 0xFFFD;
    const uchar* p1 = 0;
    if (!utf8)
        return bs_utf32();

    rc = unicode::convert_UTF8_UTF32(false, utf8, len, oututf32.data(), oututf32.size(),
                                &error_status, error_mask, error_code_point, &p1);

    if ( error_status )
        return bs_utf32();

    if ( rc > 0 && rc <= (uint)oututf32.size() )
        return oututf32;
    return bs_utf32();
}
bs_utf32 bs_string::to_utf32s(uint16* utf16, int len)
{
    static bs_utf32 oututf32;
    if (len <= 0)
        return bs_utf32();
    oututf32.resize(len, '\0');
    size_t rc = 0;
    uint error_status = 0;
    uint error_mask = 0xFFFFFFFF;
    uint error_code_point = 0xFFFD;
    const uint16* p1 = 0;
    if (!utf16)
        return bs_utf32();

    rc = unicode::convert_UTF16_UTF32(false, utf16, len, oututf32.data(), oututf32.size(),
                                &error_status, error_mask, error_code_point, &p1);

    if ( error_status )
        return bs_utf32();

    if ( rc > 0 && rc <= (uint)oututf32.size() )
        return oututf32;
    return bs_utf32();
}

/////////////////////////////////////////////////////////
bs_string &bs_string::form(bool value, int radix )
{
    append(longlong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(char value, int radix )
{
    append(longlong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(uchar value, int radix )
{
    append(ulonglong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(short value, int radix )
{
    append(longlong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(uint16 value, int radix )
{
    append(ulonglong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(int value, int radix )
{
    append(longlong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(uint value, int radix )
{
    append(ulonglong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(long value, int radix )
{
    append(longlong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(ulong value, int radix )
{
    append(ulonglong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(longlong value, int radix )
{
    append(longlong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(ulonglong value, int radix )
{
    append(ulonglong_string(value, radix));
    return *this;
}
bs_string &bs_string::form(float value, int prec)
{
    return form(real(value), prec);
}
bs_string &bs_string::form(real value,  int prec)
{
    int decpt = 0, sign = 0, slen = 0;
    char *retfll = NULL;
    double_string(value, 0, prec, &decpt, &sign, NULL, &retfll, &slen);

    bs_string temp(retfll, slen);
    bsFree (retfll);
    if (decpt != slen)
    {
        if (decpt != 0)
            temp.insert(decpt, ".");
        else
            temp.insert(0, "0.");
    }

    if (sign != 0 )
        temp.insert(0, "-");
    append (temp);
    return *this;
}
bool      bs_string::to_bool(int radix )
{
    return toIntegral_helper<bool>((char*)to_utf8().data(), length(), radix);
}
char      bs_string::to_char(int radix )
{
    return toIntegral_helper<char>((char*)to_utf8().data(), length(), radix);
}
uchar     bs_string::to_uchar(int radix )
{
    return toUIntegral_helper<uchar>((char*)to_utf8().data(), length(), radix);
}
short     bs_string::to_short(int radix )
{
    return toIntegral_helper<short>((char*)to_utf8().data(), length(), radix);
}
ushort    bs_string::to_ushort(int radix )
{
    return toUIntegral_helper<ushort>((char*)to_utf8().data(), length(), radix);
}
int       bs_string::to_int(int radix )
{
    return toIntegral_helper<int>((char*)to_utf8().data(), length(), radix);
}
uint      bs_string::to_uint(int radix )
{
    return toUIntegral_helper<uint>((char*)to_utf8().data(), length(), radix);
}
long      bs_string::to_long(int radix )
{
    return toIntegral_helper<long>((char*)to_utf8().data(), length(), radix);
}
ulong     bs_string::to_ulong(int radix )
{
    return toUIntegral_helper<ulong>((char*)to_utf8().data(), length(), radix);
}
longlong  bs_string::to_longlong(int radix )
{
    return toIntegral_helper<longlong>((char*)to_utf8().data(), length(), radix);
}
ulonglong bs_string::to_ulonglong(int radix )
{
    return toUIntegral_helper<ulonglong>((char*)to_utf8().data(), length(), radix);
}
float     bs_string::to_float()
{
    return string_double((char*)to_utf8().data(), NULL);
}
real      bs_string::to_real()
{
    return string_double((char*)to_utf8().data(), NULL);
}

// 字符串比较
/*bool bs_string::operator==(const bs_string& rhs) const
{
    if (length() != rhs.length())
        return false;
    return memEquals(dptr->data, rhs.dptr->data, rhs.length());
}*/

