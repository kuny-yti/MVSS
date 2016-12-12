
#ifndef THREADS_H
#define THREADS_H
#include "bs_define.h"
#include "tools/bs_map.h"
#include "tools/bs_stack.h"

#include <sched.h>
#include <errno.h>
#if bsPlatform == bsPlatform_Linux
#include <sys/prctl.h>
#endif

#define TIMEOUT_INDEFINITE 0xfffffffffffffffful

#if bsPlatform == bsPlatform_Linux && !defined(SCHED_IDLE)
// from linux/sched.h
# define SCHED_IDLE    5
#endif

#if bsPlatform == bsPlatform_Linux && defined(__GLIBC__)
#define hasThreadTls
#endif

/*!
 * @brief The bs_spinlock class Spin (atomic)
 * @class bs_spinlock
 */
class bs_spinlock
{
private:
    bs_atomic<bool> slock;
public:
    explicit bs_spinlock();
    explicit bs_spinlock(const bs_spinlock& m);
    ~bs_spinlock();

    // Lock the mutex.
    bool lock();
    // Try to lock the mutex. Return true on success, false otherwise.
    bool trylock();
    // Unlock the mutex
    bool unlock();

};

/*
 * Spin
 *
 * /
class bs_spinlock
{
    bsMemManage()
private:
    static const pthread_spinlock_t _spin_initializer;
    pthread_spinlock_t _spin;
    bool _is_init;
public:
    explicit bs_spinlock();
    explicit bs_spinlock(const bs_spinlock& m);
    ~bs_spinlock();

    // Lock the mutex.
    bool lock();
    // Try to lock the mutex. Return true on success, false otherwise.
    bool trylock();
    // Unlock the mutex
    bool unlock();

};*/

/*!
 * @brief The bs_rwlock class  Read Write Lock
 * @class bs_rwlock
 */
class bs_rwlock
{
private:
    static const pthread_rwlock_t _rw_initializer;
    pthread_rwlock_t _rw;
    bool _is_init;
public:
    explicit bs_rwlock();
    explicit bs_rwlock(const bs_rwlock& m);
    ~bs_rwlock();

    // Lock the mutex.
    bool lockr();
    bool lockw();
    // Try to lock the mutex. Return true on success, false otherwise.
    bool trylockr();
    bool trylockw();
    // Unlock the mutex
    bool unlock();

};

/*!
 * @brief The bs_mutex class  Mutex Lock
 * @class bs_mutex
 */
class bs_mutex
{
private:
    static const pthread_mutex_t _mutex_initializer;
    pthread_mutex_t _mutex;
    bool _is_init;

public:
    // Constructor / Destructor
    explicit bs_mutex();
    explicit bs_mutex(const bs_mutex& m);
    ~bs_mutex();

    // Lock the mutex.
    bool lock();
    // Try to lock the mutex. Return true on success, false otherwise.
    bool trylock();
    // Unlock the mutex
    bool unlock();

    friend class bs_condition;
    friend class bs_moment_lock;
};

/*!
 * @brief The bs_momlock class 片刻锁
 * @class bs_momlock
 */
class bs_momlock
{
public:
    explicit bs_momlock(bs_mutex *m);
    ~bs_momlock();

    bool unlock();
    bool relock();

private:
    bs_mutex *mutex;
};

/*!
 * @brief The bs_condition class Wait condition
 * @class bs_condition
 */
class bs_condition
{
private:
    static const pthread_cond_t _cond_initializer;
    pthread_cond_t _cond;
    bool _is_init;

public:
    // Constructor / Destructor
    explicit bs_condition();
    explicit bs_condition(const bs_condition& c);
    ~bs_condition();

    // Wait for the condition. The calling thread must have the mutex locked.
    bool wait(bs_mutex& m);
    // ms
    bool wait(bs_mutex& m, uint64 timeout /*= TIMEOUT_INDEFINITE*/);
    // Wake one thread that waits on the condition.
    bool wake_one(); //- signal
    // Wake all threads that wait on the condition.
    bool wake_all(); //- broadcast

};

typedef enum
{
    Policy_Default = 0,    //!< 系统默认
    Policy_FIFO = 1,       //!< 先进先出
    Policy_RoundRobin = 2  //!< 轮询
}eThreadPolicys;
typedef enum
{
    Priority_Idle,
    Priority_Lowest,
    Priority_Low,
    Priority_Normal,
    Priority_High,
    Priority_Highest,

    Priority_TimeCritical,

    Priority_Inherit
}eThreadPrioritys;

typedef pthread_t thread_id;
struct tThreadDatas :public bs_ref
{
    virtual void destroy() = 0;
};

/*!
 * @brief The bs_thread class Thread
 * @class bs_thread
 * Implement the run() function in a subclass.
 */
class bs_thread
{
protected:
    thread_id __thread_id;
    bs_atomic<bool> __joinable;
    bs_atomic<bool> __running;
    bs_mutex __wait_mutex;

    static void* __run(void* p);

public:
    static const int priority_default = Priority_Idle;

    explicit bs_thread();
    virtual ~bs_thread();

    //!
    //! \brief policy 线程的策略
    //! \param tp
    //! \return
    //!
    bool policy(eThreadPolicys tp);
    eThreadPolicys policy()const;

    //!
    //! \brief priority 线程的优先权
    //! \param prio
    //! \return
    //!
    bool priority(eThreadPrioritys prio);
    eThreadPrioritys priority() const;

    //!
    //! \brief run 需要执行的线程，继承
    //!
    virtual void run() = 0;

    //!
    //! \brief start 启动一个线程来执行run函数，若在运行则什么都不做
    //! \param tp
    //! \return
    //!
    bool start(eThreadPolicys tp = Policy_Default);

    //!
    //! \brief is_running 返回线程是否在运行
    //! \return
    //!
    bool is_running() {return __running;}

    //!
    //! \brief wait 等待线程运行结束，若没有运行则直接返回
    //! \return
    //!
    bool wait();

    //!
    //! \brief finish  等待线程运行结束
    //! \return
    //!
    bool finish();

    //!
    //! \brief cancel  取消线程，不安全请勿使用
    //! \return
    //!
    bool cancel(); //terminate

    //!
    //! \brief id 回线程id
    //! \return
    //!
    thread_id id()const;

    //!
    //! \brief operator 比较两个线程是否相同
    //! \param rhs
    //! \return
    //!
    bool operator != (const bs_thread &rhs)const;
    bool operator == (const bs_thread &rhs)const;

public:
    //!
    //! \brief sleep 睡眠s秒
    //! \param s
    //!
    static void sleep(uint s);//s
    //!
    //! \brief msleep 睡眠ms毫秒
    //! \param ms
    //!
    static void msleep(ulong ms);//ms
    //!
    //! \brief usleep 睡眠um微妙
    //! \param um
    //!
    static void usleep(uint64 um);//us
    //!
    //! \brief yield 出让执行权
    //!
    static void yield();
    //!
    //! \brief current 返回当前线程
    //! \return
    //!
    static bs_thread *current();
    //!
    //! \brief current_id 返回当前线程id
    //! \return
    //!
    static thread_id current_id ();
    //!
    //! \brief cpu_count 返回CPU核心数
    //! \return
    //!
    static int cpu_count();

    #ifdef hasThreadTls
    //!
    //! \brief tls_set 设置线程TLS的数据
    //! \param td
    //!
    static void tls_set(tThreadDatas * td);
    //!
    //! \brief tls_get 获取线程TLS的数据
    //! \return
    //!
    static tThreadDatas *tls_get();
    //!
    //! \brief tls_clear 清除线程TLS数据
    //!
    static void tls_clear();
    #endif

private:
    eThreadPrioritys __priority;
    int __priority_min;
    int __priority_max;
    eThreadPolicys __policy;

    static bs_map<thread_id, bs_thread*> thread_map;
};

#endif
