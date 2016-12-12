#ifndef MATHS_H
#define MATHS_H

#include "bs_define.h"
#include <math.h>
#include <float.h>
#include <ctime>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifdef PI
#undef PI
#endif

#if defined(DBL_EPSILON)
const real EPSILON = DBL_EPSILON;
#else
const real EPSILON = 2.2204460492503131e-16;
#endif
const float  FLOAT_EPS     = 1.0e-7f;
const float  FLOAT_EPS_SQ  = 1.0e-14f;
const float  FLOAT_PI      = 3.14159265358979323846f;
const real  DOUBLE_EPS    = 1.0e-14;
const real  DOUBLE_EPS_SQ = 1.0e-28;
const real  DOUBLE_PI     = 3.14159265358979323846;

const real PI = 3.14159265358979323846;
const float PIf = 3.14159265358979323846f;
#ifdef TWO_PI
#undef TWO_PI
#endif
const real TWO_PI =  6.28318530717958647693;
const real TWO_PIf =  6.28318530717958647693f;

const real SQRT_EPSILON = 1.490116119385000000e-8;
/*
    conversion from degree to radian and vice versa
*/

/// convert a radian to degree
/// 弧度转换角度
inline float bs_rad2deg(float f) {
    return (180.f / PIf) * f;
}

/// convert a radian to degree
/// 弧度转换角度
inline real bs_rad2deg(real d ) {
    return (180.0 / PI) * d;
}

/// convert a degree to radian
///  角度转换弧度
inline float bs_deg2rad(float f) {
    return (PIf / 180.f) * f;
}

/// convert a degree to radian
/// 角度转换弧度
inline real bs_deg2rad(real d ) {
    return (PI / 180.0) * d;
}

/*
    sign
*/

inline float bs_sign(float f) {
    return f == 0.f ? 0.f : f / std::fabs(f);
}

inline real bs_sign(real d) {
    return d == 0.0 ? 0.0 : d / std::fabs(d);
}

inline int bs_sign(int i) {
    return i == 0 ? 0 : i / std::fabs(i);
}

inline int bs_isign(float f) {
    return static_cast<int>(bs_sign(f));
}

inline int bs_isign(real d) {
    return static_cast<int>(bs_sign(d));
}

/*
    floor
*/

// float floor(float f) defined in std c++
// double floor(double d) defined in std c++

/// Return the largest integer not greater than \p f.
inline int bs_ifloor(float f) {
    return static_cast<int>(std::floor(f));
}

/// Return the largest integer not greater than \p d.
inline int bs_ifloor(real d) {
    return static_cast<int>(std::floor(d));
}

/*
    ceil
*/

// float ceil(float f) defined in std c++
// double ceil(double d) defined in std c++

/// Return the smallest integer not less than \p f.
inline int bs_iceil(float f) {
    return static_cast<int>(std::ceil(f));
}

/// Return the smallest integer not less than \p d.
inline int bs_iceil(real d) {
    return static_cast<int>(std::ceil(d));
}

/*
    fix
*/

/// Truncate \p f towards zero.
inline float bs_fix(float f) {
    return std::fabs(f) * bs_sign(f);
}

/// Truncate \p d towards zero.
inline double bs_fix(real d) {
    return std::fabs(d) * bs_sign(d);
}

/// Truncate \p f towards zero.
inline int bs_ifix(float f) {
    return static_cast<int>(bs_fix(f));
}

/// Truncate \p d towards zero.
inline int bs_ifix(real d) {
    return static_cast<int>(bs_fix(d));
}

/*
    round
*/

/// Return the integer nearest to \p f.
inline float bs_round(float f) {
    return std::floor(f + 0.5f);
}

/// Return the integer nearest to \p d.
inline double bs_round(real d) {
    return std::floor(d + 0.5);
}

/// Return the integer nearest to \p f.
inline int bs_iround(float f) {
    return static_cast<int>(round(f));
}

/// Return the integer nearest to \p d.
inline int bs_iround(real d) {
    return static_cast<int>(round(d));
}

/*
    clamp
*/

/// Clamps \p f to range [\p min, \p max].
inline float bs_clamp(float f, float min, float max) {
    return std::min( std::max(f,min), max );
}

/// Clamps \p f to range [\p min, \p max].
inline double bs_clamp(real f, real min, real max) {
    return std::min(std::max(f, min), max);
}

/// Clamps \p f to range [\p min, \p max].
inline int bs_clamp(int f, int min, int max) {
    return std::min(std::max(f, min), max);
}

/*
    floating point checks
 */

/// Returns false, if \p f is not a number (NaN).
inline bool bs_isNumber(float f) {
    // according to the IEEE-754 floating-point standard
    // comparisons with NaN always fail
    return f == f;
}

/// Returns false, if \p d is not a number (NaN).
inline bool bs_isNumber(real d) {
    // according to the IEEE-754 floating-point standard
    // comparisons with NaN always fail
    return d == d;
}

/// Returns true, if \p value with the type T is not a number (NaN).
template<typename T>
inline bool bs_isNaN(const T& value) {
    return (value != value);
}

/// Returns false, if \p f is NaN or infinity.
inline bool bs_isFiniteNumber(float f) {
    return (f <= FLT_MAX && f >= -FLT_MAX);
}

/// Returns false, if \p d is NaN or infinity.
inline bool bs_isFiniteNumber(real d) {
    return (d <= DBL_MAX && d >= -DBL_MAX);
}

inline float bs_fmap(float value, float inputMin, float inputMax, float outputMin,
                 float outputMax, bool clamp= false)
{
    #ifndef FLT_EPSILON
    #define FLT_EPSILON	__FLT_EPSILON__
    #endif
    if (fabs(inputMin - inputMax) < FLT_EPSILON){
        return outputMin;
    } else {
        float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);

        if( clamp ){
            if(outputMax < outputMin){
                if( outVal < outputMax )outVal = outputMax;
                else if( outVal > outputMin )outVal = outputMin;
            }else{
                if( outVal > outputMax )outVal = outputMax;
                else if( outVal < outputMin )outVal = outputMin;
            }
        }
        return outVal;
    }
}
//!
//! \brief bs_irand 取count个随机int型数组
//! \param count
//! \return
//!
template <typename T>
inline T* bs_rand(T *buf, size_t len)
{
    srand((uint)time(NULL));
    for (size_t i = 0; i < len; ++i)
    {
        if (typeid(T) == typeid(float))
            buf[i] = rand() / float(RAND_MAX);
        else if (typeid(T) == typeid(real))
            buf[i] = rand() / real(RAND_MAX);
        else
            buf[i] = rand();
    }
    return buf;
}
//!
//! \brief bs_irand 取count个以a开始的n为范围内的随机int型数组
//! \param count
//! \return
//!
template <typename T>
inline T* bs_rand(T*buf, size_t len, int a, int n)
{
    srand((uint)time(NULL));
    n = n == 0 ? 1 : n;
    for (size_t i = 0; i < len; ++i)
    {
        if (typeid(T) == typeid(float))
            buf[i] = a+ rand() %n / float(RAND_MAX);
        else if (typeid(T) == typeid(real))
            buf[i] = a+ rand() %n / real(RAND_MAX);
        else
            buf[i] = a+ rand() %n;
    }
    return buf;
}

#endif
