#ifndef BS_TIMER
#define BS_TIMER

#include "bs_datetime.h"
#include "bs_thread.h"
#include "bs_object.h"

typedef enum
{
    TimerState_Pause,
    TimerState_Stop,
    TimerState_Run,
}TimerState;
typedef enum
{
    TimerMode_Time, //! 计时器
    TimerMode_Timing//! 定时器
}TimerMode;


class bs_timer :public bs_thread, public bs_object
{
    friend class bs_datetime;
public:
    //! 模式 检测间隔时间(ms)
    explicit bs_timer(TimerMode mode = TimerMode_Time, uint64 _interval = 10);

    //! 启动，参数为超时时间(ms)。注：若为0则跟线程运行没什么区别
    void start(uint64 msec = 0);
    //! 停止
    void stop(bool is_wait = false);
    //! 暂停
    void pause();
    //! 继续
    void resume();
    //! 状态
    TimerState state() const;
    //! 检测间隔时间
    uint64 interval()const;
    TimerMode mode()const;

    //! 计时器功能
    //! 复位计时器时间
    int restart();
    //! 计时器过去的时间
    int elapsed() const;

protected:
    virtual void run();

private:
    //! 定时器
    uint64 last_time;
    uint64 timer_timeout;
    TimerState current_state;
    uint64 thread_interval;
    TimerMode timer_mode;

    bs_datetime tally;

};

#endif // BS_TIMER

