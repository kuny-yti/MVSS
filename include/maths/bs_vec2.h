#ifndef BS_VEC2_H
#define BS_VEC2_H

#include "bs_maths.h"
#include "bs_debug.h"
#include "tools/bs_stream.h"

template<typename T>
struct bs_vector2
{
    typedef T ElemType;

    enum
    {
        size = 2
    };

    union
    {
        struct
        {
            T x;
            T y;
        };
        struct {
            T s;
            T t;
        };
        struct {
            T r;
            T g;
        };
        struct {
            T width;
            T height;
        };
        T elem[size];
    };

    bs_vector2() {}
    explicit bs_vector2(T v) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = v;
    }
    explicit bs_vector2(const T* v) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = v[i];
    }
    bs_vector2(T t1, T t2) {
        elem[0] = t1;
        elem[1] = t2;
    }

    template<typename U>
    bs_vector2(const bs_vector2<U>& v) {
        for (size_t i = 0; i < v.size; ++i)
            elem[i] = T(v.elem[i]);
    }

    const T& operator [] (size_t index) const {
        return elem[index];
    }

    T& operator [] (size_t index) {
        return elem[index];
    }

    static bs_vector2<T> zero;
};

template<typename T>
bs_vector2<T> bs_vector2<T>::zero = bs_vector2<T>(T(0), T(0));

typedef bs_vector2<float>   bs_vector2f;
typedef bs_vector2<real>  bs_vector2d;

/*
        types like in GLSL
    */
typedef bs_vector2<float>  vec2;
typedef bs_vector2<real>   dvec2; // currently not supported in GLSL but a reserved keyword
typedef bs_vector2<int>    ivec2;
typedef bs_vector2<size_t> svec2;
typedef bs_vector2<bool>   bvec2;

typedef bs_vector2<int>  isize;
typedef bs_vector2<float> fsize;
typedef bs_vector2<real> dsize;

typedef bs_vector2<int>  ipoint;
typedef bs_vector2<float> fpoint;
typedef bs_vector2<real> dpoint;

/*
        Prepare to implement 3 * 5 * 8 = 120 operators and dozens of functions
        the lazy way with evil voodoo macro magic
    */
template<typename T> inline bs_vector2<T> operator + (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] + v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector2<T> operator + (const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] + t;
    return vRes;
}
template<typename T> inline bs_vector2<T> operator + (T t, const bs_vector2<T>& v)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t + v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector2<T>& operator += (bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] += v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector2<T>& operator += (bs_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] += t;
    return v;
}

template<typename T> inline bs_vector2<T> operator - (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] - v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector2<T> operator - (const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] - t;
    return vRes;
}
template<typename T> inline bs_vector2<T> operator - (T t, const bs_vector2<T>& v)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t - v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector2<T>& operator -= (bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] -= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector2<T>& operator -= (bs_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] -= t;
    return v;
}

template<typename T> inline bs_vector2<T> operator * (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] * v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector2<T> operator * (const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] * t;
    return vRes;
}
template<typename T> inline bs_vector2<T> operator * (T t, const bs_vector2<T>& v)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t * v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector2<T>& operator *= (bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] *= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector2<T>& operator *= (bs_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] *= t;
    return v;
}

template<typename T> inline bs_vector2<T> operator / (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] / v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector2<T> operator / (const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] / t;
    return vRes;
}
template<typename T> inline bs_vector2<T> operator / (T t, const bs_vector2<T>& v)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t / v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector2<T>& operator /= (bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] /= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector2<T>& operator /= (bs_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] /= t;
    return v;
}

template<typename T> inline bs_vector2<T> operator % (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] % v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector2<T> operator % (const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] % t;
    return vRes;
}
template<typename T> inline bs_vector2<T> operator % (T t, const bs_vector2<T>& v)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t % v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector2<T>& operator %= (bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] %= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector2<T>& operator %= (bs_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] %= t;
    return v;
}

template<typename T> inline bs_vector2<T> operator & (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] & v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector2<T> operator & (const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] & t;
    return vRes;
}
template<typename T> inline bs_vector2<T> operator & (T t, const bs_vector2<T>& v)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t & v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector2<T>& operator &= (bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] &= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector2<T>& operator &= (bs_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] &= t;
    return v;
}

template<typename T> inline bs_vector2<T> operator | (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] | v2.elem[i];
    return vRes;
}

template<typename T> inline bs_vector2<T> operator | (const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] | t;
    return vRes;
}
template<typename T> inline bs_vector2<T> operator | (T t, const bs_vector2<T>& v)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t | v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector2<T>& operator |= (bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] |= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector2<T>& operator |= (bs_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] |= t;
    return v;
}

template<typename T> inline bs_vector2<T> operator ^ (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] ^ v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector2<T> operator ^ (const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] ^ t;
    return vRes;
}
template<typename T> inline bs_vector2<T> operator ^ (T t, const bs_vector2<T>& v)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t ^ v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector2<T>& operator ^= (bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] ^= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector2<T>& operator ^= (bs_vector2<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] ^= t;
    return v;
}

template<typename T> inline bool operator > (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bool result = false;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] > v2.elem[i]);
    return result;
}
template<typename T> inline bool operator < (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bool result = false;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] < v2.elem[i]);
    return result;
}

/*
        relational operators
    */
template<typename T>
inline bs_vector2<bool> lessThan (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] < v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector2<bool> lessThanEqual (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] <= v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector2<bool> greaterThan (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] > v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector2<bool> greaterThanEqual (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] >= v2.elem[i]);
    return vRes;
}
template<typename T>
inline bs_vector2<bool> equal (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] == v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector2<bool> notEqual (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] != v2.elem[i]);
    return vRes;
}

template<typename T>
inline bool operator == (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] == v2.elem[i]);
    return result;
}

template<typename T>
inline bool operator != (const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    return !(v1 == v2);
}


/*
        further vector functions
    */
template<typename T> inline T dot(const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    T result = 0;
    for (size_t i = 0; i < v1.size; ++i)
        result += v1.elem[i] * v2.elem[i];
    return result;
}

template<typename T> inline T lengthSq(const bs_vector2<T>& v)
{
    T result = 0;
    for (size_t i = 0; i < v.size; ++i)
        result += (v.elem[i] * v.elem[i]);
    return result;
}

template<typename T> inline T length(const bs_vector2<T>& v)
{
    return std::sqrt(lengthSq(v));
}

template<typename T> inline bs_vector2<T> normalize(const bs_vector2<T>& v)
{
    bs_vector2<T> vRes(v);
    T recipLength = 1 / length(v);
    vRes *= recipLength;
    return vRes;
}

template<typename T> inline T distance(const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    return length(v2 - v1);
}

template<typename T> inline T distanceSq(const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    return lengthSq(v2 - v1);
}

template<typename T> inline bs_vector2<T> operator - (const bs_vector2<T>& v)
{
    bs_vector2<T> vRes(v);
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = -v.elem[i];
    return vRes;
}

template<typename T> inline bs_vector2<T> min(const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = bs_min(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_vector2<T> max(const bs_vector2<T>& v1, const bs_vector2<T>& v2)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = bs_max(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_vector2<T> min(const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = bs_min(v.elem[i], t);
    return vRes;
}

template<typename T> inline bs_vector2<T> max(const bs_vector2<T>& v, T t)
{
    bs_vector2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = bs_max(v.elem[i], t);
    return vRes;
}

template<typename T> inline T min(const bs_vector2<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = bs_min(v.elem[i], res);
    return res;
}

template<typename T> inline T max(const bs_vector2<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = bs_max(v.elem[i], res);
    return res;
}

template<typename T> inline bs_vector2<T> clamp(const bs_vector2<T>& v, const bs_vector2<T>& minVal, const bs_vector2<T>& maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline bs_vector2<T> clamp(const bs_vector2<T>& v, T minVal, T maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline bs_vector2<T> ceil(const bs_vector2<T>& v)
{
    using std::ceil;
    bs_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ceil(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<T> floor(const bs_vector2<T>& v)
{
    using std::floor;
    bs_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = floor(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<T> fix(const bs_vector2<T>& v)
{
    bs_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = fix(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<T> round(const bs_vector2<T>& v)
{
    bs_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = round(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<T> sign(const bs_vector2<T>& v) {
    bs_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = sign(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<int> iceil(const bs_vector2<T>& v)
{
    bs_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iceil(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<int> ifloor(const bs_vector2<T>& v)
{
    bs_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifloor(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<int> iround(const bs_vector2<T>& v)
{
    bs_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iround(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<int> ifix(const bs_vector2<T>& v)
{
    bs_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifix(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<int> isign(const bs_vector2<T>& v)
{
    bs_vector2<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = isign(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector2<T> mix(const bs_vector2<T>& v, const bs_vector2<T>& v2, T t)
{
    bs_vector2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result.elem[i] = (v.elem[i]*(1-t)) + (v2.elem[i]*t);
    return result;
}

/*
        horizontally working functions
    */
template<typename T> inline T hadd(const bs_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result += v.elem[i];
    return result;
}

template<typename T> inline T hsub(const bs_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result -= v.elem[i];
    return result;
}

template<typename T> inline T hmul(const bs_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result *= v.elem[i];
    return result;
}

template<typename T> inline T hdiv(const bs_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result /= v.elem[i];
    return result;
}

template<typename T> inline T hmod(const bs_vector2<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result %= v.elem[i];
    return result;
}

template<typename T> inline bool hand(const bs_vector2<T>& v)
{
    bool result = v.elem[0] && v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result &= v.elem[i];
    return result;
}

template<typename T> inline bool hor(const bs_vector2<T>& v)
{
    T result = v.elem[0] || v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result |= v.elem[i];
    return result;
}

template<typename T> inline bool hxor(const bs_vector2<T>& v)
{
    T result = v.elem[0] ^ v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result ^= v.elem[i];
    return result;
}


/**
        cross product
    */
/*
        Component-wise absolute value
    */
template<typename T>
inline bs_vector2<T> abs(const bs_vector2<T>& v) {
    using std::abs;
    return bs_vector2<T>(fabs(v.x), fabs(v.y));
}

/*
        specialization for length(VectorX<int>)
    */

inline float length(const bs_vector2<int>& v) {
    return std::sqrt( float(lengthSq(v)) );
}

/*
         stream operators
    */

/// output bs_vector2d to stream
template<typename T>
std::ostream& operator << (std::ostream& s, const bs_vector2<T>& v) {
    return (s << "[" << v.elem[0] << " " << v.elem[1] << "]");
}

// specialication for (unsigned) int, to make it output numbers and not characters

template<>
inline std::ostream& operator << <uchar>(std::ostream& s, const bs_vector2<uchar>& v) {
    return (s << bs_vector2<int>(v));
}

template<>
inline std::ostream& operator << <char>(std::ostream& s, const bs_vector2<char>& v) {
    return (s << bs_vector2<int>(v));
}

/// input bs_vector2d from stream
template<typename T>
std::istream& operator >> (std::istream& s, bs_vector2<T>& v) {
    char ch;

    s >> ch;
    if (ch == '[') {  // format "[ x y ]"
        s >> v[0] >> v[1] >> ch;
        if (ch != ']') {
            s.clear(std::ios::badbit);
        }
    } else { // format "x y"
        s.putback(ch);
        s >> v[0] >> v[1];
    }

    return s;
}


template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_vector2<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_vector2<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_vector2<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_vector2<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
#endif // VEC2_H
