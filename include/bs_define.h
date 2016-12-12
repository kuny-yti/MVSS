
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang  kunyang.@gmail.com.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * You should have received a copy of the GNU General Public License.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file     bs_define.h
 * @brief    基础定义
 *       1.定义了使用的类型.
 *       2.bs_ref 引用计数器，对象间的赋值及使用的基本管理
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.0.1
 * @date     2012/01/02
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/01/02 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/01/10 | 1.0.1.0   | kunyang  | 加入引用的可共享属性
 */
#ifndef BS_DEFINE_H
#define BS_DEFINE_H

#include "bs_intrin.h"

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <cfloat>
#include <stdint.h>
#include <limits.h>
#include <typeinfo>


typedef unsigned char      uchar;

#if (bsCompiler != bsCompiler_GNUC) || !(defined __USE_MISC)
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
#endif

typedef long long          longlong;
typedef unsigned long long ulonglong;
typedef double             real;
typedef long double        lreal;

#if bsCompiler == bsCompiler_GNUC || bsCompiler == bsCompiler_CLANG
#   if bsCompilerVersion <  270
    typedef char            int8;
    typedef unsigned char   uint8;
    typedef short           int16;
    typedef unsigned short  uint16;
    typedef int             int32;
    typedef unsigned int    uint32;
#     if bsArchitecture == bsArchitecture_64Bit
        typedef long int int64;
        typedef unsigned long int uint64;
#     else
        typedef long long int int64
        typedef unsigned long long int uint64;
# endif

#   else
//!
//! QI: An integer that is as wide as the smallest addressable unit, usually 8 bits.
//! HI: An integer, twice as wide as a QI mode integer, usually 16 bits.
//! SI: An integer, four times as wide as a QI mode integer, usually 32 bits.
//! DI: An integer, eight times as wide as a QI mode integer, usually 64 bits.
//! SF: A floating point value, as wide as a SI mode integer, usually 32 bits.
//! DF: A floating point value, as wide as a DI mode integer, usually 64 bits.
//!
typedef int int8  __attribute__ ((mode (QI)));
typedef int int16 __attribute__ ((mode (HI)));
typedef int int32 __attribute__ ((mode (SI)));
typedef int int64 __attribute__ ((mode (DI)));

typedef unsigned int uint8  __attribute__ ((mode (QI)));
typedef unsigned int uint16 __attribute__ ((mode (HI)));
typedef unsigned int uint32 __attribute__ ((mode (SI)));
typedef unsigned int uint64 __attribute__ ((mode (DI)));

#   endif
#elif bsCompiler == bsCompiler_MSVC
    typedef char    int8;
    typedef uchar   uint8;
    typedef short   int16;
    typedef ushort  uint16;
    typedef int     int32;
    typedef uint    uint32;
    typedef unsigned __int64 uint64;
    typedef __int64          int64;
#endif

#if bsPlatform == bsPlatform_Win32
typedef intptr_t intptr;
typedef uintptr_t uintptr;
#else
#   if bsArchitecture == bsArchitecture_64Bit
    typedef long intptr;
    typedef ulong uintptr;
#   else
    typedef int intptr;
    typedef uint uintptr;
#   endif
#endif

typedef intptr oid_t;
typedef int idx_t;

/* Limits of exact-width integer types */
#ifndef INT8_MIN
#define INT8_MIN        (-127 - 1)
#endif
#ifndef INT16_MIN
#define INT16_MIN       (-32767 - 1)
#endif
#ifndef INT32_MIN
#define INT32_MIN       (-2147483647 - 1)
#endif
#ifndef INT64_MIN
#define INT64_MIN       (-9223372036854775807i64 - 1)
#endif

#ifndef INT8_MAX
#define INT8_MAX        127
#endif
#ifndef INT16_MAX
#define INT16_MAX       32767
#endif
#ifndef INT32_MAX
#define INT32_MAX       2147483647
#endif
#ifndef INT64_MAX
#define INT64_MAX       9223372036854775807LL
#endif

#ifndef UINT8_MAX
#define UINT8_MAX       0xFF
#endif
#ifndef UINT16_MAX
#define UINT16_MAX      0xFFFF
#endif
#ifndef UINT32_MAX
#define UINT32_MAX      0xFFFFFFFFU
#endif
#ifndef UINT64_MAX
#define UINT64_MAX      0xFFFFFFFFFFFFFFFFULL
#endif

#ifndef INTPTR_SIZE
#define INTPTR_SIZE sizeof(intptr)
#endif
#ifndef UINTPTR_SIZE
#define UINTPTR_SIZE sizeof(uintptr)
#endif

#ifndef bsVersionNumber
#define bsVersionNumber(major, minor, path)\
    ((major << 16 & 0xffff) | (minor << 8 & 0xff) | path & 0xff)
#endif

#ifndef bsVersionStr
#define bsVersionStr(major, minor, path) \
    (#major"."#minor"."#path)
#endif


#ifndef bsFloatBit
#define bsFloatBit 4
#endif

#ifndef bsFloatAcc
#define bsFloatAcc 0.0001
#endif

#ifndef bsEqualf
#define bsEqualf(v1 ,v2) (((v1 + bsFloatAcc) > v2) && ((v1 - bsFloatAcc) < v2))
#endif
#ifndef bsUnEqualf
#define bsUnEqualf(v1 ,v2) (!bsEqualf(v1 ,v2))
#endif

#ifndef bsEmptyf
#define bsEmptyf(v1 ,v2) ((v1 < bsFloatAcc) && (v1 > -bsFloatAcc))
#endif

#ifndef bsHasUTF8
#define bsHasUTF8 1
#endif

#ifndef bsHasUTF16
#define bsHasUTF16 1
#endif

#ifndef bsHasUTF32
#define bsHasUTF32 (sizeof(wchar_t) != sizeof(unsigned short))
#endif

#ifndef bsWindowId
typedef void *winid;
#define bsWindowId winid
#endif
#ifndef bsContextId
typedef void *ctxid;
#define bsContextId ctxid
#endif
#ifndef bsConfigId
typedef void *cfgid;
#define bsConfigId cfgid
#endif

#ifndef bsPackage
#define bsPackage struct
#endif
#ifdef bsPackage
#define bsAsCast(type) dynamic_cast<type>
#else
#define bsAsCast(type) (type)
#endif

/*
 * 1.变量修饰: int a __attribute__((aligned(8))) = 0;
 * 2.类型修饰: struct test {int a;} __attribute__((aligned(8)));
 * 3.函数修饰: void test(void) __attribute__((aligned(2)));
 */
#define bsAligned(byte) __attribute__ ((aligned(byte))) //修饰变量，类型，函数

/*
 * static inline void test2(void) __attribute__((always_inline));
 */
#define bsAlwaysInline __attribute__((always_inline)) //将函数强制为内联函数
#define bsPacked __attribute__ ((packed)) //修饰结构体，枚举，联合体的字节数为紧凑结构
#define bsNoReture __attribute__((noreturn))


#define bsConstructor(level) __attribute__((constructor(100+level))) //在main之前执行此函数
#define bsDestructor(level) __attribute__((destructor(100+level))) //在main退出后执行此函数

#ifdef bsLanguage
#define _in_ const
#define _out_ &
#define _inout_ &
#define _fix_ const
#define _ref_ &
#else
#define _in_ const
#define _out_ *
#define _inout_ *
#define _fix_ const
#define _ref_ *
#endif

//! 开启智能指针
#define bsAutoPtr_Enable
//! 开启引用计数对象
#define bsReferCount_Enable
//! 开启调试和日志输出
#define bsDebug_Enable
//! 开启断言
#define bsAssert_Enable


/// 执行特定次数
#define repeat(num) for (size_t __i___ = 0; __i___ < (size_t)num; ++__i___)

/// 执行条件型 (加入条件为了防止死循环)
#define forever(cond) for (;cond;)

/// 容器操作型 (c++98 不能操作数组类型)
#if bsLanguage >= bsLanguage11
#define foreach(var, container) for (var : container)
#else

struct bsContainerBase{
    virtual bool condition()_fix_ = 0;
    virtual void operator ++()_fix_ = 0;
    virtual void operator --()_fix_ = 0;
    virtual void operator ++(int)_fix_ = 0;
    virtual void operator --(int)_fix_ = 0;

    virtual bool cond_less()_fix_ =0;
    virtual bool cond_add()_fix_ =0;
};
template <typename T>
class bsContainerOpe :public bsContainerBase
{
    bsContainerOpe & operator=(_in_ bsContainerOpe &) = delete;
public:
    inline bsContainerOpe(_in_ T & t):
        cond(0),
        container(t),
        b(t.begin()),
        e(t.end()){}
    bsContainerOpe(_in_ bsContainerOpe & rhs):
        cond(rhs.cond),
        container(rhs.container),
        b(rhs.b),
        e(rhs.e) {}

    bool condition()const {return b != e && !cond_add();}
    void operator ++()const {++b;}
    void operator --()const  {--b;}
    void operator ++(int)const {++b;}
    void operator --(int)const {--b;}

    bool cond_less()const {bsContainerOpe* co = (bsContainerOpe*)this; return co->cond > 0 ? co->cond--: false;}
    bool cond_add()const {bsContainerOpe* co = (bsContainerOpe*)this; return co->cond++;}

    int cond;
    const T container;
    mutable typename T::iterator b, e;
};
template <typename T>
T *bsContainerAffirm(_inf_ T &) {return NULL;}
template <typename T>
bsContainerOpe<T> bsContainerNew(_in_ T & conta) {return bsContainerOpe<T>(conta);}
template <typename T>
const bsContainerOpe<T> *bsContainerAs(_in_ bsContainerBase * fc, _in_ T *) {return (_fix_ bsContainerOpe<T>*)fc;}
#define foreach(val, container) \
    for (const bsContainerBase &_conta_ = bsContainerNew(container); _conta_.condition(); ++_conta_) \
    for (val = *bsContainerAs(&_conta_, bsContainerAffirm(container))->b; _conta_.cond_less() ; )

#endif


#ifndef bsReferCount_Enable
struct bs_ref{};
#else
#include "bs_atomic.h"
/*!
 * \brief The bs_ref struct
 */
struct bs_ref
{
public:
    /*!
     * \brief 引用标志定义
     */
    enum eRefFlags
    {
        refStatic = -1,                                ///< 静态模式，不支持引用
        refShareable = 0,                              ///< 可共享模式，用于对象间引用
        refOne = 1,                                    ///< 将当前引用置为1，
        refDetach = 0x40000000,                        ///< 支持分离
        refMask = 0xbfffffff,                          ///< 标志提取掩码
        refShareableDetach = refShareable | refDetach, ///< 可共享切可分离
        refOneDetach = refOne | refDetach,             ///< 可共享、可分离并且将引用置为1
    };
public:
    bs_ref(eRefFlags flag = refShareable):ref((int)flag){}
    ~bs_ref(){}

    bs_ref &operator = (const bs_ref&) = default;

    //!
    //! \brief count 返回引用的次数
    //! \return
    //!
    int count()const {return (ref & int(refMask));}
    operator int()const{return (ref & int(refMask));}
    operator int(){return (ref & int(refMask));}
    //!
    //! \brief set 设置引用标志
    //! \param flag
    //!
    void set(eRefFlags flag = refStatic){ref = (int)flag;}
    bs_ref & operator = (eRefFlags flags){ref = (int)flags;return *this;}
    //!
    //! \brief is_shared 返回是否有引用共享
    //! \return
    //!
    bool is_shared()const{return (ref & int(refMask)) > (int)refShareable;}
    //!
    //! \brief is_static 返回是否为静态引用
    //! \return
    //!
    bool is_static()const{return ref == (int)refStatic;}
    //!
    //! \brief has_shareable 返回是否启用引用共享
    //! \return
    //!
    bool has_shareable()const{return (ref & int(refMask)) >= (int)refShareable;}

    //!
    //! \brief has_detach 返回是否支持分离
    //! \return
    //!
    bool has_detach()const{return ref & (int)refDetach;}

    //!
    //! \brief addref 添加引用计数
    //!
    void addref() {++ref;}
    //!
    //! \brief lessref 释放引用计数
    //! \return
    //!
    bool lessref()
    {
        if (!(ref & int(refMask)) || (--ref) < 0)
            return true;
        return false;
    }
    //!
    //! \brief is_destroy 返回是否需要释放对象
    //! \return
    //!
    bool is_destroy()
    {
        if (!(ref & int(refMask)) || ref < 0)
            return true;
        return false;
    }
    bs_ref &operator ++(){++ref;return *this;}
    bs_ref operator ++(int){bs_ref tret = *this; ref++;return tret;}
    bs_ref &operator --(){--ref; return *this;}
    bs_ref operator --(int){bs_ref tret = *this;ref--; return tret;}

private:
    bs_atomic<int> ref;
};
#endif

#endif // DEFINE_H
