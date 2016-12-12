#ifndef __BS_MAIN__
#define __BS_MAIN__

// desktop platform
#define bsPlatform_Win32   0x00 // Microsoft windows 32 bit
#define bsPlatform_Unix    0x01  // unix
#define bsPlatform_Linux   0x02  // linux 32/64 bit
#define bsPlatform_Apple   0x03 // apple 32/64 bit

// phone platform
#define bsPlatform_Phone   0x10 // Microsoft wine,phone
#define bsPlatform_Ios     0x11 // ios
#define bsPlatform_Android 0x12 // android

// arch
#define bsArchitecture_32Bit 32  // 32 bit system
#define bsArchitecture_64Bit 64  // 64 bit system

// compiler
#define bsCompiler_MSVC   0x20  // Microsoft Visual C/C++
#define bsCompiler_GNUC   0x21  // GNU gcc/g++ (Cygwin,MinGW)
#define bsCompiler_WINSCW 0x22  // Microsoft Visual C/C++
#define bsCompiler_GCCE   0x23  // GNU Compiler Collection for Embedded
#define bsCompiler_CLANG  0x24  // Apple clang

// language
#define bsLanguageC89 198901L // c89
#define bsLanguageC95 199409L // c95
#define bsLanguageC99 199901L // c99
#define bsLanguageC11 201112L // c11
#define bsLanguage98 200303L// c++98/c++03
#define bsLanguage11 201103L// c++11
#define bsLanguage14 201402L// c++14


//! 系统平台
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define bsPlatform bsPlatform_Win32
#elif defined( __APPLE_CC__)
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || \
       __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define bsPlatform bsPlatform_Ios
#   else
#       define bsPlatform bsPlatform_Apple
#   endif
#elif defined(__ANDROID__) || defined(ANDROID)
#	define bsPlatform bsPlatform_Android
#elif defined(__linux__) || defined(__linux)
#   define bsPlatform bsPlatform_Linux
#else
#   define bsPlatform bsPlatform_Unix
#endif

//! 体系建构
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || \
    defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define bsArchitecture bsArchitecture_64Bit
#else
#   define bsArchitecture bsArchitecture_32Bit
#endif

//! 编译器
#if defined( __GCCE__ )
#   define bsCompiler bsCompiler_GCCE
#   define bsCompilerVersion _MSC_VER
#elif defined( __WINSCW__ )
#   define bsCompiler bsCompiler_WINSCW
#   define bsCompilerVersion _MSC_VER
#elif defined( _MSC_VER )
#   define bsCompiler bsCompiler_MSVC
#   define bsCompilerVersion _MSC_VER
#elif defined( __clang__ )
#   define bsCompiler bsCompiler_CLANG
#   define bsCompilerVersion (((__clang_major__)*100) +(__clang_minor__*10) +__clang_patchlevel__)
#elif defined( __GNUC__ )
#   define bsCompiler bsCompiler_GNUC
#   define bsCompilerVersion (((__GNUC__)*100) +(__GNUC_MINOR__*10) +__GNUC_PATCHLEVEL__)
#else
#   pragma error "No compiler. Abort! "
#endif

// Windows Settings
#if bsPlatform == bsPlatform_Win32
#	if defined( bsStaticLIB )
#   	define bsExport
#   elif defined( bsExportDll )
#       if bsCompiler == bsCompiler_GNUC
#           if bsCompilerVersion >= (4*100)
#               define bsExport __attribute__ ((visibility("default")))
#           else
#               define bsExport __attribute__((dllexport))
#           endif
#       elif bsPlatform == bsCompiler_MSVC
#           define bsExport __declspec( dllexport )
#       endif
#   else
#       if bsCompiler == bsCompiler_GNUC
#           if bsCompilerVersion >= (4*100)
#               define bsExport
#           else
#               define bsExport __attribute__((dllimport))
#           endif
#       else
#           if bsPlatform == bsCompiler_MSVC
#               define bsExport __declspec( dllimport )
#           endif
#       endif
#	endif

#elif bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux || \
    bsPlatform == bsPlatform_Apple || bsPlatform == bsPlatform_Ios ||  \
    bsPlatform == bsPlatform_Android
#   if bsCompiler == bsCompiler_GNUC
#       if bsCompilerVersion >= (4*100)
#           define bsExport  __attribute__ ((visibility("default")))
#       else
#           define bsExport
#       endif
#   elif bsCompiler == bsCompiler_CLANG
#       define bsExport  __attribute__ ((visibility("default")))
#   endif

#endif

#ifndef bsExternC
#define bsExternC extern "C"
#endif

#ifndef bsNew
#define bsNew(t) new t
#endif

#ifndef bsDelete
#define bsDelete(oj) delete oj
#endif

#ifndef bsMalloc
#define bsMalloc(size) malloc(size)
#endif

#ifndef bsRealloc
#define bsRealloc(ptr, size) realloc(ptr, size)
#endif

#ifndef bsFree
#define bsFree(oj) free(oj)
#endif

// 定义平台的主入口函数
#if bsPlatform == bsPlatform_Win32

#include <winsock2.h>
#include <windows.h>

#define bs_main \
     bs_main(int argc, char **argv);\
static inline char *wideToMulti(int codePage, const wchar_t *aw) \
{\
    const int required = WideCharToMultiByte(codePage, 0, aw, -1, NULL, 0, NULL, NULL);\
    char *result = new char[required];\
    WideCharToMultiByte(codePage, 0, aw, -1, result, required, NULL, NULL);\
    return result;\
}\
extern "C" int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR , int )\
{\
    int argc;\
    wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argc);\
    if (!argvW)\
        return -1;\
    char **argv = bsNew (char *[argc]);\
    for (int i = 0; i < argc; ++i)\
        argv[i] = wideToMulti(CP_ACP, argvW[i]);\
    LocalFree(argvW);\
    const int exitCode = bs_main(argc, argv);\
    for (int i = 0; i < argc; ++i)\
        bsDelete ([] argv[i]);\
    bsDelete ([] argv);\
    return exitCode;\
} \
    int bs_main

#elif bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux

#include <features.h>
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <net/if.h>

#define bs_main  main

#elif bsPlatform == bsPlatform_Apple

#endif

//! C语言版本
#if (bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux)
#   if __USE_ISOC11
#    define bsLanguageC bsLanguageC11
#   elif __USE_ISOC99
#    define bsLanguageC bsLanguageC99
#   elif __USE_ISOC95
#    define bsLanguageC bsLanguageC95
#else
#    define bsLanguageC bsLanguageC89
#   endif
#else
#if defined __STDC_VERSION__ && __STDC_VERSION__ >= bsLanguageC11
# define bsLanguageC bsLanguageC11
#elif defined __STDC_VERSION__ && __STDC_VERSION__ >= bsLanguageC99
# define bsLanguageC bsLanguageC99
#elif defined __STDC_VERSION__ && __STDC_VERSION__ >= bsLanguageC95
# define bsLanguageC bsLanguageC95
#else
# define bsLanguageC bsLanguageC89
#endif
#endif

//! C++语言版本
#ifdef __cplusplus
#  include <cstddef>

#if __cplusplus >= bsLanguage14
#define bsLanguage bsLanguage14
#elif __cplusplus >= bsLanguage11
#define bsLanguage bsLanguage11
#else
#define bsLanguage bsLanguage98
#endif

#endif // end C++
#include <stddef.h>


#ifndef bsArchEndian
typedef enum  {
    // least significant byte first or reverse byte order - Intel x86, ...
    Endian_Little = 0,
    // most significant byte first - Motorola, Sparc, MIPS, ...
    Endian_Big    = 1
}eArchEndians;

// returns endian-ness of current CPU
static inline eArchEndians ArchEndian()
{
    // returns endian-ness of cpu.
    union {
        int i;
        unsigned char b[sizeof(int)];
    } u;
    u.i = 1;
    return (u.b[0] == 1) ? Endian_Little : Endian_Big;
}
#define bsArchEndian ArchEndian()
#endif

#endif // BS_PLATFORM

