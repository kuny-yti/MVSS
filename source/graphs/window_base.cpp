#include "window_base.h"
#include "context_thread.h"

namespace impl {
class base
{
public:
    piwindow      _window;
    pirender      _rbase;
    window_base*    _base;
    context_thread  _thread;
    volatile bool   _is_run;
    bool            _is_ctx;

    pircontext     _context;
    bs_mutex        _mutex;
    bs_condition    _condition;

public:
    base (window_base* _base_):
        _window(NULL),
        _base(_base_),
        _thread(_base_),
        _is_run(false),
        _is_ctx(false),
        _context(0),
        _mutex(),
        _condition()
    {

    }
};
}

window_base::window_base(bs_object *parent):
    bs_object(parent),
    viewport(NULL),
    impl(bsNew (impl::base(this)))
{
}

void window_base::initialize(const piwindow &w, const pirender &r)
{
    impl->_window = w;
    impl->_rbase = r;
    viewport = &(w->rcontext->format.viewport);

    w->connect(this);
}
window_base::~window_base()
{
    bsDelete (impl);
}

void window_base::callback(tObjectNexus *param)
{
    tWindowNexus *nexus = (tWindowNexus*)param;

    if (nexus->event.is_window () && nexus->event.code () & Window_Initialize)
    {
        if (!impl->_is_ctx && impl->_rbase)
        {
            impl->_is_ctx = impl->_rbase->initialize();
            impl->_context = impl->_rbase->context;
            if (!(impl->_window->format.style & Window_MultiThread))
                this->initializes();
        }
        if (impl->_window->format.geometry.zw() != viewport->zw())
            *viewport = ivec4(0,0, impl->_window->format.geometry.width, impl->_window->format.geometry.height) ;
        if (impl->_window->format.style & Window_MultiThread)
        {
            render_start();
            render_activa(impl->_window->format.style & Window_AlwaysRun ? true : impl->_thread.loop());
        }
    }
    else if (nexus->event.is_window () && nexus->event.code () & Window_Exit)
    {
        impl->_is_ctx = false;
        if (impl->_window->format.style & Window_MultiThread)
            render_stop();
    }
    else if (nexus->event.is_window () && nexus->event.code () & Window_Paint)
    {
        if (!impl->_is_ctx && impl->_rbase)
        {
            impl->_is_ctx = impl->_rbase->initialize();
            impl->_context = impl->_rbase->context;
            if (!(impl->_window->format.style & Window_MultiThread))
                this->initializes();
        }
        if (impl->_window->format.geometry.zw () != viewport->zw ())
            *viewport = ivec4(0,0, impl->_window->format.geometry.width, impl->_window->format.geometry.height) ;

        if (impl->_window->format.style & Window_MultiThread)
            render_start();
        render_activa(impl->_window->format.style & Window_AlwaysRun ? true : impl->_thread.loop());
    }
    else if (nexus->event.is_window () && nexus->event.code () & Window_Resize)
    {
        if (!impl->_is_ctx && impl->_rbase)
        {
            impl->_is_ctx = impl->_rbase->initialize();
            impl->_context = impl->_rbase->context;
            if (!(impl->_window->format.style & Window_MultiThread))
                this->initializes();
        }
        if (impl->_window->format.geometry.zw ()!= viewport->zw ())
            *viewport = ivec4(0,0, impl->_window->format.geometry.width, impl->_window->format.geometry.height) ;
        render_activa(impl->_window->format.style & Window_AlwaysRun ? true : impl->_thread.loop());
    }
    else if (nexus->event.code () & Window_Input)
    {
        events(nexus->event);
        render_activa(impl->_window->format.style & Window_AlwaysRun ? true : impl->_thread.loop());
    }
}

pircontext window_base::context()
{
    return impl->_context;
}
void window_base::render_start ()
{
    if (!impl->_is_run)
    {
        impl->_thread.start();
        impl->_is_run = true;
    }
}
void window_base::render_stop( )
{
    if (impl->_is_run)
    {
        impl->_thread.stop();
        impl->_mutex.lock();
        wake_one();
        impl->_mutex.unlock();
        impl->_thread.wait();
        impl->_is_run = false;
    }
}

void window_base::render_activa(bool loop)
{
    if (impl->_window->format.style & Window_MultiThread)
    {
        if (loop != impl->_thread.loop () || !loop)
        {
            impl->_thread.loop(loop);
            wake_one();
        }
    }
    else
    {
        this->make_current();
        this->paints();
        this->swap_buffers();
        this->done_current();
    }
}
void window_base::wait()
{
    impl->_condition.wait(impl->_mutex);
}
void window_base::wake_all()
{
    impl->_condition.wake_all();
}
void window_base::wake_one()
{
    impl->_condition.wake_one();
}
void window_base::lock()
{
    impl->_mutex.lock();
}
void window_base::unlock()
{
    impl->_mutex.unlock();
}
void window_base::swap_buffers()
{
    impl->_context->swap();
}
void window_base::done_current()
{
    impl->_context->done();
}
void window_base::make_current()
{
    impl->_context->make();
}


