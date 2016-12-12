#include "bs_stream.h"
#include "interface/iio.h"
#include "bs_array.h"
#include "bs_string.h"
#include "bs_number_double.h"
#include "bs_number_longlong.h"

#if 0
#define swap16(x) \
    (((x) & 0x00ff) << 8 | \
    ( (x) & 0xff00) >> 8)

#define swap32(x) \
    (((x) & 0x000000ff) << 24 | \
    ( (x) & 0x0000ff00) << 8 | \
    ( (x) & 0x00ff0000) >> 8 | \
    ( (x) & 0xff000000) >> 24)

#define swap64(x) \
    ((((x) & 0x00000000000000ff) << 56) | \
    (( (x) & 0x000000000000ff00) << 40) | \
    (( (x) & 0x0000000000ff0000) << 24) | \
    (( (x) & 0x00000000ff000000) << 8)  | \
    (( (x) & 0x000000ff00000000) >> 8)  | \
    (( (x) & 0x0000ff0000000000) >> 24) | \
    (( (x) & 0x00ff000000000000) >> 40) | \
    (( (x) & 0xff00000000000000) >> 56))
#else
static inline char* swap16(char *x)
{
    char tmp = x[0];
    x[0] = x[1];
    x[1] = tmp;
    return x;
}
static inline char* swap32(char *x)
{
    char tmp = x[0];
    char tmp1 = x[1];
    x[0] = x[3];
    x[1] = x[2];
    x[2] = tmp1;
    x[3] = tmp;
    return x;
}
static inline char* swap64(char *x)
{
    char tmp = x[0];
    char tmp1 = x[1];
    char tmp2 = x[2];
    char tmp3 = x[3];
    x[0] = x[7];
    x[1] = x[6];
    x[2] = x[5];
    x[3] = x[4];
    x[4] = tmp3;
    x[5] = tmp2;
    x[6] = tmp1;
    x[7] = tmp;
    return x;
    return x;
}
#endif
template <typename T>
T tStreamDatas::swap_endian(T &v)
{
    union {
        T val;
        char bit[sizeof(T)] ;
    } swdat;
    swdat.val = v;
    if (sizeof(T) == 2)
        swap16(swdat.bit);
    else if (sizeof(T) == 4)
        swap32(swdat.bit);
    else if (sizeof(T) == 8)
        swap64(swdat.bit);
    v = swdat.val;
    return v;
}
void tStreamBinarys::put(char* val, size_t len)
{
    if (array)
        array->append((bs_byte::array_t*)val, len);
}
char *tStreamBinarys::take(int *len)
{
    static char buf[68] = {0};
    if (!len || *len > 68)
        return NULL;

    if (array)
    {
        bs_memory::copy_sse(buf, array->data(), *len);
        array->remove(0, *len);
    }
    return buf;
}
void tStreamBinarys::dump()
{

}
char tStreamTexts::split()const{return 0xff;}
void tStreamTexts::put(char *val, size_t len)
{
    if (array)
    {
        array->append((uint8*)val, len);
        array->append(split());
    }
}
char *tStreamTexts::take(int *len)
{
    static char buf[68] = {0};
    if (!len || *len > 68)
        return NULL;
    if (array)
    {
        char *dat = (char*)array->data();
        for (size_t i = 0; i < array->size(), i < 68; ++i)
        {
            if (dat[i] != split())
                buf[i] = dat[i];
            else
            {
                array->remove(0, i);
                buf[i] = '\0';
                if (len)
                    *len = i;
                break;
            }
        }
    }
    return buf;
}
void tStreamTexts::dump()
{
    bool is_ntp = false;
    if (array)
    {
        for (size_t i = 0 ; i < array->size(); ++i)
        {
            if ((char)(*array)[i] == '%')
                is_ntp = true;
            else if (is_ntp)
                is_ntp = false;
            else if ((char)(*array)[i] != split())
                ::putchar((char)(*array)[i]);
        }
    }
}


bs_bstream::bs_bstream(const bs_byte &ay):
    bs_stream<tStreamBinarys>(ay)
{
}
bs_bstream::bs_bstream(bs_byte *ay):
    bs_stream<tStreamBinarys>(ay)
{
}

bs_tstream::bs_tstream(const bs_byte &v):
    bs_stream<tStreamTexts>(v)
{
}
bs_tstream::bs_tstream(bs_byte *v):
    bs_stream<tStreamTexts>(v)
{
}

bs_bstream & operator << (bs_bstream &in, bool v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, char v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, uchar v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, short v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, ushort v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, int v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, uint v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, long v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, ulong v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, longlong v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, ulonglong v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, float v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, real v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}
bs_bstream & operator << (bs_bstream &in, lreal v)
{
    in.put((char*)&v, sizeof(v));
    return in;
}


bs_bstream & operator >> (bs_bstream &out, bool & v)
{
    int len = sizeof(v);
    v = *(bool*)out.take(&len);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  char& v)
{
    int len = sizeof(v);
    v = *(char*)out.take(&len);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  uchar& v)
{
    int len = sizeof(v);
    v = *(uchar*)out.take(&len);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  short& v)
{
    int len = sizeof(v);
    v = *(short*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  ushort& v)
{
    int len = sizeof(v);
    v = *(ushort*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  int& v)
{
    int len = sizeof(v);
    v = *(int*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  uint& v)
{
    int len = sizeof(v);
    v = *(uint*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  long& v)
{
    int len = sizeof(v);
    v = *(long*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  ulong& v)
{
    int len = sizeof(v);
    v = *(ulong*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  longlong& v)
{
    int len = sizeof(v);
    v = *(longlong*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  ulonglong& v)
{
    int len = sizeof(v);
    v = *(ulonglong*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  float& v)
{
    int len = sizeof(v);
    v = *(float*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  real& v)
{
    int len = sizeof(v);
    v = *(real*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream & operator >> (bs_bstream &out,  lreal& v)
{
    int len = sizeof(v);
    v = *(lreal*)out.take(&len);
    if (out.border != bsArchEndian)
        out.swap_endian(v);
    return out;
}
bs_bstream &operator << (bs_bstream &in, const bs_string &v)
{
    in << bs_byte((uint8*)v.data(), sizeof(uint16)*v.size());
    return in;
}
bs_bstream &operator >> (bs_bstream &out, bs_string &v)
{
    bs_byte tmp;
    out >> tmp;
    v = bs_string((bs_char*)tmp.data(), tmp.size()/sizeof(uint16));
    return out;
}


void bs_tstream::numput(longlong val, int bs)
{
    char *str = longlong_string(val, bs);
    put(str, strlen(str));
}
void bs_tstream::unumput(ulonglong val, int bs )
{
    char *str = ulonglong_string(val, bs);
    put(str, strlen(str));
}
void bs_tstream::fltput(real val, int prec)
{

    int decpt = 0, sign = 0, slen = 0;
    bs_array<char> refs;
    char *dadd = NULL;
    double_string(val, 0, prec, &decpt, &sign, NULL, &dadd, &slen);

    refs.append(dadd, slen);
    bsFree (dadd);

    if (decpt != slen)
    {
        if (decpt != 0)
            refs.insert(decpt, '.');
        else
            refs.insert(0, "0.", 2);
    }

    if (sign != 0 )
        refs.insert(0, '-');
    put (refs.data(), strlen(refs.data()));
}

template<typename T>
T bs_tstream::unumtake(int bs)
{
    char *str = take();
    return toIntegral_helper<T>(str, strlen(str), bs);
}
template<typename T>
T bs_tstream::numtake(int bs )
{
    char *str = take();
    return toUIntegral_helper<T>(str, strlen(str), bs);
}

bs_tstream & operator << (bs_tstream&in, bool v)
{
    in.numput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, char v)
{
    in.put(&v, 1);
    return in;
}
bs_tstream & operator << (bs_tstream&in, uchar v)
{
    in.unumput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, short v)
{
    in.numput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, ushort v)
{
    in.unumput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, int v)
{
    in.numput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, uint v)
{
    in.unumput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, long v)
{
    in.numput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, ulong v)
{
    in.unumput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, longlong v)
{
    in.numput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, ulonglong v)
{
    in.unumput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, float v)
{
    in.fltput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, real v)
{
    in.fltput(v);
    return in;
}
bs_tstream & operator << (bs_tstream&in, lreal v)
{
    in.fltput(v);
    return in;
}

bs_tstream & operator << (bs_tstream&in, char *v)
{
    in.put(v, strlen(v));
    return in;
}
bs_tstream & operator << (bs_tstream&in, const char *v)
{
    in.put((char*)v, strlen(v));
    return in;
}

bs_tstream & operator << (bs_tstream &in, const bs_string &v)
{
    char * tsp = "Unicode";
    in.put(tsp, strlen(tsp));
    in.put((char*)v.data(), v.bytecount());
    tsp = "Unicode";
    in.put(tsp, strlen(tsp));
    return in;
}
bs_tstream & operator >> (bs_tstream &out, bs_string &v)
{
    int len = 0;
    bs_string tsp = out.take(&len);
    if (bs_string("Unicode") == tsp)
    {
        if (out.array)
        {
            char *dat = (char*)out.array->data();
            for (size_t i = 0; i < out.array->size(); ++i)
            {
                if (bs_string("Unicode") == bs_string(&dat[i], strlen("Unicode")))
                {
                    v = bs_string(dat, i);
                    out.array->remove(0, i);
                    tsp = out.take(&len);
                    break;
                }
            }
        }
    }
    return out;
}

bs_tstream & operator >> (bs_tstream &out, bool &v)
{
    v = out.numtake<bool>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, char &v)
{
    v = *out.take();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, uchar &v)
{
    v = out.unumtake<uchar>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, short &v)
{
    v = out.numtake<short>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, ushort &v)
{
    v = out.unumtake<ushort>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, int &v)
{
    v = out.numtake<int>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, uint &v)
{
    v = out.unumtake<uint>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, long &v)
{
    v = out.numtake<long>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, ulong &v)
{
    v = out.unumtake<ulong>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, longlong &v)
{
    v = out.numtake<longlong>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, ulonglong &v)
{
    v = out.unumtake<ulonglong>();
    return out;
}
bs_tstream & operator >> (bs_tstream &out, float &v)
{
    int len = 68;
    char *str = out.take(&len);
    v = string_double(str, NULL);
    return out;
}
bs_tstream & operator >> (bs_tstream &out, real &v)
{
    int len = 68;
    char *str = out.take(&len);
    v = string_double(str, NULL);
    return out;
}
bs_tstream & operator >> (bs_tstream &out, lreal &v)
{
    int len = 68;
    char *str = out.take(&len);
    v = string_double(str, NULL);
    return out;
}



bs_bstream & operator << (bs_bstream &in, const bs_bstream &v)
{
    size_t vs = v.size();
    if (vs && v.impl)
    {
        in.put((char*)&vs, sizeof(size_t));
        char *tmpbuf = ((bs_bstream*)&v)->take((int*)&vs);
        in.put(tmpbuf, vs);
    }
    return in;
}
bs_bstream & operator >> (bs_bstream &out, bs_bstream &v)
{
    if (out.impl && out.size() && v.impl)
    {
        int len = sizeof(size_t);
        size_t vs = *(size_t*)out.take(&len);
        v.put((char*)&vs, len);

        char *vd = (char*)out.take((int*)&vs);
        v.put(vd, vs);
    }
    return out;
}
bs_bstream & operator << (bs_bstream &in, const bs_byte &v)
{
    size_t vs = v.size();
    in.put((char*)&vs, sizeof(size_t));
    in.put((char*)v.data(), vs);
    return in;
}
bs_bstream & operator >> (bs_bstream &out, bs_byte &v)
{
    if (out.impl && out.size())
    {
        int len = sizeof(size_t);
        len = *(size_t*) out.take(&len);
        char *buf = (char*)out.take(&len);
        v.append((bs_byte::array_t*)buf, len);
    }
    return out;
}

bs_tstream & operator << (bs_tstream &in, const bs_tstream &v)
{
    size_t vs = v.size();
    if (vs && v.impl)
    {
        in.put((char*)&vs, sizeof(size_t));
        char *tmpbuf = ((bs_tstream*)&v)->take((int*)&vs);
        in.put(tmpbuf, vs);
    }
    return in;
}
bs_tstream & operator >> (bs_tstream &out, bs_tstream &v)
{
    if (out.impl && out.size() && v.impl)
    {
        int len = sizeof(size_t);
        size_t vs = *(size_t*)out.take(&len);
        v.put((char*)&vs, len);

        char *vd = out.take((int*)&vs);
        v.put(vd, vs);
    }
    return out;
}
bs_tstream & operator << (bs_tstream &in, const bs_byte &v)
{
    size_t vs = v.size();
    in.put((char*)&vs, sizeof(size_t));
    in.put((char*)v.data(), vs);
    return in;
}
bs_tstream & operator >> (bs_tstream &out, bs_byte &v)
{
    if (out.impl && out.size())
    {
        int len = sizeof(size_t);
        len = *(size_t*) out.take(&len);
        char *buf = (char*)out.take(&len);
        v.append((bs_byte::array_t*)buf, len);
    }
    return out;
}
