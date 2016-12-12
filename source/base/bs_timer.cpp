
#include "bs_timer.h"

bs_timer::bs_timer(TimerMode m, uint64 _interval):
    bs_thread(),
    last_time(0),
    timer_timeout(0),
    current_state(TimerState_Stop),
    thread_interval(_interval),
    timer_mode(m),
    tally()
{
    start();
}
void bs_timer::start(uint64 msec)
{
    last_time = tally.micro_second() / 1000;
    if (mode() == TimerMode_Time)
        return;

    timer_timeout = msec;
    current_state = TimerState_Run;
    bs_thread::start();
}

void bs_timer::stop(bool is_wait)
{
    last_time = 0;
    if (mode() == TimerMode_Time)
        return;

    timer_timeout = 0;
    current_state = TimerState_Stop;
    if (is_wait)
        bs_thread::finish();
}

void bs_timer::pause()
{
    if (mode() == TimerMode_Time)
        return;
    if (current_state != TimerState_Stop)
        current_state = TimerState_Pause;
}

void bs_timer::resume()
{
    if (mode() == TimerMode_Time)
        return;
    if (current_state != TimerState_Stop)
        current_state = TimerState_Run;
}

TimerState bs_timer::state() const
{
    return current_state;
}

uint64 bs_timer::interval()const
{
    return thread_interval;
}

TimerMode bs_timer::mode()const
{
    return timer_mode;
}

int bs_timer::restart()
{
    uint64 t = last_time;
    last_time = tally.micro_second() / 1000;
    return last_time - t;
}

int bs_timer::elapsed() const
{
    uint64 t = tally.micro_second() / 1000;
    return t - last_time ;
}

void bs_timer::run()
{
    bool _state_ = true;
    while (_state_)
    {
        switch (current_state)
        {
        case TimerState_Stop:
            _state_ = false;
            break;
        case TimerState_Run:
            if ((uint64)elapsed() >= timer_timeout)
            {
                this->callback(NULL);

                restart();
            }
            else
                msleep(thread_interval);
            break;
        case TimerState_Pause:
            msleep(thread_interval);
            break;
        default:
            break;
        }
    }
}
