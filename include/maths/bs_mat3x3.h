#ifndef MAT3X3_H
#define MAT3X3_H
#include "bs_vec4.h"


    /**
        Matrix3 class. Works basicly like the matrix stuff in GLSL.
        But remember that this is a ROW-MATRIX and NOT a COL-MATRIX
        like in OpenGL or GLSL respectively.
    */
    template<typename T>
    struct bs_matrix3X3
    {
        typedef T ElemType;

        enum {
            size = 9,
            cols = 3,
            rows = 3
        };
        typedef bs_vector3<T> RowType;

        union {
            struct {
                T t00, t01, t02;
                T t10, t11, t12;
                T t20, t21, t22;
            };
            T elemRowCol[3][3];
            T elem[9];
        };

    /*
        constructors
    */

        /// default constructor
        bs_matrix3X3() {}
        /// Init all elements with the same value
        explicit bs_matrix3X3(T t) {
            for (size_t i = 0; i < size; ++i)
                elem[i] = t;
        }
        /// Init with another Matrix of another type
        template<typename U>
        bs_matrix3X3(const bs_matrix3X3<U>& m) {
            for (size_t i = 0; i < m.size; ++i)
                elem[i] = T(m.elem[i]);
        }
        /// Init from array with equal size
        explicit bs_matrix3X3(const T* t) {
            for (size_t i = 0; i < size; ++i)
                elem[i] = t[i];
        }
        /// Init componentwisely
        bs_matrix3X3(T _t00, T _t01, T _t02,
                T _t10, T _t11, T _t12,
                T _t20, T _t21, T _t22) {
            t00 = _t00; t01 = _t01; t02 = _t02;
            t10 = _t10; t11 = _t11; t12 = _t12;
            t20 = _t20; t21 = _t21; t22 = _t22;
        }
        /// Init with three bs_vector3
        bs_matrix3X3(const bs_vector3<T>& v1, const bs_vector3<T>& v2, const bs_vector3<T>& v3) {
            for (size_t i = 0; i < v1.size; ++i)
                elem[0*rows + i] = v1.elem[i];
            for (size_t i = 0; i < v2.size; ++i)
                elem[1*rows + i] = v2.elem[i];
            for (size_t i = 0; i < v3.size; ++i)
                elem[2*rows + i] = v3.elem[i];
        }

    /*
        create special matrices
    */
        static const bs_matrix3X3<T> zero;
        static const bs_matrix3X3<T> identity;

        static bs_matrix3X3<T> createZero() {
            return zero;
        }

        static bs_matrix3X3<T> createIdentity() {
            return identity;
        }

        static bs_matrix3X3<T> createTranslation(const bs_vector2<T>& v) {
            bs_matrix3X3<T> result = identity;
            result.t02 = v.elem[0];
            result.t12 = v.elem[1];
            return result;
        }

        static bs_matrix3X3<T> createScale(const bs_vector3<T>& v) {
            bs_matrix3X3<T> result = identity;
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
        static bs_matrix3X3<T> createRotationX(T angle) {
            bs_matrix3X3<T> result = identity;
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
        static bs_matrix3X3<T> createRotationY(T angle) {
            bs_matrix3X3<T> result = identity;
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
        static bs_matrix3X3<T> createRotationZ(T angle) {
            bs_matrix3X3<T> result = identity;
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
        static bs_matrix3X3<T> createRotation(T angle, bs_vector3<T> axis);

        /**
         * inverts the matrix.
         * @return true - if successful -- otherwise false
        */
        bool invert(bs_matrix3X3<T>& result) const;

        T det() {
            return elem[0]*elem[4]*elem[8] + elem[1]*elem[5]*elem[6] + elem[2]*elem[3]*elem[7] - elem[0]*elem[5]*elem[7] - elem[1]*elem[3]*elem[8] - elem[2]*elem[4]*elem[6];
        }
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
        const bs_vector3<T>& operator [] (size_t i) const {
            bsASSERT(i >= size, "i must be less than size");
            return *((bs_vector3<T>*) elemRowCol[i]);
        }
        /// return arbitrary row-vector similar to GLSL. <br>
        /// BUT REMEMBER: GLSL gives you COLS NOT ROWS!
        bs_vector3<T>& operator [] (size_t i) {
            bsASSERT(i >= size, "i must be less than size");
            return *((bs_vector3<T>*) elemRowCol[i]);
        }
    };

    /*
        init statics
    */

    /// init statics
    template<typename T>
    const bs_matrix3X3<T> bs_matrix3X3<T>::zero = bs_matrix3X3<T>
    (
        T(0), T(0), T(0),
        T(0), T(0), T(0),
        T(0), T(0), T(0)
    );

    template<typename T>
    const bs_matrix3X3<T> bs_matrix3X3<T>::identity = bs_matrix3X3<T>
    (
        T(1), T(0), T(0),
        T(0), T(1), T(0),
        T(0), T(0), T(1)
    );

    template<typename T>
    bool bs_matrix3X3<T>::invert(bs_matrix3X3<T>& result) const {
        using std::abs; // use overloaded abs
        result = bs_matrix3X3<T>::identity;

        T z = elem[0]*(elem[4]*elem[8] - elem[5]*elem[7]) + elem[1]*(elem[5]*elem[6] - elem[3]*elem[8]) + elem[2]*(elem[3]*elem[7] - elem[4]*elem[6]);

        if( fabs(z) < 0.00001)
            return false;

        //we have a row-matrix:
        result.elem[0] = (elem[4]*elem[8] - elem[5]*elem[7]);
        result.elem[1] = (elem[2]*elem[7] - elem[1]*elem[8]);
        result.elem[2] = (elem[1]*elem[5] - elem[2]*elem[4]);

        result.elem[3] = (elem[5]*elem[6] - elem[3]*elem[8]);
        result.elem[4] = (elem[0]*elem[8] - elem[2]*elem[6]);
        result.elem[5] = (elem[2]*elem[3] - elem[0]*elem[5]);

        result.elem[6] = (elem[3]*elem[7] - elem[4]*elem[6]);
        result.elem[7] = (elem[1]*elem[6] - elem[0]*elem[7]);
        result.elem[8] = (elem[0]*elem[4] - elem[1]*elem[3]);

        result /= z;

        return true;
    }

    typedef bs_matrix3X3<float>  bs_mat3X3f;
    typedef bs_matrix3X3<real> bs_mat3X3d;
    typedef bs_mat3X3f        mat3;
    typedef bs_mat3X3f        fmat3;
    typedef bs_mat3X3d        dmat3;

    /*
        createRotation for Matrix3<T> and Matrix4<T>
    */

    template<typename T>
    bs_matrix3X3<T> bs_matrix3X3<T>::createRotation(T angle, bs_vector3<T> axis) {
        using namespace std; // use overloaded cos and sin
        axis = normalize(axis);
        T s = sin(angle);
        T c = cos(angle);
        T x = axis.x;
        T y = axis.y;
        T z = axis.z;

        bs_matrix3X3<T> result
        (
            x*x*(1-c) + c,      x*y*(1-c) - z*s,    x*z*(1-c) + y*s,
            x*y*(1-c) + z*s,    y*y*(1-c) + c,      y*z*(1-c) - x*s,
            x*z*(1-c) - y*s,    y*z*(1-c) + x*s,    z*z*(1-c) + c
        );

        return result;
    }

    /*
        Prepare to implement 3 * 5 * 7 = 105 operators and a couple of functions
        the lazy way with evil voodoo macro magic
    */

    template<typename T> inline bs_matrix3X3<T> transpose(const bs_matrix3X3<T>& m)
    {
        bs_matrix3X3<T> mRes;
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
    template<typename T> inline typename bs_matrix3X3<T>::RowType operator * (const bs_matrix3X3<T>& m, const typename bs_matrix3X3<T>::RowType& v)
    {
        typename bs_matrix3X3<T>::RowType vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes[i] = dot(m[i], v);
        return vRes;
    }

    template<typename T> inline typename bs_matrix3X3<T>::RowType operator * (const typename bs_matrix3X3<T>::RowType& v, const bs_matrix3X3<T>& m)
    {
        typename bs_matrix3X3<T>::RowType vRes;
        bs_matrix3X3<T> transposed = transpose(m);
        for (size_t i = 0; i < v.size; ++i)
            vRes[i] = dot(v, transposed[i]);
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator * (const bs_matrix3X3<T>& m1, const bs_matrix3X3<T>& m2)
    {
        bs_matrix3X3<T> mRes;
        bs_matrix3X3<T> transposed = transpose(m2);
        for (size_t row = 0; row < m1.rows; ++row)
            for (size_t col = 0; col < m1.cols; ++col)
                mRes.elemRowCol[row][col] =
                    dot(m1[row], transposed[col]);
        return mRes;
    }

    template<typename T> inline bs_matrix3X3<T>& operator *= (bs_matrix3X3<T>& m1, const bs_matrix3X3<T>& m2)
    {
        bs_matrix3X3<T> temp = m1;
        m1 = temp * m2;
        return m1;
    }

    template<typename T> inline bs_matrix3X3<T> matrixCompMult (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2)
    {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v1.size; ++i)
            vRes.elem[i] = v1.elem[i] * v2.elem[i];
        return vRes;
    }

    //--------------------------------------------------------------

    template<typename T> inline bs_matrix3X3<T> operator - (const bs_matrix3X3<T>& v)
    {
        bs_matrix3X3<T> vRes(v);
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = -v.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> min(const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v1.size; ++i)
            vRes.elem[i] = std::min(v1.elem[i], v2.elem[i]);
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> max(const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v1.size; ++i)
            vRes.elem[i] = std::max(v1.elem[i], v2.elem[i]);
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> floor(const bs_matrix3X3<T>& v) {
        using std::floor;
        bs_matrix3X3<T> result;
        for (size_t i = 0; i < v.size; ++i)
            result[i] = floor(v.elem[i]);
        return result;
    }

    template<typename T> inline bs_matrix3X3<T> ceil(const bs_matrix3X3<T>& v) {
        using std::ceil;
        bs_matrix3X3<T> result;
        for (size_t i = 0; i < v.size; ++i)
            result[i] = ceil(v.elem[i]);
        return result;
    }

    template<typename T> inline bs_matrix3X3<T> min(const bs_matrix3X3<T>& v, T t) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = std::min(v.elem[i], t);
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> max(const bs_matrix3X3<T>& v, T t) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = std::max(v.elem[i], t);
        return vRes;
    }

    template<typename T> inline T min(const bs_matrix3X3<T>& v) {
        T res = v.elem[0];
        for (size_t i = 1; i < v.size; ++i)
            res = std::min(v.elem[i], res);
        return res;
    }

    template<typename T> inline T max(const bs_matrix3X3<T>& v) {
        T res = v.elem[0];
        for (size_t i = 1; i < v.size; ++i)
            res = std::max(v.elem[i], res);
        return res;
    }

    template<typename T> inline bs_matrix3X3<T> clamp(const bs_matrix3X3<T>& v, const bs_matrix3X3<T>& minVal, const bs_matrix3X3<T>& maxVal) {
        return min( max(v, minVal), maxVal );
    }

    template<typename T> inline bs_matrix3X3<T> clamp(const bs_matrix3X3<T>& v, T minVal, T maxVal) {
        return min( max(v, minVal), maxVal );
    }

    template<typename T> inline T hadd(const bs_matrix3X3<T>& v) {
        T result = v.elem[0];
        for (size_t i = 1; i < v.size; ++i)
            result += v.elem[i];
        return result;
    }

    template<typename T> inline T hsub(const bs_matrix3X3<T>& v) {
        T result = v.elem[0];
        for (size_t i = 1; i < v.size; ++i)
            result -= v.elem[i];
        return result;
    }

    template<typename T> inline T hmul(const bs_matrix3X3<T>& v) {
        T result = v.elem[0];
        for (size_t i = 1; i < v.size; ++i)
            result *= v.elem[i];
        return result;
    }

    template<typename T> inline T hdiv(const bs_matrix3X3<T>& v) {
        T result = v.elem[0];
        for (size_t i = 1; i < v.size; ++i)
            result /= v.elem[i];
        return result;
    }

    template<typename T> inline T hmod(const bs_matrix3X3<T>& v) {
        T result = v.elem[0];
        for (size_t i = 1; i < v.size; ++i)
            result %= v.elem[i];
        return result;
    }

    template<typename T> inline bool hand(const bs_matrix3X3<T>& v) {
        bool result = v.elem[0] && v.elem[1];
        for (size_t i = 2; i < v.size; ++i)
            result &= v.elem[i];
        return result;
    }

    template<typename T> inline bool hor(const bs_matrix3X3<T>& v) {
        T result = v.elem[0] || v.elem[1];
        for (size_t i = 2; i < v.size; ++i)
            result |= v.elem[i];
        return result;
    }

    template<typename T> inline bool hxor(const bs_matrix3X3<T>& v) {
        T result = v.elem[0] ^ v.elem[1];
        for (size_t i = 2; i < v.size; ++i)
            result ^= v.elem[i];
        return result;
    }

    template<typename T>
    inline bool operator == (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bool result = true;
        for (size_t i = 0; i < v1.size; ++i)
            result &= (v1.elem[i] == v2.elem[i]);
        return result;
    }

    template<typename T>
    inline bool operator != (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        return !(v1 == v2);
    }

    template<typename T>
    inline bs_matrix3X3<bool> lessThan (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<bool> vRes;
        for (size_t i = 0; i < vRes.size; ++i)
            vRes.elem[i] = (v1.elem[i] < v2.elem[i]);
        return vRes;
    }

    template<typename T>
    inline bs_matrix3X3<bool> lessThanEqual (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<bool> vRes;
        for (size_t i = 0; i < vRes.size; ++i)
            vRes.elem[i] = (v1.elem[i] <= v2.elem[i]);
        return vRes;
    }

    template<typename T>
    inline bs_matrix3X3<bool> greaterThan (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<bool> vRes;
        for (size_t i = 0; i < vRes.size; ++i)
            vRes.elem[i] = (v1.elem[i] > v2.elem[i]);
        return vRes;
    }

    template<typename T>
    inline bs_matrix3X3<bool> greaterThanEqual (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<bool> vRes;
        for (size_t i = 0; i < vRes.size; ++i)
            vRes.elem[i] = (v1.elem[i] >= v2.elem[i]);
        return vRes;
    }

    template<typename T>
    inline bs_matrix3X3<bool> equal (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<bool> vRes;
        for (size_t i = 0; i < vRes.size; ++i)
            vRes.elem[i] = (v1.elem[i] == v2.elem[i]);
        return vRes;
    }

    template<typename T>
    inline bs_matrix3X3<bool> notEqual (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<bool> vRes;
        for (size_t i = 0; i < vRes.size; ++i)
            vRes.elem[i] = (v1.elem[i] != v2.elem[i]);
        return vRes;
    }

    //------------------------------------------------------
    template<typename T> inline bs_matrix3X3<T> operator + (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v1.size; ++i)
            vRes.elem[i] = v1.elem[i] + v2.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator + (const bs_matrix3X3<T>& v, T t) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = v.elem[i] + t;
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator + (T t, const bs_matrix3X3<T>& v) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = t + v.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T>& operator += (bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        for (size_t i = 0; i < v1.size; ++i)
            v1.elem[i] += v2.elem[i];
        return v1;
    }

    template<typename T> inline bs_matrix3X3<T>& operator += (bs_matrix3X3<T>& v, T t) {
        for (size_t i = 0; i < v.size; ++i)
            v.elem[i] += t;
        return v;
    }

    //------------------------------------------------------
    template<typename T> inline bs_matrix3X3<T> operator - (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v1.size; ++i)
            vRes.elem[i] = v1.elem[i] - v2.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator - (const bs_matrix3X3<T>& v, T t) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = v.elem[i] - t;
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator - (T t, const bs_matrix3X3<T>& v) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = t - v.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T>& operator -= (bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        for (size_t i = 0; i < v1.size; ++i)
            v1.elem[i] -= v2.elem[i];
        return v1;
    }

    template<typename T> inline bs_matrix3X3<T>& operator -= (bs_matrix3X3<T>& v, T t) {
        for (size_t i = 0; i < v.size; ++i)
            v.elem[i] -= t;
        return v;
    }

    //------------------------------------------------------

    template<typename T> inline bs_matrix3X3<T> operator * (const bs_matrix3X3<T>& v, T t) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = v.elem[i] * t;
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator * (T t, const bs_matrix3X3<T>& v) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = t * v.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T>& operator *= (bs_matrix3X3<T>& v, T t) {
        for (size_t i = 0; i < v.size; ++i)
            v.elem[i] *= t;
        return v;
    }

    //------------------------------------------------------
    template<typename T> inline bs_matrix3X3<T> operator / (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v1.size; ++i)
            vRes.elem[i] = v1.elem[i] / v2.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator / (const bs_matrix3X3<T>& v, T t) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = v.elem[i] / t;
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator / (T t, const bs_matrix3X3<T>& v) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = t / v.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T>& operator /= (bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        for (size_t i = 0; i < v1.size; ++i)
            v1.elem[i] /= v2.elem[i];
        return v1;
    }

    template<typename T> inline bs_matrix3X3<T>& operator /= (bs_matrix3X3<T>& v, T t) {
        for (size_t i = 0; i < v.size; ++i)
            v.elem[i] /= t;
        return v;
    }

    //------------------------------------------------------
    template<typename T> inline bs_matrix3X3<T> operator & (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v1.size; ++i)
            vRes.elem[i] = v1.elem[i] & v2.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator & (const bs_matrix3X3<T>& v, T t) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = v.elem[i] & t;
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator & (T t, const bs_matrix3X3<T>& v) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = t & v.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T>& operator &= (bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        for (size_t i = 0; i < v1.size; ++i)
            v1.elem[i] &= v2.elem[i];
        return v1;
    }

    template<typename T> inline bs_matrix3X3<T>& operator &= (bs_matrix3X3<T>& v, T t) {
        for (size_t i = 0; i < v.size; ++i)
            v.elem[i] &= t;
        return v;
    }

    //------------------------------------------------------
    template<typename T> inline bs_matrix3X3<T> operator | (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v1.size; ++i)
            vRes.elem[i] = v1.elem[i] | v2.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator | (const bs_matrix3X3<T>& v, T t) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = v.elem[i] | t;
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator | (T t, const bs_matrix3X3<T>& v) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = t | v.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T>& operator |= (bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        for (size_t i = 0; i < v1.size; ++i)
            v1.elem[i] |= v2.elem[i];
        return v1;
    }

    template<typename T> inline bs_matrix3X3<T>& operator |= (bs_matrix3X3<T>& v, T t) {
        for (size_t i = 0; i < v.size; ++i)
            v.elem[i] |= t;
        return v;
    }

    //------------------------------------------------------
    template<typename T> inline bs_matrix3X3<T> operator ^ (const bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v1.size; ++i)
            vRes.elem[i] = v1.elem[i] ^ v2.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator ^ (const bs_matrix3X3<T>& v, T t) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = v.elem[i] ^ t;
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T> operator ^ (T t, const bs_matrix3X3<T>& v) {
        bs_matrix3X3<T> vRes;
        for (size_t i = 0; i < v.size; ++i)
            vRes.elem[i] = t ^ v.elem[i];
        return vRes;
    }

    template<typename T> inline bs_matrix3X3<T>& operator ^= (bs_matrix3X3<T>& v1, const bs_matrix3X3<T>& v2) {
        for (size_t i = 0; i < v1.size; ++i)
            v1.elem[i] ^= v2.elem[i];
        return v1;
    }

    template<typename T> inline bs_matrix3X3<T>& operator ^= (bs_matrix3X3<T>& v, T t) {
        for (size_t i = 0; i < v.size; ++i)
            v.elem[i] ^= t;
        return v;
    }


    /// ostream-operator
    template<typename T>
    std::ostream& operator << (std::ostream& s, const bs_matrix3X3<T>& m) {
        return (s
            << "| " << m.elem[0] << " " << m.elem[1] << " " << m.elem[2] << " |" << std::endl
            << "| " << m.elem[3] << " " << m.elem[4] << " " << m.elem[5] << " |" << std::endl
            << "| " << m.elem[6] << " " << m.elem[7] << " " << m.elem[8] << " |" << std::endl);
    }

template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_matrix3X3<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_matrix3X3<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_matrix3X3<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        in << v.elem[i];
    return in;
}
template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_matrix3X3<T> &v)
{
    for (int i = 0; i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
#endif // MAT3X3_H
