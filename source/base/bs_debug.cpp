#include "bs_debug.h"
#include "bs_datetime.h"

char *bsDebug::debug_level(LogLevel levl)
{
    static struct
    {
        LogLevel lev;
        const char *const str;
    } dlve[] =
    {
    {LOG_INFO, "[Info] "},
    {LOG_WARNING, "[Warning] "},
    {LOG_ERROR, "[Error] "},
    {LOG_ABOUT, "[About] "}
    };
    return (char*)dlve[levl].str;
}
char *bsDebug::debug_header()
{
    static char dbg[64] = {'\0'};
    bs_string dtstr = bs_datetime::current_time().to_string();

    bs_memory::copy_sse(dbg, dtstr.to_latin1(), dtstr.bytecount());
    return dbg;
}
#include <stdarg.h>
//! 实现调用打印函数(应该反馈到GUI上)。实现打印到日志。
void bsDebug::print_debug(LogLevel levl, const char *form, ...)
{
    bs_datetime dt = bs_datetime::current_time();
    bs_string head;
    head = dt.to_string()+ "  ";
    switch(levl)
    {
    case LOG_WARNING:
        head += "[Warning] ";
        break;
    case LOG_ERROR:
        head += "[Error] ";
        break;
    case LOG_ABOUT:
        head += "[About] ";
        break;
    case LOG_INFO:
        head += "[Info] ";
        break;
    }
    char sMessage[2048] = {'\0'};
    va_list args;
    va_start(args, form);
    vsnprintf(sMessage, 2048-1, form, args);
    sMessage[2048-1] = '\0';
    va_end(args);

    printf("%s%s", head.to_latin1(), sMessage);
    //bs_logs::write(head, LOG_ERROR); //! 修改可以输出日志级别
}
