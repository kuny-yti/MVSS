#ifndef BS_VEC3_H
#define BS_VEC3_H

#include "bs_vec2.h"


/**
        This is a 3-dimensional Vector typename similar to vec3 of GLSL.
        All standard-operators are overloaded and go componentwisely
        similar to GLSL. For dot- and crossproduct use the functions dot and cross.
    */
template<typename T>
struct bs_vector3 {
    typedef T ElemType;

    enum {
        size = 3
    };

    union {
        struct {
            T x;
            T y;
            T z;
        };
        struct {
            T s;
            T t;
            T p;
        };
        struct {
            T r;
            T g;
            T b;
        };
        struct {
          T width;
          T height;
          T depth;
        };
        // workaround to prevent "dereferencing type-punned pointer will break strict-aliasing
        // rules" warning with gcc and -fstrict-aliasing.
        struct {
            T _xy[size-1];
            T _z;
        };
        T elem[size];
    };

    /// default constructor
    bs_vector3() {}
    /// Init all elements with the same value
    explicit bs_vector3(T v) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = v;
    }
    /// Init from array with equal size
    explicit bs_vector3(const T* v) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = v[i];
    }
    /// Init componentwisely
    bs_vector3(T t1, T t2, T t3) {
        elem[0] = t1;
        elem[1] = t2;
        elem[2] = t3;
    }
    /// Init x and y with v, z with _z
    bs_vector3(const bs_vector3<T>& vec, T z_) {
        elem[0] = vec.elem[0];
        elem[1] = vec.elem[1];
        elem[2] = z_;
    }
    /// Init y and z with vec2, x with _x
    bs_vector3(T _x, const bs_vector3<T>& v) {
        elem[0] = _x;
        elem[1] = v.elem[1];
        elem[2] = v.elem[2];
    }
    bs_vector3(const bs_vector2<T>& v) {
        elem[0] = v.elem[0];
        elem[1] = v.elem[1];
        elem[2] = (T)0;
    }
    bs_vector3(const bs_vector2<T>& v, T z) {
        elem[0] = v.elem[0];
        elem[1] = v.elem[1];
        elem[2] = z;
    }

    /// Init with another Vector of another type
    template<typename U>
    bs_vector3(const bs_vector3<U>& v) {
        for (size_t i = 0; i < v.size; ++i)
            elem[i] = T(v.elem[i]);
    }

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
        return *((bs_vector2<T>*) _xy);
    }
    bs_vector2<T>& xy() {
        return *((bs_vector2<T>*) _xy);
    }
    const bs_vector2<T>& yz() const {
        return *((bs_vector2<T>*) (elem + 1));
    }
    bs_vector2<T>& yz() {
        return *((bs_vector2<T>*) (elem + 1));
    }

    /*
        statics
    */
    static bs_vector3<T> zero;
};

/// init statics
template<typename T>
bs_vector3<T> bs_vector3<T>::zero = bs_vector3<T>(T(0), T(0), T(0));

typedef bs_vector3<float>   bs_vector3f;
typedef bs_vector3<real>  bs_vector3d;

typedef bs_vector3f        vector;

/*
        types like in GLSL
    */

typedef bs_vector3<float>   vec3;
typedef bs_vector3<real>    dvec3; // currently not supported in GLSL but a reserved keyword
typedef bs_vector3<int>     ivec3;
typedef bs_vector3<size_t>  svec3;
typedef bs_vector3<bool>    bvec3;

typedef bs_vector3<int>  isize3;
typedef bs_vector3<float> fsize3;
typedef bs_vector3<real> dsize3;

typedef bs_vector3<int>  ipoint3;
typedef bs_vector3<float> fpoint3;
typedef bs_vector3<real> dpoint3;

template<typename T>
inline bs_vector3<T> perpendicular( const bs_vector3<T>& src, const bs_vector3<T>& vec )  {
    T crossX = src.y*vec.z - src.z*vec.y;
    T crossY = src.z*vec.x - src.x*vec.z;
    T crossZ = src.x*vec.y - src.y*vec.x;

    T length = (float)sqrt(crossX*crossX +
                           crossY*crossY +
                           crossZ*crossZ);

    if( length > 0 )
        return bs_vector3<T>( crossX/length, crossY/length, crossZ/length );
    else
        return bs_vector3<T>();
}

template<typename T>
inline bs_vector3<T> crossed( const bs_vector3<T>& src ,const bs_vector3<T>& vec )  {
    return bs_vector3<T>( src.y*vec.z - src.z*vec.y,
                          src.z*vec.x - src.x*vec.z,
                          src.x*vec.y - src.y*vec.x );
}
template<typename T>
inline bs_vector3<T> rotatedRad(const bs_vector3<T> &src, T angle, const bs_vector3<T>& axis )
{
    bs_vector3<T> ax = normalize(axis);
    T a = angle;
    T sina = sin( a );
    T cosa = cos( a );
    T cosb = (T)1 - cosa;

    return bs_vector3<T>( src.x*(ax.x*ax.x*cosb + cosa)
                          + src.y*(ax.x*ax.y*cosb - ax.z*sina)
                          + src.z*(ax.x*ax.z*cosb + ax.y*sina),
                          src.x*(ax.y*ax.x*cosb + ax.z*sina)
                          + src.y*(ax.y*ax.y*cosb + cosa)
                          + src.z*(ax.y*ax.z*cosb - ax.x*sina),
                          src.x*(ax.z*ax.x*cosb - ax.y*sina)
                          + src.y*(ax.z*ax.y*cosb + ax.x*sina)
                          + src.z*(ax.z*ax.z*cosb + cosa) );
}

/*
        Prepare to implement 3 * 5 * 8 = 120 operators and dozens of functions
        the lazy way with evil voodoo macro magic
    */
template<typename T> inline bs_vector3<T> operator + (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] + v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector3<T> operator + (const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] + t;
    return vRes;
}
template<typename T> inline bs_vector3<T> operator + (T t, const bs_vector3<T>& v)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t + v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector3<T>& operator += (bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] += v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector3<T>& operator += (bs_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] += t;
    return v;
}

template<typename T> inline bs_vector3<T> operator - (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] - v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector3<T> operator - (const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] - t;
    return vRes;
}
template<typename T> inline bs_vector3<T> operator - (T t, const bs_vector3<T>& v)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t - v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector3<T>& operator -= (bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] -= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector3<T>& operator -= (bs_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] -= t;
    return v;
}

template<typename T> inline bs_vector3<T> operator * (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] * v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector3<T> operator * (const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] * t;
    return vRes;
}
template<typename T> inline bs_vector3<T> operator * (T t, const bs_vector3<T>& v)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t * v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector3<T>& operator *= (bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] *= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector3<T>& operator *= (bs_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] *= t;
    return v;
}

template<typename T> inline bs_vector3<T> operator / (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] / v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector3<T> operator / (const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] / t;
    return vRes;
}
template<typename T> inline bs_vector3<T> operator / (T t, const bs_vector3<T>& v)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t / v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector3<T>& operator /= (bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] /= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector3<T>& operator /= (bs_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] /= t;
    return v;
}

template<typename T> inline bs_vector3<T> operator % (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] % v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector3<T> operator % (const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] % t;
    return vRes;
}
template<typename T> inline bs_vector3<T> operator % (T t, const bs_vector3<T>& v)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t % v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector3<T>& operator %= (bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] %= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector3<T>& operator %= (bs_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] %= t;
    return v;
}

template<typename T> inline bs_vector3<T> operator & (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] & v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector3<T> operator & (const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] & t;
    return vRes;
}
template<typename T> inline bs_vector3<T> operator & (T t, const bs_vector3<T>& v)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t & v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector3<T>& operator &= (bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] &= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector3<T>& operator &= (bs_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] &= t;
    return v;
}

template<typename T> inline bs_vector3<T> operator | (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] | v2.elem[i];
    return vRes;
}

template<typename T> inline bs_vector3<T> operator | (const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] | t;
    return vRes;
}
template<typename T> inline bs_vector3<T> operator | (T t, const bs_vector3<T>& v)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t | v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector3<T>& operator |= (bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] |= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector3<T>& operator |= (bs_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] |= t;
    return v;
}

template<typename T> inline bs_vector3<T> operator ^ (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] ^ v2.elem[i];
    return vRes;
}


template<typename T> inline bs_vector3<T> operator ^ (const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] ^ t;
    return vRes;
}
template<typename T> inline bs_vector3<T> operator ^ (T t, const bs_vector3<T>& v)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t ^ v.elem[i];
    return vRes;
}
template<typename T> inline bs_vector3<T>& operator ^= (bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] ^= v2.elem[i];
    return v1;
}
template<typename T> inline bs_vector3<T>& operator ^= (bs_vector3<T>& v, T t)
{
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] ^= t;
    return v;
}

/*
        relational operators
    */
template<typename T>
inline bs_vector3<bool> lessThan (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] < v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector3<bool> lessThanEqual (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] <= v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector3<bool> greaterThan (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] > v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector3<bool> greaterThanEqual (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] >= v2.elem[i]);
    return vRes;
}
template<typename T>
inline bs_vector3<bool> equal (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] == v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_vector3<bool> notEqual (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] != v2.elem[i]);
    return vRes;
}

template<typename T>
inline bool operator == (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] == v2.elem[i]);
    return result;
}

template<typename T>
inline bool operator != (const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    return !(v1 == v2);
}


/*
        further vector functions
    */
template<typename T> inline T dot(const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    T result = 0;
    for (size_t i = 0; i < v1.size; ++i)
        result += v1.elem[i] * v2.elem[i];
    return result;
}

template<typename T> inline T lengthSq(const bs_vector3<T>& v)
{
    T result = 0;
    for (size_t i = 0; i < v.size; ++i)
        result += (v.elem[i] * v.elem[i]);
    return result;
}

template<typename T> inline T length(const bs_vector3<T>& v)
{
    return std::sqrt(lengthSq(v));
}

template<typename T> inline bs_vector3<T> normalize(const bs_vector3<T>& v)
{
    bs_vector3<T> vRes(v);
    T recipLength = 1 / length(v);
    vRes *= recipLength;
    return vRes;
}

template<typename T> inline T distance(const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    return length(v2 - v1);
}

template<typename T> inline T distanceSq(const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    return lengthSq(v2 - v1);
}

template<typename T> inline bs_vector3<T> operator - (const bs_vector3<T>& v)
{
    bs_vector3<T> vRes(v);
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = -v.elem[i];
    return vRes;
}

template<typename T> inline bs_vector3<T> min(const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = bs_min(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_vector3<T> max(const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = bs_max(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_vector3<T> min(const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = bs_min(v.elem[i], t);
    return vRes;
}

template<typename T> inline bs_vector3<T> max(const bs_vector3<T>& v, T t)
{
    bs_vector3<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = bs_max(v.elem[i], t);
    return vRes;
}

template<typename T> inline T min(const bs_vector3<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = bs_min(v.elem[i], res);
    return res;
}

template<typename T> inline T max(const bs_vector3<T>& v)
{
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = bs_max(v.elem[i], res);
    return res;
}

template<typename T> inline bs_vector3<T> clamp(const bs_vector3<T>& v, const bs_vector3<T>& minVal, const bs_vector3<T>& maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline bs_vector3<T> clamp(const bs_vector3<T>& v, T minVal, T maxVal)
{
    return min( max(v, minVal), maxVal );
}

template<typename T> inline bs_vector3<T> ceil(const bs_vector3<T>& v)
{
    using std::ceil;
    bs_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ceil(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<T> floor(const bs_vector3<T>& v)
{
    using std::floor;
    bs_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = floor(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<T> fix(const bs_vector3<T>& v)
{
    bs_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = fix(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<T> round(const bs_vector3<T>& v)
{
    bs_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = round(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<T> sign(const bs_vector3<T>& v) {
    bs_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = sign(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<int> iceil(const bs_vector3<T>& v)
{
    bs_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iceil(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<int> ifloor(const bs_vector3<T>& v)
{
    bs_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifloor(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<int> iround(const bs_vector3<T>& v)
{
    bs_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = iround(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<int> ifix(const bs_vector3<T>& v)
{
    bs_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ifix(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<int> isign(const bs_vector3<T>& v)
{
    bs_vector3<int> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = isign(v.elem[i]);
    return result;
}

template<typename T> inline bs_vector3<T> mix(const bs_vector3<T>& v, const bs_vector3<T>& v2, T t)
{
    bs_vector3<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result.elem[i] = (v.elem[i]*(1-t)) + (v2.elem[i]*t);
    return result;
}

/*
        horizontally working functions
    */
template<typename T> inline T hadd(const bs_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result += v.elem[i];
    return result;
}

template<typename T> inline T hsub(const bs_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result -= v.elem[i];
    return result;
}

template<typename T> inline T hmul(const bs_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result *= v.elem[i];
    return result;
}

template<typename T> inline T hdiv(const bs_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result /= v.elem[i];
    return result;
}

template<typename T> inline T hmod(const bs_vector3<T>& v)
{
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result %= v.elem[i];
    return result;
}

template<typename T> inline bool hand(const bs_vector3<T>& v)
{
    bool result = v.elem[0] && v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result &= v.elem[i];
    return result;
}

template<typename T> inline bool hor(const bs_vector3<T>& v)
{
    T result = v.elem[0] || v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result |= v.elem[i];
    return result;
}

template<typename T> inline bool hxor(const bs_vector3<T>& v)
{
    T result = v.elem[0] ^ v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result ^= v.elem[i];
    return result;
}


/**
        cross product
    */
template<typename T>
inline bs_vector3<T> cross(const bs_vector3<T>& v1, const bs_vector3<T>& v2)
{
    bs_vector3<T> vResult;
    vResult.elem[0] = v1.elem[1] * v2.elem[2] - v1.elem[2] * v2.elem[1];
    vResult.elem[1] = v1.elem[2] * v2.elem[0] - v1.elem[0] * v2.elem[2];
    vResult.elem[2] = v1.elem[0] * v2.elem[1] - v1.elem[1] * v2.elem[0];
    return vResult;
}

/*
        Component-wise absolute value
    */

template<typename T>
inline bs_vector3<T> abs(const bs_vector3<T>& v) {
    using std::abs;
    return bs_vector3<T>(fabs(v.x), fabs(v.y), fabs(v.z));
}


/*
        specialization for length(VectorX<int>)
    */

inline float length(const bs_vector3<int>& v) {
    return std::sqrt( float(lengthSq(v)) );
}

/*
         stream operators
    */

/// output bs_vector3d to stream
template<typename T>
std::ostream& operator << (std::ostream& s, const bs_vector3<T>& v) {
    return (s << "[" << v.elem[0] << " " << v.elem[1] << " " << v.elem[2] << "]");
}

// specialication for (unsigned) int, to make it output numbers and not characters

template<>
inline std::ostream& operator << <unsigned char>(std::ostream& s, const bs_vector3<uchar>& v) {
    return (s << bs_vector3<int>(v));
}

template<>
inline std::ostream& operator << <char>(std::ostream& s, const bs_vector3<char>& v) {
    return (s << bs_vector3<int>(v));
}

/// input bs_vector3d from stream
template<typename T>
std::istream& operator >> (std::istream& s, bs_vector3<T>& v) {
    char ch;

    s >> ch;
    if (ch == '[') {  // format "[ x y z ]"
        s >> v[0] >> v[1] >> v[2] >> ch;
        if (ch != ']') {
            s.clear(std::ios::badbit);
        }
    } else { // format "x y z"
        s.putback(ch);
        s >> v[0] >> v[1] >> v[2];
    }

    return s;
}



template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_vector3<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_vector3<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_vector3<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_vector3<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
#endif // VEC3_H
