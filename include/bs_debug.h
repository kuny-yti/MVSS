#ifndef BS_PRINT
#define BS_PRINT
#include "bs_define.h"
#include "tools/bs_stream.h"
#include "tools/bs_string.h"

#ifdef bsDebug_Enable
#if bsLanguage < bsLanguage11
#   define log_about(val) bsDebug::print_debug(bsDebug::LOG_ABOUT, val)
#   define log_warning(val) bsDebug::print_debug(bsDebug::LOG_WARNING, val)
#   define log_error(val) bsDebug::print_debug(bsDebug::LOG_ERROR, val)
#   define log_information(val) bsDebug::print_debug(bsDebug::LOG_INFO, val)

#   define log_abouts(msg, ...) bsDebug::print_debug(bsDebug::LOG_ABOUT, msg, __VA_ARGS__)
#   define log_warnings(msg, ...) bsDebug::print_debug(bsDebug::LOG_WARNING, msg, __VA_ARGS__)
#   define log_errors(msg, ...) bsDebug::print_debug(bsDebug::LOG_ERROR, msg, __VA_ARGS__)
#   define log_informations(msg, ...) bsDebug::print_debug(bsDebug::LOG_INFO, msg, __VA_ARGS__)
#else
#   define log_about(val) bsDebug::print_arg(bsDebug::debug_header(), bsDebug::debug_level(bsDebug::LOG_ABOUT), val)
#   define log_warning(val) bsDebug::print_arg(bsDebug::debug_header(), bsDebug::debug_level(bsDebug::LOG_WARNING), val)
#   define log_error(val) bsDebug::print_arg(bsDebug::debug_header(), bsDebug::debug_level(bsDebug::LOG_ERROR), val)
#   define log_information(val) bsDebug::print_arg(bsDebug::debug_header(), bsDebug::debug_level(bsDebug::LOG_INFO), val)

#   define log_abouts(msg, ...) bsDebug::print_arg(bsDebug::debug_header(), bsDebug::debug_level(bsDebug::LOG_ABOUT), msg, __VA_ARGS__)
#   define log_warnings(msg, ...) bsDebug::print_arg(bsDebug::debug_header(), bsDebug::debug_level(bsDebug::LOG_WARNING),msg,  __VA_ARGS__)
#   define log_errors(msg, ...) bsDebug::print_arg(bsDebug::debug_header(), bsDebug::debug_level(bsDebug::LOG_ERROR), msg, __VA_ARGS__)
#   define log_informations(msg, ...) bsDebug::print_arg(bsDebug::debug_header(), bsDebug::debug_level(bsDebug::LOG_INFO), msg, __VA_ARGS__)
#endif

namespace bsDebug
{
    typedef enum
    {
        LOG_INFO = 0x0,
        LOG_WARNING,
        LOG_ERROR,
        LOG_ABOUT,
    }LogLevel;

#if bsLanguage >= bsLanguage11

template <typename T>
void unpack(bs_tstream &tdbg, const T& t){tdbg << (t) << ' ';}

template <typename ... Args>
void prntf_private(const Args& ... args)
{
    static bs_byte barr;
    static bs_tstream dbg(&barr);
    int dummy[] = {0 , (unpack(dbg, args), 0)...};
    *dummy = *dummy;
    dbg << '\n';
    dbg.dump();
    barr.clear();
}


template <typename ... Args>
void print_arg(const Args& ... args) {prntf_private((args)...);}

#endif
    void print_debug(LogLevel, const char *form, ...);
    char *debug_level(LogLevel);
    char *debug_header();
}

#else
#   define log_about(val)
#   define log_warning(val)
#   define log_error(val)
#   define log_information(val)

#   define log_abouts(msg, ...)
#   define log_warnings(msg, ...)
#   define log_errors(msg, ...)
#   define log_informations(msg, ...)

#endif


#ifdef bsAssert_Enable
#    if bsPlatform == bsPlatform_Unix
#        define bsASSERT(val, str)
#    elif bsPlatform == bsPlatform_Linux
#        define bsASSERT(val, str)
#    elif bsPlatform == bsPlatform_Apple
#        define bsASSERT(val, str)
#    elif bsPlatform == bsPlatform_Win32
#        define bsASSERT(val, str) \
           if (!!(val)) \
           {\
               log_abouts("file:%s -> line:%d -> function:%s [%s]",\
               __FILE__, __LINE__, __FUNCTION__, str);abort ();\
           }
#    endif
#else
#  define bsASSERT(val, str)
#endif

#endif // BS_PRINT

