#ifndef WINDOW_BASE_H
#define WINDOW_BASE_H
#include "bs_platform.h"
#include "render/irender.h"
#include "graphs/igraphs.h"

namespace impl {
class base;
}
class window_base: public bs_object
{
    bsObject(window_base)
public:
    window_base(bs_object *parent = NULL);
    ~window_base();

    void initialize(const piwindow &w, const pirender &rctx);

protected:
    ivec4 *viewport;

public:
    virtual void callback(tObjectNexus *param);

protected://渲染控制
    void render_start();
    void render_stop();

public://上下文操作
    void wait();
    void wake_all();
    void wake_one();

    void lock();
    void unlock();

    pircontext context();
    void swap_buffers();
    void done_current();
    void make_current();

protected://线程内部执行
    virtual void paints(){}
    virtual void initializes(){}
    virtual void events(const bs_event &) {}
    void render_activa(bool loop = false);//若是true则自动运行，否则运行一次

public :
    void update() {render_activa();}

private:
    impl::base *impl;
friend class context_thread;
};

#endif // WINDOW_BASE_H
