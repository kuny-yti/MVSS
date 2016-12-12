#include "bs_gui.h"
#include "tools/bs_timer.h"
#include "gui/bs_guiprivate.h"


namespace impl {
struct screen:public tWidgetManages
{
    bs_timer event_time;         //处理事件所用计时器
    bs_mutex mutex;              //保证不被多线程访问
    bs_gui *self;
    screen(bs_gui *ui, bool enabel_msaa = true):
        event_time(),
        mutex(),
        self(ui)
    {
        focus = (NULL);
        hover = (NULL);
        active = (NULL);
        top = (NULL);
        enter = (NULL);
        ui_parent = NULL;
        leave = NULL;
        dump = Dump_Not;

        self->context = (uicontext*)nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG | (enabel_msaa ? NVG_ANTIALIAS : 0));
        if (self->context == NULL)
            log_error("Could not init nanovg.");
    }
    ~screen()
    {
        nvgDeleteGL3(uiContext());
        self->context = NULL;
    }
};

}

bs_gui::bs_gui(bs_object *parent):
    igui(parent)
{
    impl = bsNew (impl::screen(this));
    impl->event_time.start();
}
bs_gui::~bs_gui()
{
    bsDelete (impl);
}

struct hover_times: public bs_timer
{
    iwidget *last_start;
    impl::screen *sc;
    bs_event ent_evt;
    hover_times ():
        bs_timer(TimerMode_Timing, UiEventHoverLimt),
        sc(NULL)
    {
        start (UiEventHoverLimt);
    }
    ~hover_times()
    {
        stop();
    }

    void detect(impl::screen *i, const bs_event &evt)
    {
        sc = i;
        ent_evt = evt;
        if (last_start == sc->leave && !sc->enter)
            stop();
        else if (sc->enter != sc->leave)
        {
            last_start = sc->enter;
            start (UiEventHoverLimt);
        }
    }

    virtual void callback(tObjectNexus *)
    {
        ent_evt.timestamp () += UiEventHoverLimt;
        if (sc)
            sc->dispatch (ent_evt);
        stop();
    }
};

void bs_gui::event(const bs_event &evt)
{
    static fpoint last_cursor_pos;
    static hover_times htime;
    impl->mutex.lock();

    impl->hover = NULL;
    if (evt.button () & Event_StateMove)
    {
        last_cursor_pos.x = evt.x ();
        last_cursor_pos.y = evt.y ();
    }
    bs_event _event(evt);
    _event.timestamp () = impl->event_time.elapsed();
    impl->dispatch(_event);
    impl->event_time.restart();

    if (evt.button () & Event_StateMove)
        htime.detect (impl, evt);

    impl->mutex.unlock();
}
void bs_gui::paint()
{
    impl->mutex.lock();

    nvgBeginFrame (uiContext(), viewport.width, viewport.height, 1.f);

    nvgBeginPath(uiContext());
    nvgRect (uiContext(), viewport.x, viewport.y, viewport.width , viewport.height);
    nvgFillColor (uiContext(), uiRGBA(bs_color(19,22,24,255)));
    nvgFill(uiContext());

    impl->render (viewport.width, viewport.height);

    nvgEndFrame(uiContext());
    impl->mutex.unlock();
}

piwidget bs_gui::create(int ut, const piwidget &p)
{
    return piwidget();
}
