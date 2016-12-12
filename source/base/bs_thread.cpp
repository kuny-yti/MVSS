
#include <cerrno>
#include <cstring>
#include <pthread.h>
#include <sched.h>
#if bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux
#include<unistd.h>
#endif

#include "bs_thread.h"
#include "bs_datetime.h"
#include "bs_debug.h"

/*
const pthread_spinlock_t bs_spinlock::_spin_initializer = PTHREAD_SPINLOCK_INITIALIZER;

bs_spinlock::bs_spinlock():
    _spin(_spin_initializer)
{
    _is_init=  true;
    int e = pthread_spin_init(&_spin, PTHREAD_PROCESS_PRIVATE);
    if (e != 0)
        _is_init = false;
}
bs_spinlock::bs_spinlock(const bs_spinlock& ):
    _spin(_spin_initializer)
{
    _is_init=  true;
    int e = pthread_spin_init(&_spin, PTHREAD_PROCESS_PRIVATE);
    if (e != 0)
        _is_init = false;
}

bs_spinlock::~bs_spinlock()
{
    if (_is_init)
        (void)pthread_spin_destroy(&_spin);
}
bool bs_spinlock::lock()
{
    if (!_is_init) return false;
    int e = pthread_spin_lock(&_spin);
    if (e != 0)
        return false;
    return true;
}
bool bs_spinlock::trylock()
{
    if (!_is_init) return false;
    int e = pthread_spin_trylock(&_spin);
    if (e != 0)
        return false;
    return true;
}
bool bs_spinlock::unlock()
{
    if (!_is_init) return false;
    int e = pthread_spin_unlock(&_spin);
    if (e != 0)
        return false;
    return true;
}
*/
bs_spinlock::bs_spinlock():
    slock(false)
{
}
bs_spinlock::bs_spinlock(const bs_spinlock& ):
    slock(false)
{

}
bs_spinlock::~bs_spinlock()
{
    unlock();
}

// Lock the mutex.
bool bs_spinlock::lock()
{
    while (!slock.bcswap(false, true))
        bs_thread::yield();
    return true;
}
// Try to lock the mutex. Return true on success, false otherwise.
bool bs_spinlock::trylock()
{
    if(!slock.bcswap(false, true))
        return false;
    return true;
}
// Unlock the mutex
bool bs_spinlock::unlock()
{
    return slock.bcswap(true, false);
}
/*
 * Read Write Lock
 *
 */
const pthread_rwlock_t bs_rwlock::_rw_initializer = PTHREAD_RWLOCK_INITIALIZER;


bs_rwlock::bs_rwlock():
    _rw(_rw_initializer)
{
    _is_init=  true;
    int e = pthread_rwlock_init(&_rw, NULL);
    if (e != 0)
        _is_init = false;
}
bs_rwlock::bs_rwlock(const bs_rwlock& ):
    _rw(_rw_initializer)
{
    _is_init=  true;
    int e = pthread_rwlock_init(&_rw, NULL);
    if (e != 0)
        _is_init = false;
}
bs_rwlock::~bs_rwlock()
{
    if (_is_init)
        (void)pthread_rwlock_destroy(&_rw);
}

// Unlock the mutex
bool bs_rwlock::unlock()
{
    if (!_is_init) return false;
    int e = pthread_rwlock_unlock(&_rw);
    if (e != 0)
        return false;
    return true;
}
bool bs_rwlock::lockr()
{
    if (!_is_init) return false;
    int e = pthread_rwlock_rdlock(&_rw);
    if (e != 0)
        return false;
    return true;
}
bool bs_rwlock::lockw()
{
    if (!_is_init) return false;
    int e = pthread_rwlock_wrlock(&_rw);
    if (e != 0)
        return false;
    return true;
}
bool bs_rwlock::trylockr()
{
    if (!_is_init) return false;
    int e = pthread_rwlock_tryrdlock(&_rw);
    if (e != 0)
        return false;
    return true;
}
bool bs_rwlock::trylockw()
{
    if (!_is_init) return false;
    int e = pthread_rwlock_trywrlock(&_rw);
    if (e != 0)
        return false;
    return true;
}
//int WINPTHREAD_API pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock, const struct timespec *ts);
//int WINPTHREAD_API pthread_rwlock_timedrdlock(pthread_rwlock_t *l, const struct timespec *ts);

const pthread_mutex_t bs_mutex::_mutex_initializer = PTHREAD_MUTEX_INITIALIZER;

bs_mutex::bs_mutex() : _mutex(_mutex_initializer)
{
    _is_init=  true;
    int e = pthread_mutex_init(&_mutex, NULL);
    if (e != 0)
        _is_init = false;
}

bs_mutex::bs_mutex(const bs_mutex&) : _mutex(_mutex_initializer)
{
    _is_init=  true;
    // You cannot have multiple copies of the same mutex.
    // Instead, we create a new one. This allows easier use of mutexes in STL containers.
    int e = pthread_mutex_init(&_mutex, NULL);
    if (e != 0)
        _is_init = false;
}

bs_mutex::~bs_mutex()
{
    if (_is_init)
    (void)pthread_mutex_destroy(&_mutex);
}

bool bs_mutex::lock()
{
    if (!_is_init) return false;
    int e = pthread_mutex_lock(&_mutex);
    if (e != 0)
        return false;
    return true;
}

bool bs_mutex::trylock()
{
    if (!_is_init) return false;
    return (pthread_mutex_trylock(&_mutex) == 0);
}

bool bs_mutex::unlock()
{
    if (!_is_init) return false;
    int e = pthread_mutex_unlock(&_mutex);
    if (e != 0)
        return false;
    return true;
}
bs_momlock::bs_momlock(bs_mutex *m):
    mutex(m)
{
    if (mutex)
        mutex->lock();
}
bs_momlock::~bs_momlock()
{
    if (mutex)
        mutex->unlock();
}

bool bs_momlock::unlock()
{
    if (mutex)
        return mutex->unlock();
    return false;
}
bool bs_momlock::relock()
{
    if (mutex)
        return mutex->lock();
    return false;
}

const pthread_cond_t bs_condition::_cond_initializer = PTHREAD_COND_INITIALIZER;

bs_condition::bs_condition() : _cond(_cond_initializer)
{
    _is_init = true;
    int e = pthread_cond_init(&_cond, NULL);
    if (e != 0)
        _is_init = false;
}

bs_condition::bs_condition(const bs_condition&) : _cond(_cond_initializer)
{
    _is_init = true;
    // You cannot have multiple copies of the same condition.
    // Instead, we create a new one. This allows easier use of conditions in STL containers.
    int e = pthread_cond_init(&_cond, NULL);
    if (e != 0)
        _is_init = false;
}

bs_condition::~bs_condition()
{
    if (!_is_init) return ;
    (void)pthread_cond_destroy(&_cond);
}

bool bs_condition::wait(bs_mutex& m)
{
    if (!_is_init) return false;
    int e = pthread_cond_wait(&_cond, &m._mutex);
    if (e != 0)
        return false;
    return true;
}
bool bs_condition::wait(bs_mutex& m, uint64 timeout )
{
    if( timeout == TIMEOUT_INDEFINITE )
        return wait(m);

    const uint32_t time = timeout;

    const timespec delta = bs_datetime::convert_timespec(time);
    bs_datetime now_dt = bs_datetime::current_time();

    timespec then;
    then.tv_sec  = delta.tv_sec + now_dt.second();
    then.tv_nsec = delta.tv_nsec + now_dt.msec() * 1000000;
    while( then.tv_nsec > 1000000000 )
    {
        ++then.tv_sec;
        then.tv_nsec -= 1000000000;
    }

    int error = pthread_cond_timedwait( &_cond, &m._mutex, &then );

    if( error == ETIMEDOUT )
        return false;
    return true;
}

bool bs_condition::wake_one()
{
    if (!_is_init) return false;
    int e = pthread_cond_signal(&_cond);
    if (e != 0)
        return false;
    return true;
}

bool bs_condition::wake_all()
{
    if (!_is_init) return false;
    int e = pthread_cond_broadcast(&_cond);
    if (e != 0)
        return false;
    return true;
}


const int bs_thread::priority_default;

static int policy_int(eThreadPolicys tp)
{
    return tp == Policy_Default ? SCHED_IDLE : tp == Policy_FIFO
                                  ? SCHED_FIFO : SCHED_RR;
}
static eThreadPolicys int_policy (int v)
{
    return v == SCHED_IDLE ? Policy_Default : v == SCHED_FIFO
                             ? Policy_FIFO : Policy_RoundRobin;
}


#ifdef hasThreadTls
struct tThreadTlss
{
    static __thread tThreadDatas *tls_tdata ;
    static pthread_once_t thread_data_once ;
    static pthread_key_t thread_data_key;

    static void set(tThreadDatas *data)
    {
#ifdef hasThreadTls
        tls_tdata = data;
#endif
        pthread_once(&thread_data_once, create_thread_data_key);
        pthread_setspecific(thread_data_key, data);
    }
    static tThreadDatas * get()
    {
#ifdef hasThreadTls
        return tls_tdata;
#else
        pthread_once(&thread_data_once, create_thread_data_key);
        return reinterpret_cast<tThreadDatas *>(pthread_getspecific(thread_data_key));
#endif
    }
    static void clear()
    {
#ifdef hasThreadTls
        tls_tdata = NULL;
#endif
        pthread_setspecific(thread_data_key, 0);
    }

    static void destroy_thread_data_key()
    {
        pthread_once(&thread_data_once, create_thread_data_key);
        pthread_key_delete(thread_data_key);

        pthread_once_t pthread_once_init = PTHREAD_ONCE_INIT;
        thread_data_once = pthread_once_init;
    }
    static void create_thread_data_key()
    {
        pthread_key_create(&thread_data_key, destroy_thread_data);
    }
    static void destroy_thread_data(void *p)
    {
        pthread_setspecific(thread_data_key, p);
        tThreadDatas *data = static_cast<tThreadDatas *>(p);
        if (data->lessref ())
            data->destroy ();

        pthread_setspecific(thread_data_key, 0);
    }
};
__thread tThreadDatas *tThreadTlss::tls_tdata = NULL;
pthread_once_t tThreadTlss::thread_data_once  = PTHREAD_ONCE_INIT;
pthread_key_t tThreadTlss::thread_data_key;

static tThreadTlss gttls;
void bs_thread::tls_set (tThreadDatas * td)
{
    gttls.set (td);
}
tThreadDatas *bs_thread::tls_get ()
{
    return gttls.get ();
}
void bs_thread::tls_clear()
{
    gttls.clear ();
}
#endif
bs_map<thread_id, bs_thread*> bs_thread::thread_map;

bs_thread::bs_thread() :
    __thread_id(0),
    __joinable(false),
    __running(false),
    __wait_mutex()
{
    __priority = (eThreadPrioritys)priority_default;
    __policy = Policy_Default;

    pthread_attr_t thread_attr;
    struct sched_param param_;
    pthread_attr_init(&thread_attr);

    pthread_attr_getschedpolicy(&thread_attr, (int*)&__policy);
    pthread_attr_getschedparam(&thread_attr, &param_);
    __priority = (eThreadPrioritys)param_.sched_priority;
    pthread_attr_destroy(&thread_attr);

    __priority_max = sched_get_priority_max(__policy);
    __priority_min = sched_get_priority_min(__policy);

    __policy = int_policy(__policy);
}

bs_thread::~bs_thread()
{
    if (__joinable)
        (void)pthread_detach(__thread_id);

    thread_map.remove (__thread_id);
}

void* bs_thread::__run(void* p)
{
    bs_thread* t = static_cast<bs_thread*>(p);

    try {
    t->run();
    }
    catch(...)
    {
        t->__running = false;
        thread_map.remove (t->__thread_id);
        throw;
    }
    t->__running = false;
    thread_map.remove (t->__thread_id);
    return NULL;
}

bool bs_thread::policy(eThreadPolicys tp)
{
    if (tp != __policy)
    {
        if (!is_running())
        {
            __policy = tp;
            return true;
        }
        return false;
    }
    return true;
}
eThreadPolicys bs_thread::policy()const
{
    return __policy;
}

bool bs_thread::priority(eThreadPrioritys prio)
{
    int sch_level = 0;
    int sch_policy = SCHED_IDLE;
    sched_param param;

    bs_momlock mlock(&__wait_mutex);

    if (!is_running ())
    {
        log_warning("Cannot set priority, thread is not running");
        return false;
    }

#ifdef SCHED_IDLE
    if (prio == Priority_Idle)
    {
        policy (Policy_Default);
        sch_policy = SCHED_IDLE; // SCHED_IDLE
        sch_level = 0;
    }

    const int lowestPriority = Priority_Lowest;
#else
    const int lowestPriority = Priority_Idle;
#endif
    const int highestPriority = Priority_TimeCritical;

    sch_policy = policy_int(policy());
    if (pthread_getschedparam(__thread_id, &sch_policy, &param) != 0)
    {
        log_warning("Cannot get scheduler parameters");
        return false;
    }
    __priority_max = sched_get_priority_max(sch_policy);
    __priority_min = sched_get_priority_min(sch_policy);
    if (__priority_min == -1 || __priority_max == -1)
        return false;
    if (prio != Priority_Idle)
    {
        sch_level = ((prio - lowestPriority) * (__priority_max - __priority_min) / highestPriority) + __priority_min;
        sch_level = bs_bound(__priority_min, sch_level, __priority_max);
    }

    param.sched_priority = sch_level;
    int status = pthread_setschedparam(__thread_id, sch_policy, &param);
# ifdef SCHED_IDLE
    // were we trying to set to idle priority and failed?
    if (status == -1 && sch_policy == SCHED_IDLE && errno == EINVAL)
    {
        pthread_getschedparam(__thread_id, &sch_policy, &param);
        param.sched_priority = sched_get_priority_min(sch_policy);
        pthread_setschedparam(__thread_id, sch_policy, &param);
        return false;
    }
# else
    if (status != 0)
        return false;
# endif

    __priority = prio;
    return true;
}
eThreadPrioritys bs_thread::priority()const
{
    return __priority;
}
bool bs_thread::start(eThreadPolicys tp)
{
    if (__running.bcswap (false, true))
    {
        wait();
        pthread_attr_t priority_thread_attr;
        struct sched_param param_;

        pthread_attr_init(&priority_thread_attr);
        pthread_attr_setdetachstate(&priority_thread_attr, PTHREAD_CREATE_DETACHED);

        // set policy
        if (policy() != tp || tp != Policy_Default)
        {
            if(pthread_attr_setschedpolicy(&priority_thread_attr, policy_int (__policy)) == 0)
            {
                __priority_max = sched_get_priority_max(policy_int (__policy));
                __priority_min = sched_get_priority_min(policy_int (__policy));
                __policy = tp;
            }
        }
        else
        {
            int sch_policy = Policy_Default;
            if (pthread_attr_getschedpolicy(&priority_thread_attr, &sch_policy) != 0)
                log_warning("Cannot determine default scheduler policy");
            else
                __policy = int_policy (sch_policy);
        }

        // set priority
        if (priority() == Priority_Inherit)
            pthread_attr_setinheritsched(&priority_thread_attr, PTHREAD_INHERIT_SCHED);
        else
        {

#if SCHED_IDLE
                const int lowestPriority = Priority_Lowest;
#else
                const int lowestPriority = Priority_Idle;
#endif
                const int highestPriority = Priority_TimeCritical;
                if (__priority_min == -1 || __priority_max == -1)
                    param_.sched_priority = 0;
                else
                {
                    int level = ((priority() - lowestPriority) * (__priority_max - __priority_min) / highestPriority) + __priority_min;
                    param_.sched_priority = bs_bound(__priority_min, level, __priority_max);
                }

                if (pthread_attr_setinheritsched(&priority_thread_attr, PTHREAD_EXPLICIT_SCHED) != 0
                    || pthread_attr_setschedparam(&priority_thread_attr, &param_) != 0)
                {
                    pthread_attr_setinheritsched(&priority_thread_attr, PTHREAD_INHERIT_SCHED);
                    priority(Priority_Idle);
                }
        }

        // create thread
        int e = pthread_create(&__thread_id, &priority_thread_attr, __run, this);
        if (e == EPERM)
        {
            pthread_attr_setinheritsched(&priority_thread_attr, PTHREAD_INHERIT_SCHED);
            e = pthread_create(&__thread_id, &priority_thread_attr, __run, this);
        }
        pthread_attr_destroy(&priority_thread_attr);
        if (e != 0)
            return false;

        thread_map.append (__thread_id, this);
        __joinable = true;
    }
    return true;
}

bool bs_thread::wait()
{
    if (id() == current_id ())
    {
        log_warning("Thread tried to wait on itself");
        return false;
    }

    __wait_mutex.lock();
    if (__joinable.bcswap (true, false))
    {
        int e = pthread_join(__thread_id, NULL);
        if (e != 0) {
            __wait_mutex.unlock();
            return false;
        }
    }
    __wait_mutex.unlock();
    return true;
}

bool bs_thread::finish()
{
    return wait();
}
thread_id bs_thread::id()const
{
    return __thread_id;
}
bool bs_thread::operator != (const bs_thread &rhs)const
{
    return pthread_equal(__thread_id, rhs.__thread_id) == 0;
}
bool bs_thread::operator == (const bs_thread &rhs)const
{
    return pthread_equal(__thread_id, rhs.__thread_id) != 0;
}
bool bs_thread::cancel()
{
    __wait_mutex.lock();
    int e = pthread_cancel(__thread_id);
    if (e != 0) {
        __wait_mutex.unlock();
        return false;
    }
    __wait_mutex.unlock();
    thread_map.remove (__thread_id);
    return true;
}
void bs_thread::yield()
{
#if bsCompiler == bsCompiler_MSVC
    SwitchToThread();
    //::Sleep( 0 ); // sleeps thread
#else
#   if bsPlatform == bsPlatform_Apple
    ::pthread_yield_np();
#   else
    ::sched_yield();
#   endif
#endif
}
void bs_thread::sleep(uint s)
{
#if bsPlatform == bsPlatform_Win32
    ::Sleep(s * 1000);
#else
#   if (bsPlatform == bsPlatform_Unix) || (bsPlatform == bsPlatform_Android) || (bsPlatform == bsPlatform_Linux)
    ::sleep(s);
#   endif
#endif
}
void bs_thread::msleep(ulong ms)
{
#if bsPlatform == bsPlatform_Win32
    ::Sleep(ms);
#elif bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Android || bsPlatform == bsPlatform_Linux
    ::usleep(ms*1000);
#endif

}

void bs_thread::usleep(uint64 um)
{
#if bsPlatform == bsPlatform_Win32
    ::Sleep((um / 1000) <=0 ? 1 : um / 1000);
#else
#   if bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Android || bsPlatform == bsPlatform_Linux
    ::usleep(um);
#   endif
#endif
}
thread_id bs_thread::current_id()
{
    return pthread_self();
}
bs_thread *bs_thread::current()
{
    thread_id ids = current_id();
    return thread_map.value (ids);
}
