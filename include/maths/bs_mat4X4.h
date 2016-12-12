#ifndef MAT4X4_H
#define MAT4X4_H
#include "bs_vec4.h"

/**
        bs_matrix4X4 class. Works basicly like the matrix stuff in GLSL.
        But remember that this is a ROW-MATRIX and NOT a COL-MATRIX
        like in OpenGL or GLSL respectively.
    */
template<typename T>
struct bs_matrix4X4 {
    typedef T ElemType;

    enum {
        size = 16,
        cols = 4,
        rows = 4
    };
    typedef bs_vector4<T> RowType;

    union {
        struct {
            T t00, t01, t02, t03;
            T t10, t11, t12, t13;
            T t20, t21, t22, t23;
            T t30, t31, t32, t33;
        };
        T elemRowCol[4][4];
        T elem[16];
    };

    /*
        constructors
    */
    /// default constructor
    bs_matrix4X4() {}
    /// Init all elements with the same value
    explicit bs_matrix4X4(T t) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = t;
    }
    /// Init with another Matrix of another type
    template<typename U>
    bs_matrix4X4(const bs_matrix4X4<U>& m) {
        for (size_t i = 0; i < m.size; ++i)
            elem[i] = T(m.elem[i]);
    }
    /// Init from array with equal size
    explicit bs_matrix4X4(const T* t) {
        for (size_t i = 0; i < size; ++i)
            elem[i] = t[i];
    }
    /// Init componentwisely
    bs_matrix4X4(T _t00, T _t01, T _t02, T _t03,
                 T _t10, T _t11, T _t12, T _t13,
                 T _t20, T _t21, T _t22, T _t23,
                 T _t30, T _t31, T _t32, T _t33) {
        t00 = _t00; t01 = _t01; t02 = _t02; t03 = _t03;
        t10 = _t10; t11 = _t11; t12 = _t12; t13 = _t13;
        t20 = _t20; t21 = _t21; t22 = _t22; t23 = _t23;
        t30 = _t30; t31 = _t31; t32 = _t32; t33 = _t33;
    }
    /// Init with four Vector4
    bs_matrix4X4(const bs_vector4<T>& v1, const bs_vector4<T>& v2,
                 const bs_vector4<T>& v3, const bs_vector4<T>& v4) {
        for (size_t i = 0; i < v1.size; ++i)
            elem[0*rows + i] = v1.elem[i];
        for (size_t i = 0; i < v2.size; ++i)
            elem[1*rows + i] = v2.elem[i];
        for (size_t i = 0; i < v3.size; ++i)
            elem[2*rows + i] = v3.elem[i];
        for (size_t i = 0; i < v4.size; ++i)
            elem[3*rows + i] = v4.elem[i];
    }

    /// Return the upper left 3x3-submatrix in a 4x4 matrix
    bs_matrix4X4<T> getRotationalPart() const {
        bs_matrix4X4<T> result(T(0));
        result.t00 = t00; result.t01  = t01; result.t02 = t02;
        result.t10 = t10; result.t11  = t11; result.t12 = t12;
        result.t20 = t20; result.t21  = t21; result.t22 = t22;
        result.t33 = T(1);
        return result;
    }

    /// Return the main diagonal of the upper left 3x3-submatrix
    bs_vector3<T> getScalingPart() const {
        bs_vector3<T> result(T(0));
        result.elem[0] = t00;
        result.elem[1] = t11;
        result.elem[2] = t22;
        return result;
    }
    /*
        create special matrices
    */
    static const bs_matrix4X4<T> zero;
    static const bs_matrix4X4<T> identity;

    static bs_matrix4X4<T> createZero() {
        return zero;
    }

    static bs_matrix4X4<T> createIdentity() {
        return identity;
    }

    static bs_matrix4X4<T> createTranslation(const bs_vector3<T>& v) {
        bs_matrix4X4<T> result = identity;
        result.t03 = v.elem[0];
        result.t13 = v.elem[1];
        result.t23 = v.elem[2];
        return result;
    }

    static bs_matrix4X4<T> createScale(const bs_vector3<T>& v) {
        bs_matrix4X4<T> result = identity;
        result.t00 = v.elem[0];
        result.t11 = v.elem[1];
        result.t22 = v.elem[2];
        return result;
    }

    /**
         * Rotation the x-axis as rotation axis.
         * The angle is inem RADIAN AND NOTem DEGREE.
         *
         * @param angle The angle of rotation inem radian.
         */
    static bs_matrix4X4<T> createRotationX(T angle) {
        using namespace std; // use overloaded cos and sin
        bs_matrix4X4<T> result = identity;
        result.t11 =  cos(angle);
        result.t12 = -sin(angle);
        result.t21 =  sin(angle);
        result.t22 =  cos(angle);
        return result;
    }

    /**
         * Rotation the y-axis as rotation axis.
         * The angle is inem RADIAN AND NOTem DEGREE.
         *
         * @param angle The angle of rotation inem radian.
         */
    static bs_matrix4X4<T> createRotationY(T angle) {
        using namespace std; // use overloaded cos and sin
        bs_matrix4X4<T> result = identity;
        result.t00 =  cos(angle);
        result.t02 =  sin(angle);
        result.t20 = -sin(angle);
        result.t22 =  cos(angle);
        return result;
    }

    /**
         * Rotation the z-axis as rotation axis.
         * The angle is inem RADIAN AND NOTem DEGREE.
         *
         * @param angle The angle of rotation inem radian.
         */
    static bs_matrix4X4<T> createRotationZ(T angle) {
        using namespace std; // use overloaded cos and sin
        bs_matrix4X4<T> result = identity;
        result.t00 =  cos(angle);
        result.t01 = -sin(angle);
        result.t10 =  sin(angle);
        result.t11 =  cos(angle);
        return result;
    }

    /**
         * Simliar to glRotate butp angle is inem RADIAN AND NOTem DEGREE.
         *
         * @param angle The angle inem radian.
         * @param axis The axis of rotation. Needn't be in unit length.
         */
    static bs_matrix4X4<T> createRotation(T angle, bs_vector3<T> axis);

    /**
        * This function returns the Matrix that gluLookAt would create and put onto the OpenGL
        * Matrix-Stack, given the position, focus-point, and up-Vector of a camera.
        */
    static bs_matrix4X4<T> createLookAt(const bs_vector3<T>& eye, const bs_vector3<T>& focus, const bs_vector3<T>& up);

    /**
         * This function provides a glFrustum() replacement.  In case you just need the matrix that glFrustum would
         * produce, you can use this function instead of putting the matrix on the OpenGL stack and then reading it.
         */
    static bs_matrix4X4<T> createFrustum(T left, T right, T top, T bottom, T pnear, T pfar);

    /**
         * This function provides a gluPerspective() replacement.
         * See its documentation for an explanation of the parameters.em BUT:
         *p fov expectsem RADIAN and notem DEGREE.
         */
    static bs_matrix4X4<T> createPerspective(T fov, T aspect, T pnear, T pfar);

    static bs_matrix4X4<T> createOrtho(T left, T right, T top, T bottom, T pnear, T pfar);

    /*
        index operator
    */

    /*
            The C++ standard forbids typenamees with non trivial constructors
            as union members. Thus this hack provides functionality similar
            to the GLSL matrix index operator.
        */
    /// return arbitrary row-vector similar to GLSL. <br>
    /// BUT REMEMBER: GLSL gives you COLS NOT ROWS!
    const bs_vector4<T>& operator [] (size_t i) const {
        bsASSERT(i >= size, "i must be less than size");
        return *((bs_vector4<T>*) elemRowCol[i]);
    }
    /// return arbitrary row-vector similar to GLSL. <br>
    /// BUT REMEMBER: GLSL gives you COLS NOT ROWS!
    bs_vector4<T>& operator [] (size_t i) {
        bsASSERT(i >= size, "i must be less than size");
        return *((bs_vector4<T>*) elemRowCol[i]);
    }

    /**
         * inverts the matrix.
         * @return true - if successful -- otherwise false
        */
    bool invert(bs_matrix4X4<T>& result) const;
};

/*
        init statics
    */
template<typename T>
const bs_matrix4X4<T> bs_matrix4X4<T>::zero = bs_matrix4X4<T>
        (
            T(0), T(0), T(0), T(0),
            T(0), T(0), T(0), T(0),
            T(0), T(0), T(0), T(0),
            T(0), T(0), T(0), T(0)
            );

template<typename T>
const bs_matrix4X4<T> bs_matrix4X4<T>::identity = bs_matrix4X4<T>
        (
            T(1), T(0), T(0), T(0),
            T(0), T(1), T(0), T(0),
            T(0), T(0), T(1), T(0),
            T(0), T(0), T(0), T(1)
            );


typedef bs_matrix4X4<float>  bs_mat4X4f;
typedef bs_matrix4X4<real> bs_mat4X4d;

typedef bs_mat4X4f        bs_matrix;
typedef bs_mat4X4f        mat4;
typedef bs_mat4X4f        fmat4;
typedef bs_mat4X4d        dmat4;


template<typename T>
bs_matrix4X4<T> bs_matrix4X4<T>::createRotation(T angle, bs_vector3<T> axis) {
    using namespace std; // use overloaded cos and sin
    axis = normalize(axis);
    T s = sin(angle);
    T c = cos(angle);
    T x = axis.x;
    T y = axis.y;
    T z = axis.z;

    bs_matrix4X4<T> result
            (
                x*x*(1-c) + c,      x*y*(1-c) - z*s,    x*z*(1-c) + y*s, T(0),
                x*y*(1-c) + z*s,    y*y*(1-c) + c,      y*z*(1-c) - x*s, T(0),
                x*z*(1-c) - y*s,    y*z*(1-c) + x*s,    z*z*(1-c) + c,   T(0),
                T(0),               T(0),               T(0),            T(1)
                );

    return result;
}

/*
        createFrustum, createPerspective and createLookAt
    */

template<typename T>
bs_matrix4X4<T> bs_matrix4X4<T>::createLookAt(const bs_vector3<T>& eye, const bs_vector3<T>& focus, const bs_vector3<T>& up) {

    bs_vector3<T> look   = normalize(focus - eye);
    bs_vector3<T> strafe = normalize(cross(look, normalize(up)));
    bs_vector3<T> up2    = cross(strafe, look);

    bs_matrix4X4<T> m(bs_vector4<T>( strafe, T(0) ),
                      bs_vector4<T>( up2, T(0) ),
                      bs_vector4<T>( -look, T(0) ),
                      bs_vector4<T>( T(0), T(0), T(0), T(1) ));

    return m * bs_matrix4X4<T>::createTranslation(-eye);
}

template<typename T>
bs_matrix4X4<T> bs_matrix4X4<T>::createFrustum(T left, T right, T bottom, T top, T pnear, T pfar)
{
    bs_matrix4X4<T> m(
                T(2)*pnear/(right-left),        T(0),               (right+left)/(right-left),             T(0),
                T(0),           T(2)*pnear/(top-bottom),    (top+bottom)/(top-bottom),             T(0),
                T(0),                   T(0),               (pnear+pfar)/(pnear-pfar),  (T(2)*pfar*pnear)/(pnear-pfar),
                T(0),                   T(0),                          -T(1),                      T(0)
                );

    return m;
}

template<typename T>
bs_matrix4X4<T> bs_matrix4X4<T>::createPerspective(T fov, T aspect, T pnear, T pfar)
{
    T f = T(1) / tan( fov/T(2) );
    bs_matrix4X4<T> m(
                f / aspect,         T(0),            T(0),                          T(0),
                T(0),             f,             T(0),                          T(0),
                T(0),           T(0),   (pnear+pfar)/(pnear-pfar),  (T(2)*pfar*pnear)/(pnear-pfar),
                T(0),           T(0),           -T(1),                          T(0)
                );

    return m;
}

template<typename T>
bs_matrix4X4<T> bs_matrix4X4<T>::createOrtho(T left, T right, T top, T bottom, T pnear, T pfar)
{
    T width = right - left;
    T invheight = top - bottom;
    T clip = pfar - pnear;
    bs_matrix4X4<T> m(
                T(2)/(width),      T(0),              T(0),          -(left+right)/(width),
                T(0),         T(2)/(invheight),      T(0),          -(top+bottom)/(invheight),
                T(0),              T(0),         T(-2)/(clip), -(pfar+pnear)/(clip),
                T(0),              T(0),              T(0),                      T(1)
                );

    return m;
}


/*
        Prepare to implement 3 * 5 * 7 = 105 operators and a couple of functions
        the lazy way with evil voodoo macro magic
    */

template<typename T> inline bs_matrix4X4<T> transpose(const bs_matrix4X4<T>& m)
{
    bs_matrix4X4<T> mRes;
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
template<typename T> inline typename bs_matrix4X4<T>::RowType operator * (const bs_matrix4X4<T>& m, const typename bs_matrix4X4<T>::RowType& v)
{
    typename bs_matrix4X4<T>::RowType vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes[i] = dot(m[i], v);
    return vRes;
}

template<typename T> inline typename bs_matrix4X4<T>::RowType operator * (const typename bs_matrix4X4<T>::RowType& v, const bs_matrix4X4<T>& m)
{
    typename bs_matrix4X4<T>::RowType vRes;
    bs_matrix4X4<T> transposed = transpose(m);
    for (size_t i = 0; i < v.size; ++i)
        vRes[i] = dot(v, transposed[i]);
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator * (const bs_matrix4X4<T>& m1, const bs_matrix4X4<T>& m2)
{
    bs_matrix4X4<T> mRes;
    bs_matrix4X4<T> transposed = transpose(m2);
    for (size_t row = 0; row < m1.rows; ++row)
        for (size_t col = 0; col < m1.cols; ++col)
            mRes.elemRowCol[row][col] =
                    dot(m1[row], transposed[col]);
    return mRes;
}

template<typename T> inline bs_matrix4X4<T>& operator *= (bs_matrix4X4<T>& m1, const bs_matrix4X4<T>& m2)
{
    bs_matrix4X4<T> temp = m1;
    m1 = temp * m2;
    return m1;
}

template<typename T> inline bs_matrix4X4<T> matrixCompMult (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2)
{
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] * v2.elem[i];
    return vRes;
}

//--------------------------------------------------------------

template<typename T> inline bs_matrix4X4<T> operator - (const bs_matrix4X4<T>& v)
{
    bs_matrix4X4<T> vRes(v);
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = -v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> min(const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = std::min(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> max(const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = std::max(v1.elem[i], v2.elem[i]);
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> floor(const bs_matrix4X4<T>& v) {
    using std::floor;
    bs_matrix4X4<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = floor(v.elem[i]);
    return result;
}

template<typename T> inline bs_matrix4X4<T> ceil(const bs_matrix4X4<T>& v) {
    using std::ceil;
    bs_matrix4X4<T> result;
    for (size_t i = 0; i < v.size; ++i)
        result[i] = ceil(v.elem[i]);
    return result;
}

template<typename T> inline bs_matrix4X4<T> min(const bs_matrix4X4<T>& v, T t) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = std::min(v.elem[i], t);
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> max(const bs_matrix4X4<T>& v, T t) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = std::max(v.elem[i], t);
    return vRes;
}

template<typename T> inline T min(const bs_matrix4X4<T>& v) {
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = std::min(v.elem[i], res);
    return res;
}

template<typename T> inline T max(const bs_matrix4X4<T>& v) {
    T res = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        res = std::max(v.elem[i], res);
    return res;
}

template<typename T> inline bs_matrix4X4<T> clamp(const bs_matrix4X4<T>& v, const bs_matrix4X4<T>& minVal, const bs_matrix4X4<T>& maxVal) {
    return min( max(v, minVal), maxVal );
}

template<typename T> inline bs_matrix4X4<T> clamp(const bs_matrix4X4<T>& v, T minVal, T maxVal) {
    return min( max(v, minVal), maxVal );
}

template<typename T> inline T hadd(const bs_matrix4X4<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result += v.elem[i];
    return result;
}

template<typename T> inline T hsub(const bs_matrix4X4<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result -= v.elem[i];
    return result;
}

template<typename T> inline T hmul(const bs_matrix4X4<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result *= v.elem[i];
    return result;
}

template<typename T> inline T hdiv(const bs_matrix4X4<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result /= v.elem[i];
    return result;
}

template<typename T> inline T hmod(const bs_matrix4X4<T>& v) {
    T result = v.elem[0];
    for (size_t i = 1; i < v.size; ++i)
        result %= v.elem[i];
    return result;
}

template<typename T> inline bool hand(const bs_matrix4X4<T>& v) {
    bool result = v.elem[0] && v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result &= v.elem[i];
    return result;
}

template<typename T> inline bool hor(const bs_matrix4X4<T>& v) {
    T result = v.elem[0] || v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result |= v.elem[i];
    return result;
}

template<typename T> inline bool hxor(const bs_matrix4X4<T>& v) {
    T result = v.elem[0] ^ v.elem[1];
    for (size_t i = 2; i < v.size; ++i)
        result ^= v.elem[i];
    return result;
}

template<typename T>
inline bool operator == (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bool result = true;
    for (size_t i = 0; i < v1.size; ++i)
        result &= (v1.elem[i] == v2.elem[i]);
    return result;
}

template<typename T>
inline bool operator != (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    return !(v1 == v2);
}

template<typename T>
inline bs_matrix4X4<bool> lessThan (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] < v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix4X4<bool> lessThanEqual (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] <= v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix4X4<bool> greaterThan (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] > v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix4X4<bool> greaterThanEqual (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] >= v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix4X4<bool> equal (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] == v2.elem[i]);
    return vRes;
}

template<typename T>
inline bs_matrix4X4<bool> notEqual (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<bool> vRes;
    for (size_t i = 0; i < vRes.size; ++i)
        vRes.elem[i] = (v1.elem[i] != v2.elem[i]);
    return vRes;
}

//------------------------------------------------------
template<typename T> inline bs_matrix4X4<T> operator + (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] + v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator + (const bs_matrix4X4<T>& v, T t) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] + t;
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator + (T t, const bs_matrix4X4<T>& v) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t + v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T>& operator += (bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] += v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix4X4<T>& operator += (bs_matrix4X4<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] += t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix4X4<T> operator - (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] - v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator - (const bs_matrix4X4<T>& v, T t) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] - t;
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator - (T t, const bs_matrix4X4<T>& v) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t - v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T>& operator -= (bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] -= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix4X4<T>& operator -= (bs_matrix4X4<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] -= t;
    return v;
}

//
template<typename T> inline bs_matrix4X4<T> operator * (const bs_matrix4X4<T>& v, T t) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] * t;
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator * (T t, const bs_matrix4X4<T>& v) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t * v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T>& operator *= (bs_matrix4X4<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] *= t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix4X4<T> operator / (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] / v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator / (const bs_matrix4X4<T>& v, T t) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] / t;
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator / (T t, const bs_matrix4X4<T>& v) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t / v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T>& operator /= (bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] /= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix4X4<T>& operator /= (bs_matrix4X4<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] /= t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix4X4<T> operator & (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] & v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator & (const bs_matrix4X4<T>& v, T t) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] & t;
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator & (T t, const bs_matrix4X4<T>& v) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t & v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T>& operator &= (bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] &= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix4X4<T>& operator &= (bs_matrix4X4<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] &= t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix4X4<T> operator | (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] | v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator | (const bs_matrix4X4<T>& v, T t) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] | t;
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator | (T t, const bs_matrix4X4<T>& v) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t | v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T>& operator |= (bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] |= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix4X4<T>& operator |= (bs_matrix4X4<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] |= t;
    return v;
}

//------------------------------------------------------
template<typename T> inline bs_matrix4X4<T> operator ^ (const bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v1.size; ++i)
        vRes.elem[i] = v1.elem[i] ^ v2.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator ^ (const bs_matrix4X4<T>& v, T t) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = v.elem[i] ^ t;
    return vRes;
}

template<typename T> inline bs_matrix4X4<T> operator ^ (T t, const bs_matrix4X4<T>& v) {
    bs_matrix4X4<T> vRes;
    for (size_t i = 0; i < v.size; ++i)
        vRes.elem[i] = t ^ v.elem[i];
    return vRes;
}

template<typename T> inline bs_matrix4X4<T>& operator ^= (bs_matrix4X4<T>& v1, const bs_matrix4X4<T>& v2) {
    for (size_t i = 0; i < v1.size; ++i)
        v1.elem[i] ^= v2.elem[i];
    return v1;
}

template<typename T> inline bs_matrix4X4<T>& operator ^= (bs_matrix4X4<T>& v, T t) {
    for (size_t i = 0; i < v.size; ++i)
        v.elem[i] ^= t;
    return v;
}

/// ostream-operator
template<typename T>
std::ostream& operator << (std::ostream& s, const bs_matrix4X4<T>& m) {
    return (s
            << "| " << m.elem[ 0] << " " << m.elem[ 1] << " " << m.elem[ 2] << " " << m.elem[ 3] << " |" << std::endl
                                  << "| " << m.elem[ 4] << " " << m.elem[ 5] << " " << m.elem[ 6] << " " << m.elem[ 7] << " |" << std::endl
                                  << "| " << m.elem[ 8] << " " << m.elem[ 9] << " " << m.elem[10] << " " << m.elem[11] << " |" << std::endl
                                  << "| " << m.elem[12] << " " << m.elem[13] << " " << m.elem[14] << " " << m.elem[15] << " |" << std::endl);
}

/// This operator is not available in GLSL but very usefull: A mat4 * vec3 operator, returning a vec3
/// note: the resulting vec3 is divided by the w component of the temporary result!
template<typename T>
bs_vector3<T> operator * (const bs_matrix4X4<T>& m, const bs_vector3<T>& v) {
    bs_vector4<T> v4(v, 1);
    v4 = m * v4;
    v4 /= v4.w;
    return bs_vector3<T>(v4.elem);
}

// addtional matrix functions

template<typename T>
bool bs_matrix4X4<T>::invert(bs_matrix4X4<T>& result) const {
    using std::abs; // use overloaded abs
    T t;
    bs_matrix4X4<T> tmp = *this;
    result = bs_matrix4X4<T>::identity;

    for (size_t i = 0; i < 4; ++i) {
        // Look for largest element in column
        size_t swap = i;
        for (size_t j = i + 1; j < 4; ++j) {
            if (fabs(tmp.elemRowCol[j][i]) > fabs(tmp.elemRowCol[i][i]))
                swap = j;
        }

        if (swap != i) {
            // Swap rows.
            for (size_t k = 0; k < 4; ++k) {
                t = tmp.elemRowCol[i][k];
                tmp.elemRowCol[i][k] = tmp.elemRowCol[swap][k];
                tmp.elemRowCol[swap][k] = t;

                t = result.elem[i*4+k];
                result.elem[i*4+k] = result.elem[swap*4+k];
                result.elem[swap*4+k] = t;
            }
        }

        if (tmp.elemRowCol[i][i] == 0) {
            // The matrix is singular
            return false;
        }

        t = tmp.elemRowCol[i][i];
        for (size_t k = 0; k < 4; k++) {
            tmp.elemRowCol[i][k] /= t;
            result.elem[i*4+k] /= t;
        }
        for (size_t j = 0; j < 4; j++) {
            if (j != i) {
                t = tmp.elemRowCol[j][i];

                for (size_t k = 0; k < 4; k++) {
                    tmp.elemRowCol[j][k] -= tmp.elemRowCol[i][k]*t;
                    result.elem[j*4+k] -= result.elem[i*4+k]*t;
                }
            }
        }
    }
    return true;
}
typedef bs_mat4X4f bs_xform;

template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_matrix4X4<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_matrix4X4<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_matrix4X4<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        in << v.elem[i];
    return in;
}
template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_matrix4X4<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
#endif // MAT4X4_H
