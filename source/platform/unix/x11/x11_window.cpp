#include "x11_window.h"
#include "x11_header.h"

struct xserver
{
    Display *disp;

    xserver(iplatform::iwindow *_self)
    {
        if (connect(NULL))
        {
            _self->parents = (winid)disp;
            return ;
        }
        log_error("X server connection failed.");
    }
    ~xserver()
    {
        disconnect();
    }
    Display* connect(const char *dname)
    {
    #if !defined(X_HAVE_UTF8_STRING)
        if (strcmp(setlocale(LC_CTYPE, NULL), "C") == 0)
            setlocale(LC_CTYPE, "");

    #else
        if (setlocale(LC_ALL, "") == NULL)
            return NULL;
    #endif

        if (dname == NULL)
            dname = getenv("DISPLAY");

        XInitThreads();
        disp = XOpenDisplay(dname);
        return disp;
    }
    void disconnect()
    {
        XCloseDisplay(disp);
    }
};

namespace wctx
{

Display *display(iwcontext *wr){return wr->parent<Display*>();}
Window root(iwcontext *wr) {return DefaultRootWindow(display(wr));}
int screen(iwcontext *wr) {return DefaultScreen(display(wr));}
int depth(iwcontext *wr) {return DefaultDepth(display(wr), screen(wr));}
XVisualInfo* config(iwcontext *wr){return wr->rcontext->config<XVisualInfo*>();}
GLXContext context(iwcontext *wr){return wr->rcontext->context<GLXContext>();}
Window handle(iwcontext *wr){return wr->handle<Window>();}

}

namespace
{
static Bool WaitForNotify( Display*, XEvent *e, char *arg )
{
    return (e->type == MapNotify) && (e->xmap.window == (Window)arg);
}
}

#include "ext/x11_clipboard.h"
#include "ext/x11_drag_drop.h"
#include "ext/x11_extension.h"
#include "ext/x11_inputmethod.h"
#include "ext/x11_window_manager.h"
#include "x11_event.h"

namespace impl {
struct window :public xserver, public x11_extension, public x11_event
{
    x11_window_manager *manage;
    x11_clipboard *clipboard;
    x11_drag_drop *xdnd;
    iplatform::iwindow *self;

    bool is_exit;

    window(iplatform::iwindow *_self):
        xserver(_self),
        x11_extension(_self),
        x11_event(this),
        manage(bsNew (x11_window_manager(this))),
        clipboard(bsNew (x11_clipboard(this))),
        xdnd(bsNew (x11_drag_drop(this))),
        self(_self) {is_exit = false;}
    ~window()
    {
        bsDelete (manage);
        bsDelete (clipboard);
        bsDelete (xdnd);
    }
    void send_event(const bs_event &evt)
    {
        tWindowNexus nexus;
        nexus.source = self;
        nexus.target = NULL;
        nexus.event = evt;
        self->emit(&nexus);
    }
    virtual void send_event(const XEvent &xEvent)
    {
        static bool is_initw = false;
        const int w =  DisplayWidth(wctx::display(self), wctx::screen(self));
        const int h =  DisplayHeight(wctx::display(self), wctx::screen(self));
        const int ww = xEvent.xconfigure.width;
        const int wh = xEvent.xconfigure.height;
        const int wx = xEvent.xconfigure.x;
        const int wy = xEvent.xconfigure.y;

        if (xEvent.type == Expose)
        {
            if ((bs_bound(1, ww, w) == ww && bs_bound(1, wh, h) == wh) &&
                 ivec2(ww, wh) != self->format.geometry.xy ())
            {
                if (ww < self->format.maxisize.width && ww > self->format.minisize.width)
                    self->format.geometry.width = ww;
                if (wh < self->format.maxisize.height && wh > self->format.minisize.height)
                    self->format.geometry.height = wh;
            }
            if (wx != self->format.geometry.x || wy != self->format.geometry.y)
            {
                if (self->format.style & Window_FullScreen || xEvent.xany.send_event)
                {
                    self->format.geometry.x = wx;
                    self->format.geometry.y = wy;
                }
            }

            if (!is_initw)
            {
                send_event(bs_event(Window_Initialize));
                is_initw = true;
            }
            else
                send_event(bs_event(Window_Paint));
        }
        else if (xEvent.type == ConfigureNotify)
        {
            if ((bs_bound(1, ww, w) == ww && bs_bound(1, wh, h) == wh) &&
                 ivec2(ww, wh) != self->format.geometry.xy ())
            {
                if (ww < self->format.maxisize.width && ww > self->format.minisize.width)
                    self->format.geometry.width = ww;
                if (wh < self->format.maxisize.height && wh > self->format.minisize.height)
                    self->format.geometry.height = wh;
            }
            if (wx != self->format.geometry.x || wy != self->format.geometry.y)
            {
                if (self->format.style & Window_FullScreen || xEvent.xany.send_event)
                {
                    self->format.geometry.x = wx;
                    self->format.geometry.y = wy;
                }
            }
            send_event(bs_event(Window_Resize));
        }
        else if (xEvent.type == ClientMessage)
        {
            if (manage->event(xEvent))
                return ;
            else if (clipboard->event(xEvent))
                return ;
            else if (xdnd->event(xEvent))
                return ;
        }
        else if (xEvent.type == SelectionNotify)
            xdnd->drop(xEvent);
        else if (xEvent.type == FocusIn)
        {
            if (xEvent.xfocus.mode == NotifyGrab || xEvent.xfocus.mode == NotifyUngrab)
            {
                // Ignore focus events from popup indicator windows, window menu
                // key chords and window dragging
                return;
            }

            setIc();
        }
        else if (xEvent.type ==  FocusOut)
        {
            if (xEvent.xfocus.mode == NotifyGrab || xEvent.xfocus.mode == NotifyUngrab)
            {
                // Ignore focus events from popup indicator windows, window menu
                // key chords and window dragging
                return;
            }

            unsetIc();
        }
    }

    bool create_drawable (Visual* visInfo)
    {
        if (self->rcontext->format.target == Render_OffScreen ||
            self->rcontext->format.target == Render_Image)
        {
            Window h =  _create_window (visInfo);
            self->handles = (winid)h;
            return h != NULL;
        }
        else if (self->rcontext->format.target == Render_PBuffer)
        {
            if (!wctx::handle(self))
            {
                log_error("GLX PBuffer handle null.");
                return false;
            }
            return true;
        }

        return create_window( visInfo );
    }
private:
    bool create_window( Visual* visInfo)
    {
        if (self->format.style & Window_FullScreen)
        {
            self->format.geometry.height = DisplayHeight(wctx::display(self), wctx::screen(self) );
            self->format.geometry.width = DisplayWidth(wctx::display(self), wctx::screen(self));
        }

        XID drawable = _create_window( visInfo);

        //manage->setWMProtocol();

        //xdnd->setProtocol();

        //x11_inputmethod::setProtocol();

        /*XMapWindow(impl::wctx::display(self_event), drawable );

            XEvent event;
            XIfEvent(impl::wctx::display(self_event), &event, WaitForNotify, (XPointer)(drawable) );

            XMoveResizeWindow(impl::wctx::display(self_event), drawable,
                              self_event->format.geometry.x, self_event->format.geometry.y,
                              self_event->format.geometry.width(), self_event->format.geometry.height());*/
        XFlush (wctx::display(self));


        // Grab keyboard focus in fullscreen mode
        //if(self_event->format.style & Window_FullScreen || !(self_event->format.style & Window_Rimless))
        //    XGrabKeyboard(impl::wctx::display(self_event), drawable, True, GrabModeAsync, GrabModeAsync, CurrentTime );

        self->handles = (winid)drawable;

        return true;
    }

    Window _create_window (Visual* visInfo)
    {
        if(!visInfo)
        {
            log_error("GLX visual null.");
            return NULL;
        }

        XSetWindowAttributes wa;
        wa.colormap = XCreateColormap(wctx::display(self), wctx::root(self), visInfo, AllocNone );
        wa.background_pixmap = None;
        wa.border_pixel = 0;
        wa.event_mask = StructureNotifyMask | VisibilityChangeMask | ExposureMask |
                FocusChangeMask | PropertyChangeMask;
        if (self->format.event & Window_MouseTrack)
            wa.event_mask |= EnterWindowMask | LeaveWindowMask;
        if (self->format.event & Window_Keybord)
            wa.event_mask |= KeyPressMask | KeyReleaseMask;
        if (self->format.event & Window_Mouse)
            wa.event_mask |= ButtonPressMask | ButtonReleaseMask | PointerMotionMask;

        if (self->format.style & Window_Rimless)
            wa.override_redirect = False;
        else
            wa.override_redirect =True;
        if (self->format.style & Window_FullScreen)
            wa.override_redirect =False;

        int rimless = CWBorderPixel | CWColormap;
        if (self->format.style & Window_FullScreen)
            rimless |= CWBackPixmap | CWOverrideRedirect;
        if (self->format.event & Window_Keybord ||
                self->format.event & Window_Mouse ||
                self->format.event & Window_DragDrop ||
                self->format.event & Window_Clipboard ||
                self->format.event & Window_MouseTrack)
            rimless |= CWEventMask;

        manage->enableError ();

        Window drawable = XCreateWindow(wctx::display(self), wctx::root(self),
                                        self->format.geometry.x, self->format.geometry.y,
                                        self->format.geometry.width, self->format.geometry.height,
                                        0, wctx::depth(self),
                                        self->format.event & Window_InputOut ? InputOutput : InputOnly,
                                        visInfo, rimless, &wa);

        manage->disableError ();

        if( !drawable )
        {
            log_error("X create window failed.");
            return NULL;
        }

        XStoreName(wctx::display(self), drawable, self->format.title.to_latin1());

        return drawable;
    }
};
}

void *x11_window::ext()
{
    return impl;
}
x11_window::x11_window(iwindow *parent):
    iwindow(parent)
{
     impl = bsNew (impl::window(this));
}
x11_window::~x11_window()
{
    bsDelete (impl);
}

void x11_window::fullscreen(bool s)
{
    impl->manage->setWindowState (Window_FullScreen);
}
/// 置顶窗口
void x11_window::top(bool s )
{

}
/// 关闭窗口
/// 最小化
void x11_window::minimum()
{
    impl->manage->setWindowState (Window_Minimum);
}
/// 最大化
void x11_window::maximun()
{
    impl->manage->setWindowState (Window_Maximum);
}
/// 正常显示
void x11_window::normal()
{
    impl->manage->setWindowState (Window_Show);
}

/// 设置窗口是否为开启状态
void x11_window::enable(bool s)
{

}
void x11_window::disable(bool s )
{

}

/// 设置窗口是否为隐藏状态
void x11_window::hide(bool s)
{
    if (s)
        impl->manage->setWindowState(Window_Hide);
    else
        show();
}
void x11_window::show(bool s )
{
    if (s)
        impl->manage->setWindowState(Window_Show);
    else
        hide();
}

/// 窗口的尺寸
void x11_window::size(const ivec2 &rect)
{
    impl->manage->setWindowSize (rect);
}
void x11_window::resize(const ivec2 &rect)
{
    impl->manage->setWindowSize (rect);
}

/// 窗口的位置
void x11_window::pos(const ivec2 &p)
{

}
void x11_window::move(const ivec2 &p)
{

}

/// 窗口内的客户区域几何大小
void x11_window::geometry(const ivec4 &rect)
{

}
void x11_window::title(const bs_string &til)
{

}

int x11_window::dispatch()
{
    while (!impl->is_exit)
    {
        //while (XPending(x11globle->display()))
        {
            XEvent xEvent;
            XNextEvent (wctx::display(this), &xEvent );
            if (XFilterEvent(&xEvent, wctx::handle(this)))
                continue;
            if (KeymapNotify == xEvent.type)
            {
                XRefreshKeyboardMapping(&xEvent.xmapping);
                continue;
            }

            impl->route(xEvent);
        }
    }
    impl->send_event(bs_event(Window_Exit));
}
void x11_window::post(const tWindowNexus &evt)
{
    if (evt.event.is_window ())
    {
        if (evt.event.code () == Window_Exit)
            impl->is_exit = true;
    }
}
bool x11_window::create(ircontext *rctx, const tWindowFormats &wf)
{
    if (!rctx)
    {
        log_error("Window context No context rendering.");
        return false;
    }

    bool r = false;
    iwcontext::create(rctx, wf);
    Visual* fbc = NULL;
    /*if (rctx->format.client == Render_Default)
            fbc = DefaultVisual(impl::wctx::display(this), impl::wctx::screen(this));
        else if (rctx->format.client == Render_OpenGL && impl::wctx::config(this))*/
    fbc = wctx::config(this)->visual;
    /*else
        {
            log_error("X render client not.");
            return false;
        }*/

    r = impl->create_drawable(fbc);
    if (!r)
    {
        log_error("X create window drawable failed.");
        return false;
    }

    return r;
}
void x11_window::close()
{
    XSync( wctx::display(this), False );
    this->rcontext->close();
    if(wctx::handle(this))
    {
        if (this->rcontext->format.target != Render_PBuffer)
            XDestroyWindow(wctx::display(this), wctx::handle(this));
        handles  = NULL;
    }
}

void x11_window::callback(tObjectNexus *param)
{

}
