#include "context_thread.h"

#include "window_base.h"

namespace impl {
class ctx_thread
{
public:
    window_base*  _base;
    ivec4       _viewport ;
    volatile bool _render;
    volatile bool _loop;

    ctx_thread(window_base *_base_):
        _base(_base_),
        _viewport(0,0,640,480),
        _render(true),
        _loop(false)
    {

    }
};
}
context_thread::context_thread(window_base *_base_):
    impl(bsNew (impl::ctx_thread(_base_)))
{
}
context_thread::~context_thread()
{
    bsDelete (impl);
}

void context_thread::stop()
{
    if (impl->_loop)
    {
        impl->_base->lock();
        impl->_render = false;
        impl->_loop = false;
        impl->_base->unlock();
    }
    else
    {
        impl->_render = false;
        impl->_loop = false;
    }
}
bool context_thread::loop()
{
    return impl->_loop;
}
void context_thread::loop(bool mode)
{
    impl->_loop = mode;
}
void context_thread::run()
{
     impl->_base->make_current();
     impl->_base->initializes();
     impl->_base->done_current();
    while (impl->_render)
    {
        impl->_base->lock();
        impl->_base->make_current();

        impl->_base->paints ();

        impl->_base->swap_buffers();
        impl->_base->done_current();
        impl->_base->unlock();

        if (!impl->_loop)
        {
            impl->_base->lock();
            impl->_base->wait();
            impl->_base->unlock();
        }
    }
}
