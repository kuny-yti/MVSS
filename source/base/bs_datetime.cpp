#include "bs_datetime.h"

#include "bs_string.h"
#define HAVE_CLOCK_GETTIME

#include <cerrno>
#ifdef HAVE_CLOCK_GETTIME
# include <time.h>
#else
# include <sys/time.h>
#endif

#if bsPlatform == bsPlatform_Win32
#include <windows.h>
#elif bsPlatform == bsPlatform_LINUX

#endif

bs_datetime::bs_datetime(ulonglong time_micro):
    time_msec(0),
    time_second(0),
    time_minute(0),
    time_hour(0),
    date_year(0),
    date_month(0),
    date_day(0)
{
    _microsecond_ = 0;
    if (time_micro)
        *this = convert_time(time_micro);
}
bs_datetime::bs_datetime(int y, int mh, int d, int h, int me, int s, int ms):
    time_msec(ms),
    time_second(s),
    time_minute(me),
    time_hour(h),
    date_year(y),
    date_month(mh),
    date_day(d)
{
    _microsecond_ = 0;
}
bs_datetime::bs_datetime(int h, int me, int s, int ms):
    time_msec(ms),
    time_second(s),
    time_minute(me),
    time_hour(h),
    date_year(0),
    date_month(0),
    date_day(0)
{
    _microsecond_ = 0;
}
bs_datetime::bs_datetime(int me, int s, int ms ):
    time_msec(ms),
    time_second(s),
    time_minute(me),
    time_hour(0),
    date_year(0),
    date_month(0),
    date_day(0)
{
    _microsecond_ = 0;
}
bs_datetime::~bs_datetime()
{

}

int bs_datetime::year() const
{
    return date_year;
}
int bs_datetime::month() const
{
    return date_month;
}
int bs_datetime::day() const
{
    return date_day;
}
int bs_datetime::hour() const
{
    return time_hour;
}
int bs_datetime::minute() const
{
    return time_minute;
}
int bs_datetime::second() const
{
    return time_second;
}
int bs_datetime::msec() const
{
    return time_msec;
}
ulong bs_datetime::to_millisecond()const
{
    return _microsecond_ / 1000;
}
bs_string bs_datetime::to_string(const bs_string &form)const
{
    bs_string out;
    out.format(form.to_latin1(), date_year, date_month, date_day,
               time_hour, time_minute, time_second, time_msec);
    return out;
}
void bs_datetime::from_string(const bs_string &s, const bs_string &)
{
    bs_list <bs_string> sp_dt = s.split(" ");
    if (sp_dt.count() ==2)
    {
        if (!sp_dt[0].is_empty())
        {
            bs_list <bs_string> sp_dd = sp_dt[0].split("-");
            if (sp_dd.count() == 3)
            {
                date_year = sp_dd[0].to_uint();
                date_month = sp_dd[1].to_uint();
                date_day = sp_dd[2].to_uint();
            }
        }
        if (!sp_dt[1].is_empty())
        {
            bs_list <bs_string> sp_dd = sp_dt[1].split(":");
            if (sp_dd.count() == 4)
            {
                time_hour = sp_dd[0].to_uint();
                time_minute = sp_dd[1].to_uint();
                time_second = sp_dd[2].to_uint();
                time_msec = sp_dd[3].to_uint();
            }
        }
    }
}

bool bs_datetime::operator !=(const bs_datetime &rhs)
{
    return !(*this == rhs);
}
bool bs_datetime::operator ==(const bs_datetime &rhs)
{
    return _microsecond_ == rhs._microsecond_;
}

bool bs_datetime::operator >(const bs_datetime &rhs)
{
    return _microsecond_ > rhs._microsecond_;
}
bool bs_datetime::operator <(const bs_datetime &rhs)
{
    return _microsecond_ < rhs._microsecond_;
}
bool bs_datetime::operator >=(const bs_datetime &rhs)
{
    return _microsecond_ >= rhs._microsecond_;
}
bool bs_datetime::operator <=(const bs_datetime &rhs)
{
    return _microsecond_ <= rhs._microsecond_;
}
bs_datetime bs_datetime::operator +(const bs_datetime &rhs)
{
    bs_datetime out;
    out.time_msec = rhs.time_msec + time_msec;
    out.time_second = rhs.time_second + time_second;
    out.time_minute = rhs.time_minute  + time_minute;
    out.time_hour = rhs.time_hour + time_hour;
    out.date_year  = rhs.date_year  + date_year;
    out.date_month  = rhs.date_month  + date_month;
    out.date_day  = rhs.date_day  + date_day;
    out._microsecond_ = rhs._microsecond_+ _microsecond_;
    return out;
}
bs_datetime bs_datetime::operator -(const bs_datetime &rhs)
{
    bs_datetime out;
    out.time_msec = time_msec - rhs.time_msec;
    out.time_second = time_second - rhs.time_second;
    out.time_minute = time_minute  - rhs.time_minute;
    out.time_hour = time_hour - rhs.time_hour;
    out.date_year  = date_year  - rhs.date_year;
    out.date_month  = date_month  - rhs.date_month;
    out.date_day  = date_day  - rhs.date_day;
    out._microsecond_ = _microsecond_ - rhs._microsecond_;
    return out;
}
bs_datetime &bs_datetime::operator +=(const bs_datetime &rhs)
{
    time_msec += rhs.time_msec;
    time_second += rhs.time_second;
    time_minute  += rhs.time_minute;
    time_hour += rhs.time_hour;
    date_year  += rhs.date_year;
    date_month  += rhs.date_month;
    date_day  += rhs.date_day;
    _microsecond_ += rhs._microsecond_;
    return *this;
}
bs_datetime &bs_datetime::operator -=(const bs_datetime &rhs)
{
    time_msec -= rhs.time_msec;
    time_second -= rhs.time_second;
    time_minute  -= rhs.time_minute;
    time_hour -= rhs.time_hour;
    date_year  -= rhs.date_year;
    date_month  -= rhs.date_month;
    date_day  -= rhs.date_day;
    _microsecond_ -= rhs._microsecond_;
    return *this;
}
bs_datetime bs_datetime::convert_time(ulonglong total_us)
{
    bs_datetime out;
    int timezone = 8;
    uint64 total_s = total_us / (1000 *1000);
    total_s += 60 *60 *timezone;
    int second = total_s % 60;
    uint64 total_minute = total_s / 60;
    int minute = total_minute % 60;
    uint64 total_hour = total_minute /60;
    int hour = total_hour % 24;
    uint64 total_day = total_hour /24;

    int year = 1970 + (total_day / 366);
    int month = 1 ;
    int day = 1;

    int diff_day = 0; bool leap_year = false;
    while (true)
    {
        int diff = (year - 1970) * 365;
        diff += (year - 1) / 4 -(1970 - 1) /4;
        diff -= (year - 1) / 100 -(1970 - 1) /100;
        diff += (year - 1) / 400 -(1970 - 1) /400;

        diff_day = total_day - diff;
        leap_year = ((year % 4 == 0) && (year %100 != 0)) ||
                (year % 400 == 0);
        if ((!leap_year && (diff_day < 365)) || (leap_year && (diff_day < 366)))
            break;
        else
            year++;
    }
    int index = 0;
    int month_day[2][13] =
    {
        {-1, 0,31,60,91,121,152,182,213,244,274,305,335},
        {-1, 0,31,59,90,120,151,181,212,243,273,304,334}
    };

    if (diff_day >= 59 && leap_year)
        index = 0;
    else
        index = 1;

    for (int i = 13 -1; i >= 1; i--)
    {
        if (diff_day >= month_day[index][i])
        {
            month = i;
            day = diff_day - month_day[index][i] +1;
            break;
        }
    }
    out._microsecond_ = total_us ;
    out.time_msec =   (total_us / 1000) % 1000;
    out.time_second = second ;
    out.time_minute = minute;
    out.time_hour =   hour;

    out.date_day =    day;
    out.date_month =  month;
    out.date_year =   year;
    return out;
}
bs_datetime bs_datetime::current_time()
{
    uint64 total_us = micro_second();
    return convert_time(total_us);
}
ulong bs_datetime::millisecond()
{
    return micro_second() / 1000;
}
ulonglong bs_datetime::microsecond()
{
    return micro_second();
}
timespec bs_datetime::convert_timespec(ulonglong milliSeconds )
{
    timespec ts = { 0, 0 };
    ts.tv_sec  = static_cast<int>( milliSeconds / 1000 );
    ts.tv_nsec = ( milliSeconds - ts.tv_sec * 1000 ) * 1000000;
    return ts;
}
timespec bs_datetime::convert_timespec()
{
    timespec ts = { 0, 0 };
    ulonglong micro = microsecond();
    ts.tv_sec  = static_cast<int>( micro / 1000 /1000 );
    ts.tv_nsec = ( micro - ts.tv_sec * 1000 *1000 ) * 1000;
    return ts;
}
ulonglong bs_datetime::micro_second(TimeType t)
{
#ifdef HAVE_CLOCK_GETTIME

    struct timespec time;
    int r;
    if (t == TimeType_RealTime)
    {
        r = clock_gettime(CLOCK_REALTIME, &time);
    }
    else if (t == TimeType_Monotonic)
    {
        r = clock_gettime(CLOCK_MONOTONIC, &time);
    }
    else if (t == TimeType_Process)
    {
#if defined(CLOCK_PROCESS_CPUTIME_ID)
        r = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
#elif defined(CLOCK_PROF)
        r = clock_gettime(CLOCK_PROF, &time);
#else
        r = -1;
        errno = ENOSYS;
#endif
    }
    else /* t == thread_cpu */
    {
#ifdef CLOCK_PROCESS_CPUTIME_ID
        r = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
#else
        r = -1;
        errno = ENOSYS;
#endif
    }
    if (r != 0)
        throw bs_string(("Cannot get time."));
    ulonglong ret = ulonglong(time.tv_sec) * 1000000;

    return ret + (time.tv_nsec / 1000);

#else

    if (t == TimeType_RealTime || t == TimeType_Monotonic)
    {
        struct timeval tv;
        int r = gettimeofday(&tv, NULL);
        if (r != 0)
            throw bs_string("Cannot get time.");
        return static_cast<ulonglong>(tv.tv_sec) * 1000000 + tv.tv_usec;
    }
    else if (t == TimeType_Process)
    {
        clock_t c = clock();
        return (c * static_cast<ulonglong>(1000000) / CLOCKS_PER_SEC);
    }
    else
    {
        throw bs_string(("Cannot get time."));
    }

#endif
}

#include "bs_stream.h"
bs_bstream &operator << (bs_bstream &in, const bs_datetime &v)
{
    in << v.year();
    in << v.month();
    in << v.day();
    in << v.hour();
    in << v.minute();
    in << v.second();
    in << v.msec();
    return in;
}
bs_bstream &operator >> (bs_bstream &out, bs_datetime &v)
{
    int tmp_year, tmp_month, tmp_day, tmp_hour, tmp_minute, tmp_second, tmp_msec;
    out >> tmp_year ;
    out >> tmp_month ;
    out >> tmp_day ;
    out >> tmp_hour ;
    out >> tmp_minute ;
    out >> tmp_second ;
    out >> tmp_msec;
    v = bs_datetime(tmp_year, tmp_month, tmp_day, tmp_hour, tmp_minute, tmp_second, tmp_msec);
    return out;
}
bs_tstream &operator << (bs_tstream &in, const bs_datetime &v)
{
    in << v.year();
    in << v.month();
    in << v.day();
    in << v.hour();
    in << v.minute();
    in << v.second();
    in << v.msec();
    return in;
}
bs_tstream &operator >> (bs_tstream &out, bs_datetime &v)
{
    int tmp_year, tmp_month, tmp_day, tmp_hour, tmp_minute, tmp_second, tmp_msec;
    out >> tmp_year ;
    out >> tmp_month ;
    out >> tmp_day ;
    out >> tmp_hour ;
    out >> tmp_minute ;
    out >> tmp_second ;
    out >> tmp_msec;
    v = bs_datetime(tmp_year, tmp_month, tmp_day, tmp_hour, tmp_minute, tmp_second, tmp_msec);
    return out;
}
