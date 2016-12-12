#ifndef bs_quaternion_H
#define bs_quaternion_H

#include "bs_define.h"
#include "bs_vec4.h"
#include "bs_mat2x2.h"
#include "bs_mat3x3.h"
#include "bs_mat4X4.h"

template<typename T>
struct bs_quaternion {
    typedef T ElemType;
    enum {size = 4};
    union {
        struct {
            T x;
            T y;
            T z;
            T w;
        };

        T elem[size];
    };

    /// Constructor, the standard is the identity-bs_quaternion.
    bs_quaternion (T X = 0, T Y = 0, T Z = 0, T W = 1)
        : x(X), y(Y), z(Z), w(W) {}

    /// Constructor expecting a 3d-vector and a scalar value
    bs_quaternion (bs_vector3<T> V, T W)
        : x(V.x), y(V.y), z(V.z), w(W) {}

    /// Init from array with size 4
    bs_quaternion (const T* t) {
        for (size_t i = 0; i < 4; ++i)
            elem[i] = t[i];
    }

    /// Init from matrix
    bs_quaternion (const bs_matrix4X4<T>& m) {
        *this = generateQuatFromMatrix(m);
    }

    /// calculate the absolute length of the bs_quaternion
    T abs() const {
        return std::sqrt(x * x + y * y +
                         z * z + w * w);
    }

    /// return the vector consisting of the imaginary parts of the bs_quaternion
    bs_vector3<T> vec() const {
        return bs_vector3<T>(x, y, z);
    }

    /// normalizes this bs_quaternion
    void normalize() {
        T ab = abs();

        if (ab != 0) {
            x /= ab;
            y /= ab;
            z /= ab;
            w /= ab;
        }
    }

    /// conjugates this bs_quaternion
    void conjugate() {
        x = -x;
        y = -y;
        z = -z;
    }

    /// inverts this bs_quaternion
    void invert() {
        T absq = x*x + y*y + z*z + w*w;
        if (absq != 0) {
            conjugate();
            *this = *this / absq;
        }
    }

    /// Calculates the bs_quaternion dot-product of this bs_quaternion
    /// with another bs_quaternion.  The *-operator is overloaded for this purpose.
    bs_quaternion operator * (const bs_quaternion& q) const {
        bs_quaternion res;

        res.x = w*q.x + x*q.w + y*q.z - z*q.y;
        res.y = w*q.y - x*q.z + y*q.w + z*q.x;
        res.z = w*q.z + x*q.y - y*q.x + z*q.w;
        res.w = w*q.w - x*q.x - y*q.y - z*q.z;

        return res;
    }

    /// Calculates the bs_quaternion dot-product of this bs_quaternion
    /// with another bs_quaternion.  The *-operator is overloaded for this purpose.
    bs_quaternion operator *= (const bs_quaternion& q) {
        *this = *this * q;
        return *this;
    }

    /// bs_quaternion addition
    bs_quaternion operator + (const bs_quaternion& q) const {
        bs_quaternion res;

        res.x = x + q.x;
        res.y = y + q.y;
        res.z = z + q.z;
        res.w = w + q.w;

        return res;
    }

    /// bs_quaternion subtraction
    bs_quaternion operator - (const bs_quaternion& q) const {
        bs_quaternion res;

        res.x = x - q.x;
        res.y = y - q.y;
        res.z = z - q.z;
        res.w = w - q.w;

        return res;
    }

    /// divide bs_quaternion by a float
    bs_quaternion operator / (const T f) const {
        bs_quaternion res;

        res.x = x / f;
        res.y = y / f;
        res.z = z / f;
        res.w = w / f;

        return res;
    }

    /// multiply bs_quaternion with a float
    bs_quaternion operator * (const T f) const {
        bs_quaternion res;

        res.x = x * f;
        res.y = y * f;
        res.z = z * f;
        res.w = w * f;

        return res;
    }

    /// multiply bs_quaternion with a vector
    bs_vector3<T> operator * (const bs_vector3<T> v) const {
        bs_vector3<T> tmp1, tmp2;
        bs_vector3<T> qvector = vec();

        tmp1 = cross(qvector, v);
        tmp2 = cross(qvector, tmp1);
        tmp1 = (T)(2 * w) * tmp1;
        tmp2 = (T)2 * tmp2;

        return v + tmp1 + tmp2;
    }

    /// equality operator
    bool operator == (const bs_quaternion& q) {
        return(x == q.x && y == q.y && z == q.z && w == q.w);
    }

    /// calculate the t-th power of this bs_quaternion
    bs_quaternion pow(const T t) {
        using namespace std; // use overloaded cos and sin
        if (abs() != 1 || t < 0 || t > 0) return 0;

        double phi = acos(w);
        bs_vector3<T> u = vec() / sin(phi);

        double factor = sin(phi*t);
        bs_quaternion res = bs_quaternion(factor*u.x, factor*u.y, factor*u.z, cos(phi*t));
        return res;
    }

    /// calculate the natural logarithm of this bs_quaternion
    bs_vector3<T> log() const {
        using namespace std; // use overloaded cos and sin
        if (abs() != 1) return 0;

        double phi = acos(w);
        bs_vector3<T> u = vec() / sin(phi);

        return u*phi;
    }

    /// create a bs_quaternion from a a given angle and axis representing that rotation
    static bs_quaternion createQuat(T angle, const bs_vector3<T>& axis) {
        bs_quaternion res;

        bs_vector3<T> newAxis = ::normalize(axis);

        res.x = newAxis.x * sinf(angle / 2);
        res.y = newAxis.y * sinf(angle / 2);
        res.z = newAxis.z * sinf(angle / 2);
        res.w = std::cos(angle / 2);

        return res;
    }

    /// rotate a vector about an axis using quaterions
    static bs_vector3<T> rotate(const bs_vector3<T>& target, T angle, const bs_vector3<T>& axis) {
        return rotate(target, createQuat(angle, axis));
    }

    /// rotate a vector about an axis using quaterions
    static bs_vector3<T> rotate(const bs_vector3<T>& target, bs_quaternion rot) {

        bs_quaternion rotconj = rot;
        rotconj.conjugate();

        // create a target bs_quaternion so we can use bs_quaternion-multiplication
        bs_quaternion targ(target.x, target.y, target.z, 0.f);

        // We calculate r * t * r^-1.  This yields our rotated vector.  Technically,
        // we would have to invert rot, but since it is a unit bs_quaternion the inverted
        // bs_quaternion is equal to the conjugated one, so we use conjugation which is cheaper.
        // Also note that the returned vector is automatically of length 1 since we only use
        // use unit-bs_quaternions.
        bs_quaternion res = (rot * targ) * rotconj;

        return vec3(res.x, res.y, res.z);
    }
};

/**
        Multiply a float with a bs_quaternion
    */
template<typename T>
inline bs_quaternion<T> operator * (T f, const bs_quaternion<T>& q) {
    bs_quaternion<T> res;

    res.x = q.x * f;
    res.y = q.y * f;
    res.z = q.z * f;
    res.w = q.w * f;

    return res;
}

/**
        Calculates the bs_quaternion dot-product of two bs_quaternions.
    */
template<typename T>
inline bs_quaternion<T> multQuat(const bs_quaternion<T>& q1, const bs_quaternion<T>& q2) {
    bs_quaternion<T> res;

    res.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
    res.y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
    res.z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;
    res.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;

    return res;
}

/**
        e^bs_quaternion given as:
        Qexp(v*a) = [cos(a),vsin(a)]
    */
template<typename T>
bs_quaternion<T> expQuat(const bs_quaternion<T>& q) {
    float a = std::sqrt(q.x*q.x + q.y*q.y + q.y*q.y);
    float sina = sinf(a);
    bs_quaternion<T> ret;

    ret.w = cosf(a);

    if (a > 0) {
        ret.x = sina * q.x / a;
        ret.y = sina * q.y / a;
        ret.z = sina * q.z / a;
    }
    else {
        ret.x = q.x;
        ret.y = q.y;
        ret.z = q.z; //= ret.y = ret.z = 0;
    }

    return ret;
}

/**
        Returns the logarithm of a bs_quaternion.
    */
template<typename T>
bs_quaternion<T> logQuat(const bs_quaternion<T>& q) {
    float a = acosf(q.w);
    float sina = sinf(a);
    bs_quaternion<T> ret;

    ret.w = 0;

    if (sina > 0) {
        ret.x = a*q.x/sina;
        ret.y = a*q.y/sina;
        ret.z = a*q.z/sina;
    }
    else {
        ret.x=ret.y=ret.z=0;
    }

    return ret;
}

/**
         Returns a bs_quaternion in normalized form.
    */
template<typename T>
inline bs_quaternion<T> normalize(const bs_quaternion<T>& q) {
    T ab = q.abs();
    bs_quaternion<T> res = q;

    if (ab != 0) {
        res.x /= ab;
        res.y /= ab;
        res.z /= ab;
        res.w /= ab;
    }

    return res;
}

/**
        Returns a bs_quaternion in conjugated form.
    */
template<typename T>
inline bs_quaternion<T> conjugate(const bs_quaternion<T>& q) {
    bs_quaternion<T> res = q;

    res.x *= -1;
    res.y *= -1;
    res.z *= -1;

    return res;
}

/**
        Return the inverted form of a bs_quaternion.
    */
template<typename T>
inline bs_quaternion<T> invert(const bs_quaternion<T>& q) {
    T absq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;
    if (absq != 0) {
        bs_quaternion<T> res = conjugate(q);
        return res / absq;
    }

    return q;
}

/**
        Lerp is the simplest form of bs_quaternion interpolation; the two given bs_quaternions are simply linearly
        interpolated.  Lerp may not yield as nice results as slerp, but it is faster and in certain situations
        sufficient.
    */
template<typename T>
inline bs_quaternion<T> lerpQuat(const bs_quaternion<T>& a, const bs_quaternion<T>& b, T t) {
    ASSERT(((t < 0.0) || (t > 1.0)), "running-parameter must be between 0 and 1");

    return normalize((b - a)*t + a);
}

/**
        Slerp (Spherical Linear Interpolation) returns a interpolated bs_quaternion between the two given
        bs_quaternions, depending on the interpolation variable t.  This is based on on the chapter concerning
        bs_quaternions in Real Time Rendering and subsequently on Ken Shoemakes paper "Animating rotation with
        bs_quaternion curves".
    */
template<typename T>
bs_quaternion<T> slerpQuat(const bs_quaternion<T>& a, const bs_quaternion<T>& b, T t, bool inv = true) {
    using namespace std; // use overloaded cos and sin
    ASSERT(((t < T(0.0)) || (t > T(1.0))), "running-parameter must be between 0 and 1");

    bs_quaternion<T> c = b;
    T tmp = a.w*b.w + a.x*b.x + a.y*b.y + a.z*b.z;

    if (inv && tmp < T(0.0)) {
        tmp = -tmp;
        c = -1.f*c;
    }

    if (tmp < T(1.0) && tmp > T(-1.0)) {
        T ang = acos(tmp);
        return normalize(((a * sin(ang * (T(1.0) - t))) + (c * sin(ang * t))) / sin(ang));
    }
    else {
        return lerpQuat(a, c, t);
    }
}

/**
        This function returns an interpolated bs_quaternion between p and q, using a and b as "Control-
        bs_quaternions" and t as the interpolation variable.  This is based on the chapter concerning
        bs_quaternions in Real Time Rendering.
    */
template<typename T>
inline bs_quaternion<T> squadQuat(const bs_quaternion<T>& p, const bs_quaternion<T>& q,
                                  const bs_quaternion<T>& a, const bs_quaternion<T>& b, double t)
{
    bsASSERT(((t < 0.0) || (t > 1.0)), "running-parameter must be between 0 and 1");

    return slerpQuat<T>(slerpQuat<T>(p, q, static_cast<T>(t)), slerpQuat<T>(a, b, static_cast<T>(t)), static_cast<T>(2.0*t*(1.0 - t)), false);
}

/**
        Generate "Control-bs_quaternions" that can be used for the squad-Algorithm to interpolate between
        given bs_quaternions.  This is based on the chapter concerning bs_quaternions in Real Time Rendering.
    */
template<typename T>
inline bs_quaternion<T> splineQuat(const bs_quaternion<T>& pa, const bs_quaternion<T>& p, const bs_quaternion<T>& pb) {
    bs_quaternion<T> pinvert = conjugate(p);

    return p * expQuat((logQuat(pa*pinvert) + logQuat(pb*pinvert)) * static_cast<T>(-0.25));
}

/**
        Generate a bs_quaternion that represents the rotation from the source to the destination
        vector.
    */
template<typename T>
bs_quaternion<T> generatebs_quaternionFromTo(const bs_vector3<T> src, const bs_vector3<T> dest) {
    // This is taken from Game Programming Gems 1 and Real Time Rendering

    bs_quaternion<T> q;

    bs_vector3<T> v1 = normalize(src);
    bs_vector3<T> v2 = normalize(dest);

    bs_vector3<T> crs = cross(v1, v2);

    T dot = v1.x*v2.x + v1.y+v2.y + v1.z*v2.z;

    if (std::fabs(dot) >= 1) // the vectors are identical
        return bs_quaternion<T>(0, 0, 0, 1); // ... so we return a rotation that does nothing

    T root = std::sqrt((1 + dot) * 2);

    if (root < T(1e-6)) { // do this for numerical stability
        bs_vector3<T> axis = cross(bs_vector3<T>(1, 0, 0), src);
        if (length(axis) == 0) {
            axis = cross(bs_vector3<T>(0, 1, 0), src);
        }
        axis = normalize(axis);
        q = bs_quaternion<T>::createQuat(PIf, axis);
    }
    else {
        T inverse = 1 / root;
        q.x = crs.x * inverse;
        q.y = crs.y * inverse;
        q.z = crs.z * inverse;
        q.w = T(0.5) * inverse;
    }

    return q;
}

/**
        Generates a rotational matrix from a unit bs_quaternion
    */
template<typename T>
bs_matrix4X4<T> generateMatrixFromQuat(const bs_quaternion<T>& q) {
    T xx      = q.x * q.x;
    T xy      = q.x * q.y;
    T xz      = q.x * q.z;
    T xw      = q.x * q.w;
    T yy      = q.y * q.y;
    T yz      = q.y * q.z;
    T yw      = q.y * q.w;
    T zz      = q.z * q.z;
    T zw      = q.z * q.w;

    bs_matrix4X4<T> mat;

    mat.elem[0]  = 1 - 2 * ( yy + zz );
    mat.elem[1]  =     2 * ( xy - zw );
    mat.elem[2]  =     2 * ( xz + yw );
    mat.elem[4]  =     2 * ( xy + zw );
    mat.elem[5]  = 1 - 2 * ( xx + zz );
    mat.elem[6]  =     2 * ( yz - xw );
    mat.elem[8]  =     2 * ( xz - yw );
    mat.elem[9]  =     2 * ( yz + xw );
    mat.elem[10] = 1 - 2 * ( xx + yy );
    mat.elem[3]  = mat.elem[7] = mat.elem[11] = mat.elem[12] = mat.elem[13] = mat.elem[14] = 0;
    mat.elem[15] = 1;

    return transpose(mat);
}
/**
        Generates a unit bs_quaternion from a rotational matrix
    */
template<typename T>
bs_quaternion<T> generateQuatFromMatrix(const bs_matrix4X4<T>& mat) {
    bs_quaternion<T> q;
    T t = mat[0][0] + mat[1][1] + mat[2][2] + T(1);

    //     if (t > 0) {
    //         double s = 0.5 / std::sqrt(t);
    //         q.w = 0.25 / s;
    //         q.x = ( mat[2][1] - mat[1][2] ) * s;
    //         q.y = ( mat[0][2] - mat[2][0] ) * s;
    //         q.z = ( mat[1][0] - mat[0][1] ) * s;
    //     } else if ((mat[0][0] > mat[1][1]) && (mat[0][0] > mat[2][2])) {
    //         double s = std::sqrt( 1.0 + mat[0][0] - mat[1][1] - mat[2][2] ) * 2.0;
    //         q.x = 0.25 / s;
    //         q.y = (mat[0][1] + mat[1][0] ) / s;
    //         q.z = (mat[0][2] + mat[2][0] ) / s;
    //         q.w = (mat[1][2] - mat[2][1] ) / s;
    //     } else if (mat[1][1] > mat[2][2]) {
    //         double s = std::sqrt( 1.0 + mat[1][1] - mat[0][0] - mat[2][2] ) * 2.0;
    //         q.x = (mat[0][1] + mat[1][0] ) / s;
    //         q.y = 0.25 / s;
    //         q.z = (mat[1][2] + mat[2][1] ) / s;
    //         q.w = (mat[0][2] - mat[2][0] ) / s;
    //     } else {
    //         double s = std::sqrt( 1.0 + mat[2][2] - mat[0][0] - mat[1][1] ) * 2.0;
    //         q.x = (mat[0][2] + mat[2][0] ) / s;
    //         q.y = (mat[1][2] + mat[2][1] ) / s;
    //         q.z = 0.25 / s;
    //         q.w = (mat[0][1] - mat[1][0] ) / s;
    //     }
    if (t > T(0)) {
        T s = T(0.5) / std::sqrt(t);
        q.w = T(0.25) / s;
        q.x = ( mat[1][2] - mat[2][1] ) * s;
        q.y = ( mat[2][0] - mat[0][2] ) * s;
        q.z = ( mat[0][1] - mat[1][0] ) * s;
    } else if ((mat[0][0] > mat[1][1]) && (mat[0][0] > mat[2][2])) {
        T s = std::sqrt( T(1.0) + mat[0][0] - mat[1][1] - mat[2][2] ) * T(2.0);
        q.x = T(0.25) / s;
        q.y = (mat[1][0] + mat[0][1] ) / s;
        q.z = (mat[2][0] + mat[0][2] ) / s;
        q.w = (mat[2][1] - mat[1][2] ) / s;
    } else if (mat[1][1] > mat[2][2]) {
        T s = std::sqrt( T(1.0) + mat[1][1] - mat[0][0] - mat[2][2] ) * T(2.0);
        q.x = (mat[1][0] + mat[0][1] ) / s;
        q.y = T(0.25) / s;
        q.z = (mat[2][1] + mat[1][2] ) / s;
        q.w = (mat[2][0] - mat[0][2] ) / s;
    } else {
        T s = std::sqrt( T(1.0) + mat[2][2] - mat[0][0] - mat[1][1] ) * T(2.0);
        q.x = (mat[2][0] + mat[0][2] ) / s;
        q.y = (mat[2][1] + mat[1][2] ) / s;
        q.z = T(0.25) / s;
        q.w = (mat[1][0] - mat[0][1] ) / s;
    }

    return q;
}

template<typename T>
void generateAxisAngleFromQuat(const bs_quaternion<T>& qu, T& angle, bs_vector3<T>& axis) {

    bs_quaternion<T> q = qu;

    if (q.w > 1)
        q.normalize();

    angle = 2 * std::acos(q.w);
    T s = std::sqrt(1 - q.w * q.w);

    if (s < 0.001) {
        axis.x = q.x;
        axis.y = q.y;
        axis.z = q.z;
    } else {
        axis.x = q.x / s; // normalise axis
        axis.y = q.y / s;
        axis.z = q.z / s;
    }
}

/**
        ostream-operator for output
    */
template<typename T>
std::ostream& operator << (std::ostream& s, const bs_quaternion<T>& q) {
    return (s << "{ " << q.x << " " << q.y << " " << q.z << " " << q.w << " }" << std::endl);
}

// just a typedef for easier use, since most of the time float-bs_quaternions are used
typedef bs_quaternion<float>   quat;
typedef bs_quaternion<float>   fquat;
typedef bs_quaternion<real>  dquat;


template<typename T>
bs_bstream &operator << (bs_bstream &in, const bs_quaternion<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_bstream &operator >> (bs_bstream &out, bs_quaternion<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
template<typename T>
bs_tstream &operator << (bs_tstream &in, const bs_quaternion<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        in << v.elem[i];
    return in;
}

template<typename T>
bs_tstream &operator >> (bs_tstream &out, bs_quaternion<T> &v)
{
    for (int i = 0;i < v.size; ++i)
        out >> v.elem[i];
    return out;
}
#endif // bs_quaternion_H
