#ifndef bs_matrix2X2_H
#define bs_matrix2X2_H

#include "bs_vec4.h"

/*
    FIXME build a SSE-optimized mat4
*/

/**
    bs_matrix2X2 class. Works basicly like the matrix stuff in GLSL.
    But remember that this is a ROW-MATRIX and NOT a COL-MATRIX
    like in OpenGL or GLSL respectively.
*/

template<typename T>
struct bs_matrix2X2
{
    typedef T ElemType;

    enum
    {
        size = 4,
        cols = 2,
        rows = 2
    };
    typedef bs_vector2<T> RowType;

    union
    {
        struct
        {
            T t00, t01;
            T t10, t11;
        };
        T elemRowCol[2][2];
        T elem[4];
    };

    bs_matrix2X2() {}
    /// Init all elements with the same value
    explicit bs_matrix2X2(T t) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = t;
    }
    /// Init with another Matrix of another type
    template<typename U>
    bs_matrix2X2(const bs_matrix2X2<U>& m) {
        for (size_t i = 0; i < m.size; ++i)
            elem[i] = T(m.elem[i]);
    }
    /// Init from array with equal size
    explicit bs_matrix2X2(const T* t) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = t[i];
    }
    /// Init componentwisely
    bs_matrix2X2(T _t00, T _t01,
                 T _t10, T _t11) {
        t00 = _t00; t01 = _t01;
        t10 = _t10; t11 = _t11;
    }
    /// Init with two Vector2
    bs_matrix2X2(const bs_vector2<T>& v1, const bs_vector2<T>& v2) {
        for (size_t i = 0; i < v1.size; ++i)
            elem[0*rows + i] = v1.elem[i];
        for (size_t i = 0; i < v2.size; ++i)
            elem[1*rows + i] = v2.elem[i];
    }

    static const bs_matrix2X2<T> zero;
    static const bs_matrix2X2<T> identity;

    static bs_matrix2X2<T> createZero() {
        return zero;
    }

    static bs_matrix2X2<T> createIdentity() {
        return identity;
    }

    static bs_matrix2X2<T> createScale(const bs_vector2<T>& v) {
        bs_matrix2X2<T> result = identity;
        result.t00 = v.elem[0];
        result.t11 = v.elem[1];
        return result;
    }

    /**
         * Creates a planar rotation with a givenp angle inem RADIAN AND NOTem DEGREE.
         *
         * @param angle The angle of rotation inem radian.
         */
    static bs_matrix2X2<T> createRotation(T angle) {
        bs_matrix2X2<T> result = identity;
        result.t00 =  cos(angle);
        result.t01 = -sin(angle);
        result.t10 =  sin(angle);
        result.t11 =  cos(angle);
        return result;
    }

    /*
        index operators
    */
    /*
            The C++ standard forbids typenamees with non trivial constructors
            as union members. Thus this hack provides functionality similar
            to the GLSL matrix index operator.
        */
    /// return arbitrary row-vector similar to GLSL. <br>
    /// BUT REMEMBER: GLSL gives you COLS NOT ROWS!
    const bs_vector2<T>& operator [] (size_t i) const
    {
        bsASSERT(i >= size, "i must be less than size");
        return *((bs_vector2<T>*) elemRowCol[i]);
    }
    /// return arbitrary row-vector similar to GLSL. <br>
    /// BUT REMEMBER: GLSL gives you COLS NOT ROWS!
    bs_vector2<T>& operator [] (size_t i)
    {
        bsASSERT(i >= size, "i must be less than size");
        return *((bs_vector2<T>*) elemRowCol[i]);
    }
};

template<typename T>
const bs_matrix2X2<T> bs_matrix2X2<T>::zero = bs_matrix2X2<T>
        (
            T(0), T(0),
            T(0), T(0)
            );

template<typename T>
const bs_matrix2X2<T> bs_matrix2X2<T>::identity = bs_matrix2X2<T>
        (
            T(1), T(0),
            T(0), T(1)
            );

/*
        typedefs for easy usage
    */

typedef bs_matrix2X2<float>  bs_mat2X2f;
typedef bs_matrix2X2<real> bs_mat2X2d;

/*
        types like in GLSL
    */
typedef bs_mat2X2f        fmat2;
typedef bs_mat2X2d        dmat2;
typedef fmat2        mat2;

/*
        Prepare to implement 3 * 5 * 7 = 105 operators and a couple of functions
        the lazy way with evil voodoo macro magic
    */

template<typename T> inline bs_matrix2X2<T> transpose(const bs_matrix2X2<T>& m)
{
    bs_matrix2X2<T> mRes;
    for (size_t row = 0; row < m.rows; ++row)
        for (size_t col = 0; col < m.cols; ++col)
            mRes.elemRowCol[col][row] = m.elemRowCol[row][col];
    return mRes;
}

/*
        Similar to GLSL make an exception with the
        Matrix-Matrix, Vector-Matrix and Matrix-Vector multiplication.
        These are "correct" operations and do not go componentwisely
    */
template<typename T> inline typename bs_matrix2X2<T>::RowType operator * (const bs_matrix2X2<T>& m, const typename bs_matrix2X2<T>::RowType& v)
{
    typename bs_matrix2X2<T>::RowType vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes[i] = dot(m[i], v);
    return vRes;
}

template<typename T> inline typename bs_matrix2X2<T>::RowType operator * (const typename bs_matrix2X2<T>::RowType& v, const bs_matrix2X2<T>& m)
{
    typename bs_matrix2X2<T>::RowType vRes;
    bs_matrix2X2<T> transposed = transpose(m);
    for (size_t i = 0; i < v.size; ++i)
        vRes[i] = dot(v, transposed[i]);
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator * (const bs_matrix2X2<T>& m1, const bs_matrix2X2<T>& m2)
{
    bs_matrix2X2<T> mRes;
    bs_matrix2X2<T> transposed = transpose(m2);
    for (size_t row = 0; row < m1.rows; ++row)
        for (size_t col = 0; col < m1.cols; ++col)
            mRes.elemRowCol[row][col] =
                    dot(m1[row], transposed[col]);
    return mRes;
}

template<typename T> inline bs_matrix2X2<T>& operator *= (bs_matrix2X2<T>& m1, const bs_matrix2X2<T>& m2)
{
    bs_matrix2X2<T> temp = m1;
    m1 = temp * m2;
    return m1;
}

template<typename T> inline bs_matrix2X2<T> matrixCompMult (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2)
{
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] * v2.elem[i];
    return vRes;
}

/*
        Implementation of bs_matrix2X2<T> operators
    */

//--------------------------------------------------------------

template<typename T> inline bs_matrix2X2<T> operator - (const bs_matrix2X2<T>& v)
{
    bs_matrix2X2<T> vRes(v);
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = -v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> min(const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = std::min(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> max(const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = std::max(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> floor(const bs_matrix2X2<T>& v) {
    using std::floor;
    bs_matrix2X2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = floor(v.elem[i]);
    return result;
}

template<typename T> inline bs_matrix2X2<T> ceil(const bs_matrix2X2<T>& v) {
    using std::ceil;
    bs_matrix2X2<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ceil(v.elem[i]);
    return result;
}

template<typename T> inline bs_matrix2X2<T> min(const bs_matrix2X2<T>& v, T t) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = std::min(v.elem[i], t);
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> max(const bs_matrix2X2<T>& v, T t) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = std::max(v.elem[i], t);
    return vRes;
}

template<typename T> inline T min(const bs_matrix2X2<T>& v) {
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = std::min(v.elem[i], res);
    return res;
}

template<typename T> inline T max(const bs_matrix2X2<T>& v) {
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = std::max(v.elem[i], res);
    return res;
}

template<typename T> inline bs_matrix2X2<T> clamp(const bs_matrix2X2<T>& v, const bs_matrix2X2<T>& minVal, const bs_matrix2X2<T>& maxVal) {
    return min( max(v, minVal), maxVal );
}

template<typename T> inline bs_matrix2X2<T> clamp(const bs_matrix2X2<T>& v, T minVal, T maxVal) {
    return min( max(v, minVal), maxVal );
}

template<typename T> inline T hadd(const bs_matrix2X2<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result += v.elem[i];
    return result;
}

template<typename T> inline T hsub(const bs_matrix2X2<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result -= v.elem[i];
    return result;
}

template<typename T> inline T hmul(const bs_matrix2X2<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result *= v.elem[i];
    return result;
}

template<typename T> inline T hdiv(const bs_matrix2X2<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result /= v.elem[i];
    return result;
}

template<typename T> inline T hmod(const bs_matrix2X2<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result %= v.elem[i];
    return result;
}

template<typename T> inline bool hand(const bs_matrix2X2<T>& v) {
    bool result = v.elem[0] && v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result &= v.elem[i];
    return result;
}

template<typename T> inline bool hor(const bs_matrix2X2<T>& v) {
    T result = v.elem[0] || v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result |= v.elem[i];
    return result;
}

template<typename T> inline bool hxor(const bs_matrix2X2<T>& v) {
    T result = v.elem[0] ^ v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result ^= v.elem[i];
    return result;
}

template<typename T>
inline bool operator == (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] == v2.elem[i]);
    return result;
}

template<typename T>
inline bool operator != (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    return !(v1 == v2);
}

template<typename T>
inline bs_matrix2X2<bool> lessThan (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] < v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix2X2<bool> lessThanEqual (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] <= v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix2X2<bool> greaterThan (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] > v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix2X2<bool> greaterThanEqual (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] >= v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix2X2<bool> equal (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] == v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix2X2<bool> notEqual (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] != v2.elem[i]);
    return vRes;
}

//------------------------------------------------------
template<typename T> inline bs_matrix2X2<T> operator + (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] + v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator + (const bs_matrix2X2<T>& v, T t) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] + t;
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator + (T t, const bs_matrix2X2<T>& v) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t + v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T>& operator += (bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] += v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix2X2<T>& operator += (bs_matrix2X2<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] += t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix2X2<T> operator - (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] - v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator - (const bs_matrix2X2<T>& v, T t) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] - t;
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator - (T t, const bs_matrix2X2<T>& v) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t - v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T>& operator -= (bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] -= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix2X2<T>& operator -= (bs_matrix2X2<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] -= t;
    return v;
}

//------------------------------------------------------


template<typename T> inline bs_matrix2X2<T> operator * (const bs_matrix2X2<T>& v, T t) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] * t;
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator * (T t, const bs_matrix2X2<T>& v) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t * v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T>& operator *= (bs_matrix2X2<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] *= t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix2X2<T> operator / (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] / v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator / (const bs_matrix2X2<T>& v, T t) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] / t;
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator / (T t, const bs_matrix2X2<T>& v) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t / v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T>& operator /= (bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] /= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix2X2<T>& operator /= (bs_matrix2X2<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] /= t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix2X2<T> operator & (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] & v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator & (const bs_matrix2X2<T>& v, T t) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] & t;
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator & (T t, const bs_matrix2X2<T>& v) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t & v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T>& operator &= (bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] &= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix2X2<T>& operator &= (bs_matrix2X2<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] &= t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix2X2<T> operator | (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] | v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator | (const bs_matrix2X2<T>& v, T t) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] | t;
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator | (T t, const bs_matrix2X2<T>& v) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t | v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T>& operator |= (bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] |= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix2X2<T>& operator |= (bs_matrix2X2<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] |= t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix2X2<T> operator ^ (const bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] ^ v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator ^ (const bs_matrix2X2<T>& v, T t) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] ^ t;
    return vRes;
}

template<typename T> inline bs_matrix2X2<T> operator ^ (T t, const bs_matrix2X2<T>& v) {
    bs_matrix2X2<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t ^ v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix2X2<T>& operator ^= (bs_matrix2X2<T>& v1, const bs_matrix2X2<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] ^= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix2X2<T>& operator ^= (bs_matrix2X2<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] ^= t;
    return v;
}

/*
        ostream operators
    */

/// ostream-operator
template<typename T>
std::ostream& operator << (std::ostream& s, const bs_matrix2X2<T>& m) {
    return (s
            << "| " << m.elem[0] << " " << m.elem[1] << " |" << std::endl
                                 << "| " << m.elem[2] << " " << m.elem[3] << " |" << std::endl);
}


template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_matrix2X2<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_matrix2X2<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_matrix2X2<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        in << v.elem[i];
    return in;
}
template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_matrix2X2<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
#endif // MATRIX2_H
