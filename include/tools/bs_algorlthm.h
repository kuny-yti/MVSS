#ifndef BS_ALGORLTHM_H
#define BS_ALGORLTHM_H

#include "bs_define.h"


template <typename T>
inline _fix_ T &bs_min(_in_ T & a, _in_ T & b) { return (a < b) ? a : b; }
template <typename T>
inline _fix_ T &bs_max(_in_ T & a, _in_ T & b) { return (a < b) ? b : a; }
template <typename T>
inline _fix_ T &bs_bound(_in_ T & min, _in_ T & val, _in_ T & max)
{ return bs_max(min, bs_min(max, val)); }

#define _SWAP_(T) inline void bs_swap(T& a, T& b){T tmp = a; a = b; b = tmp;}
_SWAP_(bool)
_SWAP_(char)
_SWAP_(short)
_SWAP_(int)
_SWAP_(long)
_SWAP_(float)
_SWAP_(uchar)
_SWAP_(ushort)
_SWAP_(uint)
_SWAP_(ulong)
_SWAP_(real)
#if bsCompiler != bsCompiler_GNUC && bsCompiler != bsCompiler_CLANG
_SWAP_(int64)
#endif
#if bsCompiler != bsCompiler_GNUC && bsCompiler != bsCompiler_CLANG
_SWAP_(uint64)
#endif
_SWAP_(wchar_t)



//!
//! @brief bs_isort 数组型插入排序
//!
//! @param a 是数组的开始地址
//! @param length 是数组的长度
//!
template<typename T>
inline void bs_isort( T *a, size_t length)
{
    for (int i = 1; i < (int)length; i++)
        for (int j = i - 1; j >= 0 && a[j] > a[j + 1]; j--)
            bs_swap(a[j], a[j + 1]);
}
template<typename container_iterator>
inline void bs_isort (container_iterator begin, container_iterator end)
{
    int length = end - begin;
    for (int i = 1; i < (int)length; i++)
        for (int j = i - 1; j >= 0 && *(begin + j) > *(begin + (j + 1)); j--)
            bs_swap(*(begin + j), *(begin + (j + 1)));
}
//!
//! @brief bs_isort2 数组型2路插入排序
//!
//! @param a 是数组的开始地址
//! @param length 是数组的长度
//!
template<typename T>
inline void bs_isort2(T* a, size_t alength)
{
    int first = 0, last = 0;
    T b[alength] = {T()};
    b[0] = a[0];
    for (int i = 1; i < (int)alength; i++)
    {
        if (a[i] < b[first])
        {
            first = (first - 1 + alength) % alength;
            b[first] = a[i];
        }
        else if (a[i]>=b[last])
        {
            last++;
            b[last] = a[i];
        }
        else
        {
            int low, high, mid, d;
            low = first, high = last;
            while (low != high)
            {
                d = (high-low+alength) % alength;
                mid = (low + d / 2) % alength;
                if (a[i] < b[mid])
                    high = mid;
                else
                    low = (mid + 1) % alength;
            }
            for (int k = last + 1; k != low; k = (k - 1 + alength) % alength)
                b[k] = b[(k - 1 + alength) % alength];
            b[low] = a[i];
            last++;
        }
    }
    for (int i = 0; i < (int)alength; i++)
        a[i] = b[(i + first) % alength];
}
//!
//! @brief bs_qsort 数组型快速排序
//!
//! @param begin 是数组的开始地址
//! @param end 是数组的结束地址
//!
template<typename T>
inline void bs_qsort(T *begin, T *end)
{
top:
    int length = end - begin;
    if (length < 2)
        return;

    --end;
    T *low = begin, *high = end-1;
    T *pivot = begin + length / 2 ;

    if (*end < *begin)
        bs_swap(*end, *begin);
    if (length == 2)
        return ;

    if (*pivot < *begin)
        bs_swap(*pivot, *begin);
    if (*end < *pivot)
        bs_swap(*end, *pivot);
    if (length == 3)
        return ;

    bs_swap(*pivot, *end);

    while (low < high)
    {
        while (low < high && *low < *end)
            ++low;
        while (high > low && *end < *high)
            --high;

        if (low < high)
        {
            bs_swap(*low, *high);
            ++low;
            --high;
        } else
            break;
    }

    if (*low < *end)
        ++low;

    bs_swap(*end, *low);
    bs_qsort(begin, low);

    begin = low + 1;
    ++end;
    goto top;
}
//!
//! @brief bs_qsort 容器型快速排序
//!
//! @param begin 是容器开始迭代
//! @param end 是容器结束迭代
//!
template<typename container_iterator>
inline void bs_qsort(container_iterator begin, container_iterator end)
{
top:
    int span = int(end - begin);
    if (span < 2)
        return;

    --end;
    container_iterator low = begin, high = end - 1;
    container_iterator pivot = begin + span / 2;

    if (*end < *begin)
        bs_swap(*end, *begin);
    if (span == 2)
        return;

    if (*pivot < *begin)
        bs_swap(*pivot, *begin);
    if (*end < *pivot)
        bs_swap(*end, *pivot);
    if (span == 3)
        return;

    bs_swap(*pivot, *end);

    while (low < high)
    {
        while (low < high && *low < *end)
            ++low;
        while (high > low && *end < *high)
            --high;

        if (low < high)
        {
            bs_swap(*low, *high);
            ++low;
            --high;
        } else
            break;
    }

    if (*low < *end)
        ++low;

    bs_swap(*end, *low);
    bs_qsort(begin, low);

    begin = low + 1;
    ++end;
    goto top;
}

template <typename T>
inline void _HeapAdjust_(T *H, T s, size_t length)
{
    T tmp  = H[s];
    size_t child = 2 * s+1;
    while (child < length)
    {
        if (child+1 < length && H[child] < H[child+1])
            ++child ;
        if (H[s] < H[child])
        {
            H[s] = H[child];
            s = child;
            child = 2*s+1;
        }
        else
            break;
        H[s] = tmp;
    }
}

//!
//! @brief bs_hsort 堆排序算法
//!
//! @param H是待调整的堆数组
//! @param length是数组的长度
//!
template <typename T>
inline void bs_hsort(T *H, size_t length)
{
    for (size_t i = (length -1) / 2 ; i > 0; --i)
        _HeapAdjust_<T>(H, i, length);
    _HeapAdjust_<T>(H, 0, length);
    for (size_t i = length - 1; i > 0; --i)
    {
        bs_swap(H[i], H[0]);
        _HeapAdjust_<T>(H, 0, i);
    }
}
//!
//! @brief bs_bsort 冒泡排序算法
//!
//! @param H是待调整的堆数组
//! @param length是数组的长度
//!
template <typename T>
inline void bs_bsort (T *r, size_t length)
{
    int low = 0;
    int high = length -1;
    while (low < high)
    {
        for (int j = low; j < high; ++j)
            if (r[j] > r[j+1])
                bs_swap(r[j], r[j+1]);
        --high;
        for (int j = high; j > low; --j)
            if (r[j] < r[j-1])
                bs_swap(r[j], r[j-1]);
        ++low;
    }
}

template <typename T>
inline void _merge_( T *arr, int low, int mid, int high)
{
    int i = low, j = mid+1, k=0;
    T temp[high-low+1]= {T()};
    while (i <= mid && j <= high)
    {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= high)
        temp[k++] = arr[j++];
    for(i = low, k = 0; i <= high; i++, k++)
        arr[i] = temp[k];
}
//!
//! @brief bs_msort 归并排序算法
//!
//! @param arr 是待调整的堆数组
//! @param length是数组的长度
//!
template <typename T>
inline void bs_msort(T *arr, size_t length)
{
    int size = 1, low, mid, high;
    while(size <= (int)length-1)
    {
        low = 0;
        while(low + size <= (int)length-1)
        {
            mid = low + size-1;
            high = mid + size;
            if(high > (int)length-1)
                high = length-1;
            _merge_<T>(arr, low, mid, high);
            low = high+1;
        }
        size *= 2;
    }
}

namespace __hash_
{
    // RS Hash Function
    static uint32 RS (char *str, size_t len)
    {
        uint32 b = 378551 ,a =63689 ;
        uint32 hash = 0 ;

        for (size_t i = 0; i < len; ++i)
        {
            hash = hash * a + str[i];
            a *= b;
        }

        return hash;
    }
    // JS Hash Function
    static uint32 JS(char *str, size_t len)
    {
        uint32 hash = 1315423911 ;

        for (size_t i = 0; i < len; ++i)
            hash ^= ((hash << 5) + (str[i]) + (hash >> 2));

        return hash;
    }
    // P. J. Weinberger Hash Function
    static uint32 PJW(char *str, size_t len)
    {
        uint32 BitsInUnignedInt = (uint32)(sizeof(uint) *8);
        uint32 ThreeQuarters = (uint32)((BitsInUnignedInt *3) /4);
        uint32 OneEighth = (uint32)(BitsInUnignedInt /8);
        uint32 HighBits = (uint32)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
        uint32 hash = 0 ;
        uint32 test = 0 ;

        for (size_t i = 0; i < len; ++i)
        {
            hash = (hash << OneEighth) + (str[i]);
            if((test = hash & HighBits) != 0)
                hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }

        return hash;
    }
    // ELF Hash Function
    static uint32 ELF(char *str, size_t len)
    {
        uint32 hash = 0 ;
        uint32 x = 0 ;

        for (size_t i = 0; i < len; ++i)
        {
            hash = (hash << 4) + str[i];
            if((x = hash & 0xF0000000L) != 0)
            {
                hash ^= (x >> 24);
                hash &= ~x ;
            }
        }

        return hash;
    }

    // BKDR Hash Function
    static uint32 BKDR(char *str, size_t len, uint seed = 131)
    {
        //uint seed = 131 ;// 31 131 1313 13131 131313 etc..
        uint32 hash = 0 ;

        for (size_t i = 0; i < len; ++i)
            hash = hash * seed + str[i];

        return hash;
    }

    // SDBM Hash Function
    static uint32 SDBM(char *str, size_t len)
    {
        uint32 hash=0 ;

        for (size_t i = 0; i < len; ++i)
            hash = str[i] + (hash << 6) + (hash << 16) - hash ;

        return hash;
    }

    // DJB Hash Function
    static uint32 DJB(char *str, size_t len)
    {
        uint32 hash = 5381 ;

        for (size_t i = 0; i < len; ++i)
            hash = (hash + (hash << 5)) + str[i];

        return hash;
    }

    // AP Hash Function
    static uint32 AP(char *str, size_t len)
    {
        uint32 hash = 0 ;
        for(size_t i = 0; i < len; i++)
        {
            if((i & 1) == 0)
                hash ^= ((hash << 7) ^ str[i] ^ (hash >> 3));
            else
                hash ^= (~((hash << 11) ^ str[i] ^ (hash >> 5)));
        }

        return hash;
    }
    /*DEKHash*/
    static uint32 DEK(char *str, size_t len)
    {
        uint32 hash = len;
        for(size_t i = 0; i < len; i++)
            hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
        return hash;
    }
    /*BPHash*/
    static uint32 BP(char *str, size_t len)
    {
        uint32 hash = 0;
        for(size_t i = 0;i < len; i++)
            hash = hash << 7 ^ str[i];
        return hash;
    }
    /*FNVHash*/
    static uint32 FNV(char *str, size_t len)
    {
        uint32 fnv_prime = 0x811C9DC5;
        uint32 hash = 0;
        for(size_t i = 0; i < len; i++)
        {
            hash *= fnv_prime;
            hash ^= str[i];
        }
        return hash;
    }
    static uint32 H64(uint64 v)
    {
        v = (~v) + (v << 18); // v = (v << 18) - v - 1;
        v = v ^ (v >> 31);
        v = v * 21; // v = (v + (v << 2)) + (v << 4);
        v = v ^ (v >> 11);
        v = v + (v << 6);
        v = v ^ (v >> 22);
        return (uint32) v;
    }
}

inline uint32 bs_hash(_in_ char & v){return (uint32)v;}
inline uint32 bs_hash(_in_ uchar & v){return (uint32)v;}
inline uint32 bs_hash(_in_ short & v){return (uint32)v;}
inline uint32 bs_hash(_in_ ushort & v){return (uint32)v;}
inline uint32 bs_hash(_in_ int & v){return (uint32)v;}
inline uint32 bs_hash(_in_ uint & v){return (uint32)v;}
inline uint32 bs_hash(_in_ wchar_t & v){return (uint32)v;}
inline uint32 bs_hash(_in_ long & v){return __hash_::H64 (v);}
inline uint32 bs_hash(_in_ ulong & v){return __hash_::H64 (v);}
inline uint32 bs_hash(_in_ longlong & v){return __hash_::H64 (v);}
inline uint32 bs_hash(_in_ ulonglong & v){return __hash_::H64 (v);}
inline uint32 bs_hash(float v){union{float tv;uint64 v64;};tv = v; return __hash_::H64(v64);}
inline uint32 bs_hash(real v){union{real tv;uint64 v64;};tv = v; return __hash_::H64(v64);}
inline uint32 bs_hash(lreal v){union{lreal tv;uint64 v64;};tv = v; return __hash_::H64(v64);}
inline uint32 bs_hash(char *v, int len = -1){if (len < 0) len = strlen (v);return __hash_::DJB(v, len);}

#endif //BS_ALGORLTHM_H
