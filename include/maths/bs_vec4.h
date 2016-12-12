#ifndef VEC4_H
#define VEC4_H

#include "bs_vec3.h"

/**
    This is a 4-dimensional Vector typename similar to vec4 of GLSL.
    All standard-operators are overloaded and go componentwisely
    similar to GLSL. For dotproduct use the function dot.
*/
template<typename T>
struct bs_vector4 {
    typedef T ElemType;

    enum {
        size = 4
    };

    union {
        struct {
            T x;
            T y;
            T z;
            T w;
        };
        struct {
            T s;
            T t;
            T p;
            T q;
        };
        struct {
            T r;
            T g;
            T b;
            T a;
        };
        struct {
            T _x;
            T _y;
            T width;
            T height;
        };
        // workaround to prevent "dereferencing type-punned pointer will break strict-aliasing
        // rules" warning with gcc and -fstrict-aliasing.
        struct {
            T _xyz[size-1];
            T _w;
        };
        T elem[size];
    };

    /*
    constructors
*/
    /// default constructor
    bs_vector4() {}
    /// Init all elements with the same value
    explicit bs_vector4(T init) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = init;
    }
    /// Init from array with equal size
    explicit bs_vector4(const T* init) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = init[i];
    }
    /// Init componentwisely
    bs_vector4(T t1, T t2, T t3, T t4) {
        elem[0] = t1;
        elem[1] = t2;
        elem[2] = t3;
        elem[3] = t4;
    }
    /// Init x and y with v1, z and w with v2
    bs_vector4(const bs_vector4<T>& v1, const bs_vector4<T>& v2) {
        elem[0] = v1.elem[0];
        elem[1] = v1.elem[1];
        elem[2] = v2.elem[0];
        elem[3] = v2.elem[1];
    }
    /// Init x and y with vec2, z and w with _z and _w
    bs_vector4(const bs_vector4<T>& vec, T z_, T w_) {
        elem[0] = vec.elem[0];
        elem[1] = vec.elem[1];
        elem[2] = z_;
        elem[3] = w_;
    }
    /// Init x and y with _x and _y, z and w with _z and _w
    bs_vector4(T _x, T _y, const bs_vector4<T>& v) {
        elem[0] = _x;
        elem[1] = _y;
        elem[2] = v.elem[0];
        elem[3] = v.elem[1];
    }
    /// Init x, y, z with v and w with _w
    bs_vector4(const bs_vector3<T>& vec, T w_) {
        elem[0] = vec.elem[0];
        elem[1] = vec.elem[1];
        elem[2] = vec.elem[2];
        elem[3] = w_;
    }
    /// Init x with _x and y, z and w with v
    bs_vector4(T x_, const bs_vector4<T>& v) {
        elem[0] = x_;
        elem[1] = v.elem[0];
        elem[2] = v.elem[1];
        elem[3] = v.elem[2];
    }
    bs_vector4(const bs_vector2<T>& pos, const bs_vector2<T>& size) {
        elem[0] = pos.x;
        elem[1] = pos.y;
        elem[2] = size.width;
        elem[3] = size.height;
    }

    /// Init with another Vector of another type
    template<typename U>
    bs_vector4(const bs_vector4<U>& v) {
        for (size_t i = 0; i < v.size; ++i)
            elem[i] = T(v.elem[i]);
    }

    /*
    index operator
*/
    /// Index operator
    const T& operator [] (size_t index) const {
        return elem[index];
    }
    /// Index operator
    T& operator [] (size_t index) {
        return elem[index];
    }

    /*
    sub-vector getters
*/
    const bs_vector2<T>& xy() const {
        return xyz().xy();
    }
    const bs_vector2<T>& yz() const {
        return *((bs_vector2<T>*) (elem + 1));
    }
    const bs_vector2<T>& zw() const {
        return *((bs_vector2<T>*) (elem + 2));
    }
    const bs_vector3<T>& xyz() const {
        return *((bs_vector3<T>*) _xyz);
    }
    const bs_vector3<T>& yzw() const {
        return *((bs_vector3<T>*) (elem + 1));
    }
    bs_vector2<T>& xy() {
        return xyz().xy();
    }
    bs_vector2<T>& yz() {
        return *((bs_vector2<T>*) (elem + 1));
    }
    bs_vector2<T>& zw() {
        return *((bs_vector2<T>*) (elem + 2));
    }
    bs_vector3<T>& xyz() {
        return *((bs_vector3<T>*) _xyz);
    }
    bs_vector3<T>& yzw() {
        return *((bs_vector3<T>*) (elem + 1));
    }


    /*
    statics
*/
    //static bs_vector4<float> white;
    //static bs_vector4<float> black;
    //static bs_vector4<float> gray;
    //static bs_vector4<float> red;
    //static bs_vector4<float> green;
    //static bs_vector4<float> blue;
    //static bs_vector4<float> brown;
    //static bs_vector4<float> yellow;

    //static bs_vector4<T> zero;
};


typedef bs_vector4<float>   bs_vector4f;
typedef bs_vector4<real>  bs_vector4d;

/*
        types like in GLSL
    */
typedef bs_vector4<float>   vec4;
typedef bs_vector4<real>    dvec4; // currently not supported in GLSL but a reserved keyword
typedef bs_vector4<int>    ivec4;
typedef bs_vector4<size_t> svec4;
typedef bs_vector4<bool>   bvec4;

typedef bs_vector4<int> irect;
typedef bs_vector4<float> frect;
typedef bs_vector4<real> drect;


/*
        Prepare to implement 3 * 5 * 8 = 120 operators and dozens of functions
        the lazy way with evil voodoo macro magic
    */
template<typename T> inline bs_vector4<T> operator + (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] + v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector4<T> operator + (const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] + t;
    return vRes;
}
template<typename T> inline bs_vector4<T> operator + (T t, const bs_vector4<T>& v)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t + v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector4<T>& operator += (bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] += v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector4<T>& operator += (bs_vector4<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] += t;
    return v;
}

template<typename T> inline bs_vector4<T> operator - (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] - v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector4<T> operator - (const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] - t;
    return vRes;
}
template<typename T> inline bs_vector4<T> operator - (T t, const bs_vector4<T>& v)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t - v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector4<T>& operator -= (bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] -= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector4<T>& operator -= (bs_vector4<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] -= t;
    return v;
}

template<typename T> inline bs_vector4<T> operator * (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] * v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector4<T> operator * (const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] * t;
    return vRes;
}
template<typename T> inline bs_vector4<T> operator * (T t, const bs_vector4<T>& v)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t * v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector4<T>& operator *= (bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] *= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector4<T>& operator *= (bs_vector4<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] *= t;
    return v;
}

template<typename T> inline bs_vector4<T> operator / (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] / v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector4<T> operator / (const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] / t;
    return vRes;
}
template<typename T> inline bs_vector4<T> operator / (T t, const bs_vector4<T>& v)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t / v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector4<T>& operator /= (bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] /= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector4<T>& operator /= (bs_vector4<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] /= t;
    return v;
}

template<typename T> inline bs_vector4<T> operator % (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] % v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector4<T> operator % (const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] % t;
    return vRes;
}
template<typename T> inline bs_vector4<T> operator % (T t, const bs_vector4<T>& v)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t % v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector4<T>& operator %= (bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] %= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector4<T>& operator %= (bs_vector4<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] %= t;
    return v;
}

template<typename T> inline bs_vector4<T> operator & (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] & v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector4<T> operator & (const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] & t;
    return vRes;
}
template<typename T> inline bs_vector4<T> operator & (T t, const bs_vector4<T>& v)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t & v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector4<T>& operator &= (bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] &= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector4<T>& operator &= (bs_vector4<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] &= t;
    return v;
}

template<typename T> inline bs_vector4<T> operator | (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] | v2.elem[i];
    return vRes;
}

template<typename T> inline bs_vector4<T> operator | (const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] | t;
    return vRes;
}
template<typename T> inline bs_vector4<T> operator | (T t, const bs_vector4<T>& v)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t | v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector4<T>& operator |= (bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] |= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector4<T>& operator |= (bs_vector4<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] |= t;
    return v;
}

template<typename T> inline bs_vector4<T> operator ^ (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] ^ v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector4<T> operator ^ (const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] ^ t;
    return vRes;
}
template<typename T> inline bs_vector4<T> operator ^ (T t, const bs_vector4<T>& v)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t ^ v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector4<T>& operator ^= (bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] ^= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector4<T>& operator ^= (bs_vector4<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] ^= t;
    return v;
}

/*
        relational operators
    */
template<typename T>
inline bs_vector4<bool> lessThan (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] < v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector4<bool> lessThanEqual (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] <= v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector4<bool> greaterThan (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] > v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector4<bool> greaterThanEqual (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] >= v2.elem[i]);
    return vRes;
}
template<typename T>
inline bs_vector4<bool> equal (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] == v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector4<bool> notEqual (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] != v2.elem[i]);
    return vRes;
}

template<typename T>
inline bool operator == (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] == v2.elem[i]);
    return result;
}

template<typename T>
inline bool operator != (const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    return !(v1 == v2);
}


/*
        further vector functions
    */
template<typename T> inline T dot(const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    T result = 0;
    for (size_t i = 0; i < v1.size; ++i)
        result += v1.elem[i] * v2.elem[i];
    return result;
}

template<typename T> inline T lengthSq(const bs_vector4<T>& v)
{
    T result = 0;
    for (size_t i = 0; i < v.size; ++i)
        result += (v.elem[i] * v.elem[i]);
    return result;
}

template<typename T> inline T length(const bs_vector4<T>& v)
{
    return std::sqrt(lengthSq(v));
}

template<typename T> inline bs_vector4<T> normalize(const bs_vector4<T>& v)
{
    bs_vector4<T> vRes(v);
    T recipLength = 1 / length(v);
    vRes *= recipLength;
    return vRes;
}

template<typename T> inline T distance(const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    return length(v2 - v1);
}

template<typename T> inline T distanceSq(const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    return lengthSq(v2 - v1);
}

template<typename T> inline bs_vector4<T> operator - (const bs_vector4<T>& v)
{
    bs_vector4<T> vRes(v);
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = -v.elem[i];
    return vRes;
}

template<typename T> inline bs_vector4<T> min(const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = bs_min(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_vector4<T> max(const bs_vector4<T>& v1, const bs_vector4<T>& v2)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = bs_max(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_vector4<T> min(const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = bs_min(v.elem[i], t);
    return vRes;
}

template<typename T> inline bs_vector4<T> max(const bs_vector4<T>& v, T t)
{
    bs_vector4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = bs_max(v.elem[i], t);
    return vRes;
}

template<typename T> inline T min(const bs_vector4<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = bs_min(v.elem[i], res);
    return res;
}

template<typename T> inline T max(const bs_vector4<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = bs_max(v.elem[i], res);
    return res;
}

template<typename T> inline bs_vector4<T> clamp(const bs_vector4<T>& v, const bs_vector4<T>& minVal, const bs_vector4<T>& maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline bs_vector4<T> clamp(const bs_vector4<T>& v, T minVal, T maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline bs_vector4<T> ceil(const bs_vector4<T>& v)
{
    using std::ceil;
    bs_vector4<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ceil(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<T> floor(const bs_vector4<T>& v)
{
    using std::floor;
    bs_vector4<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = floor(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<T> fix(const bs_vector4<T>& v)
{
    bs_vector4<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = fix(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<T> round(const bs_vector4<T>& v)
{
    bs_vector4<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = round(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<T> sign(const bs_vector4<T>& v) {
    bs_vector4<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = sign(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<int> iceil(const bs_vector4<T>& v)
{
    bs_vector4<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iceil(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<int> ifloor(const bs_vector4<T>& v)
{
    bs_vector4<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifloor(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<int> iround(const bs_vector4<T>& v)
{
    bs_vector4<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iround(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<int> ifix(const bs_vector4<T>& v)
{
    bs_vector4<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifix(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<int> isign(const bs_vector4<T>& v)
{
    bs_vector4<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = isign(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector4<T> mix(const bs_vector4<T>& v, const bs_vector4<T>& v2, T t)
{
    bs_vector4<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result.elem[i] = (v.elem[i]*(1-t)) + (v2.elem[i]*t);
    return result;
}

/*
        horizontally working functions
    */
template<typename T> inline T hadd(const bs_vector4<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result += v.elem[i];
    return result;
}

template<typename T> inline T hsub(const bs_vector4<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result -= v.elem[i];
    return result;
}

template<typename T> inline T hmul(const bs_vector4<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result *= v.elem[i];
    return result;
}

template<typename T> inline T hdiv(const bs_vector4<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result /= v.elem[i];
    return result;
}

template<typename T> inline T hmod(const bs_vector4<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result %= v.elem[i];
    return result;
}

template<typename T> inline bool hand(const bs_vector4<T>& v)
{
    bool result = v.elem[0] && v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result &= v.elem[i];
    return result;
}

template<typename T> inline bool hor(const bs_vector4<T>& v)
{
    T result = v.elem[0] || v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result |= v.elem[i];
    return result;
}

template<typename T> inline bool hxor(const bs_vector4<T>& v)
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
inline bs_vector4<T> abs(const bs_vector4<T>& v)
{
    using std::abs;
    return bs_vector4<T>(fabs(v.x), fabs(v.y), fabs(v.z), fabs(v.z));
}

/*
        specialization for length(VectorX<int>)
    */

inline float length(const bs_vector4<int>& v) {
    return std::sqrt( float(lengthSq(v)) );
}

template<typename T>
inline bs_vector2<T> center(const bs_vector4<T>& v)
{
    return v.pos () + (v.size ()*0.5);
}
template<typename T>
inline bool contains(const bs_vector4<T>& v, const bs_vector4<T>& v1)
{
    T l1 = v.x;
    T r1 = v.x;
    if (v.width < 0)
        l1 += v.width;
    else
        r1 += v.width;
    if (l1 == r1) // null rect
        return false;

    T l2 = v1.x;
    T r2 = v1.x;
    if (v1.width < 0)
        l2 += v1.width;
    else
        r2 += v1.width;
    if (l2 == r2) // null rect
        return false;

    if (l2 < l1 || r2 > r1)
        return false;

    T t1 = v.y;
    T b1 = v.y;
    if (v.height < 0)
        t1 += v.height ;
    else
        b1 += v.height ;
    if (t1 == b1) // null rect
        return false;

    T t2 = v1.y;
    T b2 = v1.y;
    if (v1.height  < 0)
        t2 += v1.height ;
    else
        b2 += v1.height ;
    if (t2 == b2) // null rect
        return false;

    if (t2 < t1 || b2 > b1)
        return false;

    return true;
}
template<typename T>
inline bool contains(const bs_vector4<T>& v, const bs_vector2<T>&p)
{
    T l = v.x;
    T r = v.x;
    if (v.width < 0)
        l += v.width ;
    else
        r += v.width ;
    if (l == r) // null rect
        return false;

    if (p.x < l || p.x > r)
        return false;

    T t = v.y;
    T b = v.y;
    if (v.height < 0)
        t += v.height ;
    else
        b += v.height ;
    if (t == b) // null rect
        return false;

    if (p.y < t || p.y > b)
        return false;

    return true;
}
template<typename T>
inline bool intersects(const bs_vector4<T>& v, const bs_vector4<T> &rhs)
{
    T l1 = v.x;
    T r1 = v.x;
    if (v.width < 0)
        l1 += v.width;
    else
        r1 += v.width;
    if (l1 == r1) // null rect
        return false;

    T l2 = rhs.x;
    T r2 = rhs.x;
    if (rhs.width < 0)
        l2 += rhs.width;
    else
        r2 += rhs.width;
    if (l2 == r2) // null rect
        return false;

    if (l1 >= r2 || l2 >= r1)
        return false;

    T t1 = v.y;
    T b1 = v.y;
    if (v.height  < 0)
        t1 += v.height ;
    else
        b1 += v.height ;
    if (t1 == b1) // null rect
        return false;

    T t2 = rhs.y;
    T b2 = rhs.y;
    if (rhs.height < 0)
        t2 += rhs.height ;
    else
        b2 += rhs.height ;
    if (t2 == b2) // null rect
        return false;

    if (t1 >= b2 || t2 >= b1)
        return false;

    return true;
}
template<typename T>
bs_vector4<T> intersect(const bs_vector4<T>& v, const bs_vector4<T> &rhs)
{
    T l1 = v.x;
    T r1 = v.x;
    if (v.width < 0)
        l1 += v.width ;
    else
        r1 += v.width ;
    if (l1 == r1) // null rect
        return bs_vector4<T>();

    T l2 = rhs.x;
    T r2 = rhs.x;
    if (rhs.width  < 0)
        l2 += rhs.width ;
    else
        r2 += rhs.width ;
    if (l2 == r2) // null rect
        return bs_vector4<T>();

    if (l1 >= r2 || l2 >= r1)
        return bs_vector4<T>();

    T t1 = v.y;
    T b1 = v.y;
    if (v.height ()< 0)
        t1 += v.height ;
    else
        b1 += v.height ;
    if (t1 == b1) // null rect
        return bs_vector4<T>();

    T t2 = rhs.y;
    T b2 = rhs.y;
    if (rhs.height < 0)
        t2 += rhs.height ;
    else
        b2 += rhs.height ;
    if (t2 == b2) // null rect
        return bs_vector4<T>();

    if (t1 >= b2 || t2 >= b1)
        return bs_vector4<T>();

    bs_vector4<T> tmp;
    tmp.x = bs_max(l1, l2);
    tmp.y = bs_max(t1, t2);
    tmp.width  = bs_min(r1, r2) - tmp.x;
    tmp.height = bs_min(b1, b2) - tmp.y;
    return tmp;
}

/*
         stream operators
    */

/// output bs_vector4d to stream
template<typename T>
std::ostream& operator << (std::ostream& s, const bs_vector4<T>& v) {
    return (s << "[" << v.elem[0] << " " << v.elem[1] << " " << v.elem[2] << " " << v.elem[3] << "]");
}


/// input bs_vector4d from stream
template<typename T>
std::istream& operator >> (std::istream& s, bs_vector4<T>& v) {
    char ch;

    s >> ch;
    if (ch =='[') {  // format "[ x y z w ]"
        s >> v[0] >> v[1] >> v[2] >> v[3] >> ch;
        if (ch != ']') {
            s.clear(std::ios::badbit);
        }
    } else { // format "x y z w"
        s.putback(ch);
        s >> v[0] >> v[1] >> v[2] >> v[3];
    }

    return s;
}

template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_vector4<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_vector4<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_vector4<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_vector4<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
#endif // VEC4_H
