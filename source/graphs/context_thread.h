#ifndef CONTEXT_THREAD_H
#define CONTEXT_THREAD_H
#include "maths/bs_vec4.h"
#include "bs_thread.h"
#include "bs_ptr.h"
class window_base ;
namespace impl {
class ctx_thread;
}
class context_thread :public bs_thread
{
public:
    explicit context_thread(window_base *_base_);
    ~context_thread();

public:
    void stop();
    void loop(bool mode);
    bool loop();

protected:
    void run();

private:
    impl::ctx_thread *impl;
friend class window_base;
};

#endif // CONTEXT_THREAD_H
