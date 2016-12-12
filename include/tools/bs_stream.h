#ifndef bs_bstream_H
#define bs_bstream_H

#include "bs_vector.h"
#include "bs_memory.h"
#include "bs_array.h"
#include "bs_string.h"

typedef eArchEndians eByteOrders;
struct tStreamDatas
{
    eByteOrders border;
    bs_byte *array;

    virtual void put(char* val, size_t len) = 0;
    virtual char *take(int *len = NULL) = 0;

    virtual void dump() {}
    virtual char split()const{return 0;}

    template <typename T>
    T swap_endian(T &v);
};

struct tStreamBinarys : tStreamDatas
{
    virtual void put(char* val, size_t len);
    virtual char *take(int *len = NULL);

    virtual void dump();
};
struct tStreamTexts : tStreamDatas
{
    virtual char *take(int *len = NULL);
    virtual void put(char* val, size_t len);
    virtual void dump() ;
    virtual char split()const;
};

/*!
 * @brief The bs_stream class
 * @class bs_stream
 */
template <typename OpeT>
class bs_stream : public OpeT
{
public :
    explicit bs_stream(const bs_byte &);
    explicit bs_stream(bs_byte *);

    virtual ~bs_stream();
    size_t size()const;

public:
    friend bs_stream & operator << (bs_stream &, const bs_stream &);
    friend bs_stream & operator >> (bs_stream &, bs_stream &);
    friend bs_stream & operator << (bs_stream &, const bs_byte &);
    friend bs_stream & operator >> (bs_stream &, bs_byte &);

protected:
    tStreamDatas *impl;
};

#include "bs_stream.inl"
class bs_bstream : public bs_stream<tStreamBinarys>
{
public:
    bs_bstream();
    explicit bs_bstream(const bs_byte &);
    explicit bs_bstream(bs_byte *);

public:
    friend bs_bstream & operator << (bs_bstream &, bool);
    friend bs_bstream & operator << (bs_bstream &, char);
    friend bs_bstream & operator << (bs_bstream &, uchar);
    friend bs_bstream & operator << (bs_bstream &, short);
    friend bs_bstream & operator << (bs_bstream &, ushort);
    friend bs_bstream & operator << (bs_bstream &, int);
    friend bs_bstream & operator << (bs_bstream &, uint);
    friend bs_bstream & operator << (bs_bstream &, long);
    friend bs_bstream & operator << (bs_bstream &, ulong);
    friend bs_bstream & operator << (bs_bstream &, longlong);
    friend bs_bstream & operator << (bs_bstream &, ulonglong);
    friend bs_bstream & operator << (bs_bstream &, float);
    friend bs_bstream & operator << (bs_bstream &, real);
    friend bs_bstream & operator << (bs_bstream &, lreal);

    friend bs_bstream & operator >> (bs_bstream &, bool & );
    friend bs_bstream & operator >> (bs_bstream &, char & );
    friend bs_bstream & operator >> (bs_bstream &, uchar & );
    friend bs_bstream & operator >> (bs_bstream &, short & );
    friend bs_bstream & operator >> (bs_bstream &, ushort & );
    friend bs_bstream & operator >> (bs_bstream &, int & );
    friend bs_bstream & operator >> (bs_bstream &, uint & );
    friend bs_bstream & operator >> (bs_bstream &, long & );
    friend bs_bstream & operator >> (bs_bstream &, ulong & );
    friend bs_bstream & operator >> (bs_bstream &, longlong & );
    friend bs_bstream & operator >> (bs_bstream &, ulonglong & );
    friend bs_bstream & operator >> (bs_bstream &, float & );
    friend bs_bstream & operator >> (bs_bstream &, real & );
    friend bs_bstream & operator >> (bs_bstream &, lreal & );

    friend bs_bstream & operator << (bs_bstream &in, const bs_bstream &v);
    friend bs_bstream & operator >> (bs_bstream &out, bs_bstream &v);
    friend bs_bstream & operator << (bs_bstream &, const bs_byte &);
    friend bs_bstream & operator >> (bs_bstream &, bs_byte &);
    friend bs_bstream & operator << (bs_bstream &in, const bs_string &v);
    friend bs_bstream & operator >> (bs_bstream &out, bs_string &v);


};

class bs_tstream : public bs_stream<tStreamTexts>
{
public:
    explicit bs_tstream(const bs_byte &);
    explicit bs_tstream(bs_byte *);
public:
    friend bs_tstream & operator << (bs_tstream &, bool);
    friend bs_tstream & operator << (bs_tstream &, char);
    friend bs_tstream & operator << (bs_tstream &, uchar);
    friend bs_tstream & operator << (bs_tstream &, short);
    friend bs_tstream & operator << (bs_tstream &, ushort);
    friend bs_tstream & operator << (bs_tstream &, int);
    friend bs_tstream & operator << (bs_tstream &, uint);
    friend bs_tstream & operator << (bs_tstream &, long);
    friend bs_tstream & operator << (bs_tstream &, ulong);
    friend bs_tstream & operator << (bs_tstream &, longlong);
    friend bs_tstream & operator << (bs_tstream &, ulonglong);
    friend bs_tstream & operator << (bs_tstream &, float);
    friend bs_tstream & operator << (bs_tstream &, real);
    friend bs_tstream & operator << (bs_tstream &, lreal);

    friend bs_tstream & operator << (bs_tstream &, char *);
    friend bs_tstream & operator << (bs_tstream &, const char *);

    friend bs_tstream & operator >> (bs_tstream &, bool & );
    friend bs_tstream & operator >> (bs_tstream &, char & );
    friend bs_tstream & operator >> (bs_tstream &, uchar & );
    friend bs_tstream & operator >> (bs_tstream &, short & );
    friend bs_tstream & operator >> (bs_tstream &, ushort & );
    friend bs_tstream & operator >> (bs_tstream &, int & );
    friend bs_tstream & operator >> (bs_tstream &, uint & );
    friend bs_tstream & operator >> (bs_tstream &, long & );
    friend bs_tstream & operator >> (bs_tstream &, ulong & );
    friend bs_tstream & operator >> (bs_tstream &, longlong & );
    friend bs_tstream & operator >> (bs_tstream &, ulonglong & );
    friend bs_tstream & operator >> (bs_tstream &, float & );
    friend bs_tstream & operator >> (bs_tstream &, real & );
    friend bs_tstream & operator >> (bs_tstream &, lreal & );

    friend bs_tstream & operator << (bs_tstream &in, const bs_tstream &v);
    friend bs_tstream & operator >> (bs_tstream &out, bs_tstream &v);
    friend bs_tstream & operator << (bs_tstream &, const bs_byte &);
    friend bs_tstream & operator >> (bs_tstream &, bs_byte &);
    friend bs_tstream & operator << (bs_tstream &, const bs_string &);
    friend bs_tstream & operator >> (bs_tstream &, bs_string &);
private:
    void numput(longlong val, int bs = 10);
    void unumput(ulonglong val, int bs = 10);
    void fltput(real val, int prec = 4);

    template<typename T>
    T unumtake(int bs = 10);
    template<typename T>
    T numtake(int bs = 10);
};

#endif // bs_bstream_H
