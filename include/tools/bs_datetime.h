#ifndef BS_DATATIME_H
#define BS_DATATIME_H

#include "bs_define.h"
#include "bs_string.h"
#include "bs_stream.h"

enum TimeType {
        TimeType_RealTime,   //! 系统实时时间
        TimeType_Monotonic,  //! 从系统启动到现在的时间
        TimeType_Process,    //! 从进程启动到现在的时间
        TimeType_Thread      //! 从线程启动到现在的时间
    };

class bs_datetime
{
    friend class bs_timer;
public:
    bs_datetime(ulonglong time_micro = 0);
    bs_datetime(int y, int mh, int d, int h, int me, int s, int ms = 0);
    bs_datetime(int h, int me, int s, int ms = 0);
    bs_datetime(int me, int s, int ms = 0);
    ~bs_datetime();

public:
    int year() const;
    int month() const;
    int day() const;

    int hour() const;
    int minute() const;
    int second() const;
    int msec() const;
    bs_string to_string(const bs_string &form = bs_string("%d-%d-%d %d:%d:%d:%d"))const;
    void from_string(const bs_string &s,
                     const bs_string &form = bs_string("%d-%d-%d %d:%d:%d:%d"));
    ulong to_millisecond()const;

public:
    bool operator !=(const bs_datetime &);
    bool operator ==(const bs_datetime &);

    bool operator >(const bs_datetime &);
    bool operator <(const bs_datetime &);
    bool operator >=(const bs_datetime &);
    bool operator <=(const bs_datetime &);

    bs_datetime operator +(const bs_datetime &);
    bs_datetime operator -(const bs_datetime &);
    bs_datetime &operator +=(const bs_datetime &);
    bs_datetime &operator -=(const bs_datetime &);

public:
    //! 当前系统时间
    static bs_datetime convert_time(ulonglong time_micro); //微妙转时间
    static bs_datetime current_time(); // 当前系统时间
    static ulong millisecond(); // 毫秒
    static ulonglong microsecond();// 微妙
    static timespec convert_timespec(ulonglong milliSeconds);
    static timespec convert_timespec();
private:
    static ulonglong micro_second(TimeType type = TimeType_RealTime);

private:
    //////////////////////
    //! 时间
    uint time_msec;
    uint time_second;
    uint time_minute;
    uint time_hour;

    uint date_year ;
    uint date_month ;
    uint date_day ;

    ulonglong _microsecond_;
};

bs_bstream &operator << (bs_bstream &in, const bs_datetime &v);
bs_bstream &operator >> (bs_bstream &out, bs_datetime &v);
bs_tstream &operator << (bs_tstream &in, const bs_datetime &v);
bs_tstream &operator >> (bs_tstream &out, bs_datetime &v);
#endif // UTIME_H
