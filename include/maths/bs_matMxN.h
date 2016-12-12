#ifndef MATMXN_H
#define MATMXN_H
#include "bs_vec4.h"
#include "tools/bs_memory.h"

template<typename T>
struct bs_matrixMxN
{
    T *elem;
    size_t cols;
    size_t rows;

    bs_matrixMxN()
    {
        elem == NULL;
        cols = 0;
        rows = 0;
    }
    bs_matrixMxN(size_t m, size_t n)
    {
        elem == NULL;
        cols = 0;
        rows = 0;
        create(m,n);
    }
    bs_matrixMxN(const bs_matrixMxN<T> &mn)
    {
        elem == NULL;
        cols = 0;
        rows = 0;
        *this = mn;
    }
    template<typename U>
    bs_matrixMxN(const bs_matrixMxN<U> &mn) {
        elem == NULL;
        cols = 0;
        rows = 0;
        *this = mn;
    }
    ~bs_matrixMxN(){
        destroy();
    }

    size_t size(){return cols * rows;}
    size_t bytecount(){return cols * rows * sizeof(T);}
    void create(size_t m, size_t n)
    {
        cols = m;
        rows = n;
        elem = (T*)bsMalloc(sizeof(T) *m*n);
        memset(elem, 0, sizeof(T) *m*n);
    }
    void destroy()
    {
        if (elem && cols)
            bsFree(elem);
        cols = 0;
        rows = 0;
        elem = NULL;
    }

    void resize(size_t m, size_t n)
    {
        destroy();
        create(m, n);
    }
    void clean()
    {
        memset(elem, 0, sizeof(T) *cols*rows);
    }

    bs_matrixMxN<T> &operator =(const T** mn)
    {
        for (size_t i = 0; i < cols; ++i)
            for (size_t j = 0; j < rows; ++j)
                elem[i*rows+j] = mn[i][j];
        return *this;
    }
    bs_matrixMxN<T> &operator =(const bs_matrixMxN<T> &mn)
    {
        destroy();
        create(mn.cols, mn.rows);
        bs_memory::copy_sse (elem, mn.elem, sizeof(T) *mn.cols*mn.rows);

        return *this;
    }
    template<typename U>
    bs_matrixMxN<T> &operator =(const bs_matrixMxN<U> &mn)
    {
        destroy();
        create(mn.cols, mn.rows);
        for (size_t i = 0; i < cols *rows; ++i)
                elem[i] = mn.elem[i];

        return *this;
    }

    T* operator [](size_t mi)
    {
        bsASSERT((mi >= cols), "i must be less than size");
        return &elem[mi*rows];
    }
    const T* operator [](size_t mi)const
    {
        bsASSERT((mi >= cols), "i must be less than size");
        return &elem[mi*rows];
    }
    T& pos(size_t mi, size_t ni)
    {
        bsASSERT((mi >= cols) && (ni >= rows), "i must be less than size");
        return elem[mi*rows+ni];
    }
    const T& pos(size_t mi, size_t ni)const
    {
        bsASSERT((mi >= cols) && (ni >= rows), "i must be less than size");
        return elem[mi*rows+ni];
    }
    T* pos(size_t mi)
    {
        return *this[mi];
    }
    const T* pos(size_t mi)const
    {
        return *this[mi];
    }

};

typedef bs_matrixMxN<float> bs_matMxNi;
typedef bs_matrixMxN<float> bs_matMxNf;
typedef bs_matrixMxN<real> bs_matMxNd;

typedef bs_matrixMxN<int>  imatMxN;
typedef bs_matrixMxN<float> fmatMxN;
typedef bs_matrixMxN<real> dmatMxN;

template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_matrixMxN<T> &v)
{
    in << v.cols;
    in << v.rows;
    in << bs_byte((char*)v.elem, sizeof(T)*v.cols*v.rows);
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_matrixMxN<T> &v)
{
    bs_byte tmp;
    out >> v.cols;
    out >> v.rows;
    out >> tmp;
    v.resize(v.cols, v.rows);
    bs_memory::copy_sse(v.elem, tmp.data(), tmp.size());

    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_matrixMxN<T> &v)
{
    in << v.cols;
    in << v.rows;
    in << '\n';
    for (size_t m = 0; m < v.cols; ++m)
    {
        for (size_t n = 0; n < v.rows; ++n)
            in <<  v[m][n];
        in << '\n';
    }
    return in;
}
template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_matrixMxN<T> &v)
{
    size_t col = 0, row = 0;
    char tm;
    out >> col >> row >> tm;
    v.resize(col, row);
    for (size_t m = 0; m < col; ++m)
    {
        for (size_t n = 0; n < row; ++n)
            out >> v[m][n];
        out >> tm;
    }
    return out;
}
#endif // MAT4X4_H
