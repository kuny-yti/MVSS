#include "x11_window_manager.h"
#include <unistd.h>

// X error handler
//
static int errorHandler(Display *display, XErrorEvent* event)
{
    log_errors("X Error code: ", event->error_code);
    return 0;
}

x11_window_manager::x11_window_manager(x11_extension *ext):
    ext(ext)
{
    req_exit = false;
    detectEWMH();

    ext->UTF8_STRING = XInternAtom(ext->display(), "UTF8_STRING", False);
    // ICCCM，EWMH和Motif窗口属性原子, 即使没有WM支持也可以安全地设置, 需要WM支持的EWMH原子在检测EWMH
    WM_PROTOCOLS = XInternAtom(ext->display(), "WM_PROTOCOLS", False);
    WM_STATE = XInternAtom(ext->display(), "WM_STATE", False);
    WM_DELETE_WINDOW = XInternAtom(ext->display(), "WM_DELETE_WINDOW", False);
    NET_WM_ICON = XInternAtom(ext->display(), "_NET_WM_ICON", False);
    NET_WM_PING = XInternAtom(ext->display(), "_NET_WM_PING", False);
    NET_WM_PID =  XInternAtom(ext->display(), "_NET_WM_PID", False);
    NET_WM_NAME = XInternAtom(ext->display(), "_NET_WM_NAME", False);
    NET_WM_ICON_NAME = XInternAtom(ext->display(), "_NET_WM_ICON_NAME", False);
    NET_WM_BYPASS_COMPOSITOR = XInternAtom(ext->display(), "_NET_WM_BYPASS_COMPOSITOR", False);
    MOTIF_WM_HINTS = XInternAtom(ext->display(), "_MOTIF_WM_HINTS", False);
}
x11_window_manager::~x11_window_manager()
{

}
void x11_window_manager::enableError()
{
    XSetErrorHandler(errorHandler);
}
void x11_window_manager::disableError()
{
    XSync(ext->display(), False);
    XSetErrorHandler(NULL);
}
bool x11_window_manager::is_exit()const
{
    return req_exit;
}
bool x11_window_manager::detectEWMH()
{
    // 检测运行的窗口管理器是否符合EWMH
    Window* windowFromRoot = NULL;
    Window* windowFromChild = NULL;
    const Atom supportingWmCheck = XInternAtom(ext->display(), "_NET_SUPPORTING_WM_CHECK", False);
    const Atom wmSupported = XInternAtom(ext->display(), "_NET_SUPPORTED", False);

    // 查找根窗口的_NET_SUPPORTING_WM_CHECK属性
    if (ext->property(ext->root(), supportingWmCheck, XA_WINDOW, (uchar**) &windowFromRoot) != 1)
    {
        if (windowFromRoot)
            XFree(windowFromRoot);
        return false;
    }

    enableError ();
    // 在子窗口中寻找和根窗口相同的属性
    if (ext->property(*windowFromRoot, supportingWmCheck, XA_WINDOW, (uchar**) &windowFromChild) != 1)
    {
        XFree(windowFromRoot);
        if (windowFromChild)
            XFree(windowFromChild);
        return false;
    }
    disableError ();

    // 两个相同的属性应该属于同一个子窗口ID
    if (*windowFromRoot != *windowFromChild)
    {
        XFree(windowFromRoot);
        XFree(windowFromChild);
        return false;
    }
    XFree(windowFromRoot);
    XFree(windowFromChild);

    // 已经确定有符合EWMH正在运行
    Atom* supportedAtoms;
    ulong atomCount = ext->property(ext->root(), wmSupported, XA_ATOM, (uchar**) &supportedAtoms);

    // 获取当前管理器支持那些WM原子
    NET_WM_STATE = ext->supported(supportedAtoms, atomCount, "_NET_WM_STATE");
    NET_WM_STATE_ABOVE = ext->supported(supportedAtoms, atomCount, "_NET_WM_STATE_ABOVE");
    NET_WM_STATE_FULLSCREEN = ext->supported(supportedAtoms, atomCount, "_NET_WM_STATE_FULLSCREEN");
    NET_WM_STATE_MAXIMIZED_VERT = ext->supported(supportedAtoms, atomCount, "_NET_WM_STATE_MAXIMIZED_VERT");
    NET_WM_STATE_MAXIMIZED_HORZ = ext->supported(supportedAtoms, atomCount, "_NET_WM_STATE_MAXIMIZED_HORZ");
    NET_WM_FULLSCREEN_MONITORS = ext->supported(supportedAtoms, atomCount, "_NET_WM_FULLSCREEN_MONITORS");
    NET_WM_WINDOW_TYPE = ext->supported(supportedAtoms, atomCount, "_NET_WM_WINDOW_TYPE");
    NET_WM_WINDOW_TYPE_NORMAL = ext->supported(supportedAtoms, atomCount, "_NET_WM_WINDOW_TYPE_NORMAL");
    NET_ACTIVE_WINDOW = ext->supported(supportedAtoms, atomCount, "_NET_ACTIVE_WINDOW");
    NET_FRAME_EXTENTS = ext->supported(supportedAtoms, atomCount, "_NET_FRAME_EXTENTS");
    NET_REQUEST_FRAME_EXTENTS = ext->supported(supportedAtoms, atomCount, "_NET_REQUEST_FRAME_EXTENTS");

    XFree(supportedAtoms);
    // 检测EWMH结束
    return true;
}

bool x11_window_manager::event(const XEvent &evt)
{
    if (evt.xclient.message_type == WM_PROTOCOLS)
    {
        const Atom protocol = evt.xclient.data.l[0];
        if (protocol == WM_DELETE_WINDOW)
        {
            // The window manager was asked to close the window, for example by
            // the user pressing a 'close' window decoration button
            req_exit = true;
            printf("close\n");
        }
        else if (protocol == NET_WM_PING)
        {
            // The window manager is pinging the application to ensure it's
            // still responding to events
            XEvent reply = evt;
            reply.xclient.window = ext->root();

            XSendEvent(ext->display(), ext->root(), False,
                       SubstructureNotifyMask | SubstructureRedirectMask, &reply);
        }
        return true;
    }
    return false;
}
eWindowStates x11_window_manager::getWindowState()
{
    XWindowAttributes wa;
    XGetWindowAttributes(ext->display(), ext->handle(), &wa);
    if (wa.map_state == IsViewable)
        return Window_Show;
    return Window_StateNot;
}
void x11_window_manager::setWindowState(int s, int full_inx)
{
    if (s == Window_Hide)
        XUnmapWindow(ext->display(), ext->handle());
    else if (s == Window_Show)
        XMapWindow(ext->display(), ext->handle());
    else if (s == Window_Minimum)
        ;
    else if (s == Window_Maximum)
    {
        if (NET_WM_STATE && NET_WM_STATE_MAXIMIZED_VERT && NET_WM_STATE_MAXIMIZED_HORZ)
            sendEventToWM(NET_WM_STATE, _NET_WM_STATE_ADD, NET_WM_STATE_MAXIMIZED_VERT,
                          NET_WM_STATE_MAXIMIZED_HORZ, 1, 0);
    }
    else if (s == Window_FullScreen)
        toggle_fullscreen(full_inx);

    XFlush(ext->display());
}
void x11_window_manager::setWindowTitle(const char* title)
{
#if defined(X_HAVE_UTF8_STRING)
    Xutf8SetWMProperties(ext->display(), ext->handle(),
                         title, title, NULL, 0, NULL, NULL, NULL);
#else
    XmbSetWMProperties(ext->display(), ext->handle(),
                       title, title, NULL, 0, NULL, NULL, NULL);
#endif

    XChangeProperty(ext->display(), ext->handle(), NET_WM_NAME, UTF8_STRING,
                    8, PropModeReplace, (uchar*) title, strlen(title));

    XChangeProperty(ext->display(), ext->handle(), NET_WM_ICON_NAME, UTF8_STRING,
                    8, PropModeReplace, (uchar*) title, strlen(title));

    XFlush(ext->display());
}
// Sends an EWMH or ICCCM event to the window manager
void x11_window_manager::sendEventToWM(Atom type, long a, long b, long c, long d, long e)
{
    XEvent event;
    memset(&event, 0, sizeof(event));

    event.type = ClientMessage;
    event.xclient.window = ext->handle();
    event.xclient.format = 32; // Data is 32-bit longs
    event.xclient.message_type = type;
    event.xclient.data.l[0] = a;
    event.xclient.data.l[1] = b;
    event.xclient.data.l[2] = c;
    event.xclient.data.l[3] = d;
    event.xclient.data.l[4] = e;

    XSendEvent(ext->display(), ext->root(), False, SubstructureNotifyMask | SubstructureRedirectMask, &event);
}
void x11_window_manager::setWindowIcon(int count, const bs_image* images)
{
    if (count)
    {
        int i, j, longCount = 0;

        for (i = 0;  i < count;  i++)
            longCount += 2 + images[i].size().width * images[i].size().height;

        long* icon = (long*)calloc(longCount, sizeof(long));
        long* target = icon;

        for (i = 0;  i < count;  i++)
        {
            *target++ = images[i].size().width;
            *target++ = images[i].size().height;

            for (j = 0;  j < images[i].size().width * images[i].size().height;  j++)
            {
                *target++ = (images[i].bits()[j * 4 + 0] << 16) |
                            (images[i].bits()[j * 4 + 1] <<  8) |
                            (images[i].bits()[j * 4 + 2] <<  0) |
                            (images[i].bits()[j * 4 + 3] << 24);
            }
        }

        XChangeProperty(ext->display(), ext->handle(), NET_WM_ICON,
                        XA_CARDINAL, 32, PropModeReplace, (uchar*) icon, longCount);

        free(icon);
    }
    else
    {
        XDeleteProperty(ext->display(), ext->handle(), NET_WM_ICON);
    }

    XFlush(ext->display());
}
void x11_window_manager::setWindowPos(const ivec2 &pos)
{
    if (!(getWindowState() && Window_Show))
    {
        long supplied;
        XSizeHints* hints = XAllocSizeHints();

        if (XGetWMNormalHints(ext->display(),ext->handle(), hints, &supplied))
        {
            hints->flags |= PPosition;
            hints->x = hints->y = 0;

            XSetWMNormalHints(ext->display(),ext->handle(), hints);
        }

        XFree(hints);
    }

    XMoveWindow(ext->display(),ext->handle(), pos.x, pos.y);
    XFlush(ext->display());
}
void x11_window_manager::setWindowSize(const ivec2 &size)
{
    if (!(ext->wctx->format.style && Window_Resizable))
        normal_window(size);

    XResizeWindow(ext->display(), ext->handle(), size.width, size.height);

    XFlush(ext->display());
}

vec2 x11_window_manager::getWindowPos()
{
    Window child;
    int x, y;

    XTranslateCoordinates(ext->display(),ext->handle(), ext->root(), 0, 0, &x, &y, &child);

    if (child)
    {
        int left, top;
        XTranslateCoordinates(ext->display(),ext->handle(), child, 0, 0, &left, &top, &child);

        x -= left;
        y -= top;
    }

    return vec2(x, y);
}
vec2 x11_window_manager::getWindowSize()
{
    XWindowAttributes attribs;
    XGetWindowAttributes(ext->display(),ext->handle(), &attribs);

    return vec2(attribs.width, attribs.height);
}

void x11_window_manager::setWMProtocol()
{
    if (!(ext->wctx->format.style & Window_Rimless))
    {
        struct
        {
            ulong flags;
            ulong functions;
            ulong decorations;
            long input_mode;
            ulong status;
        } hints;

        hints.flags = 2;       // Set decorations
        hints.decorations = 0; // No decorations

        XChangeProperty(ext->display(), ext->handle(), MOTIF_WM_HINTS, MOTIF_WM_HINTS,
                        32, PropModeReplace, (uchar*) &hints, sizeof(hints) / sizeof(long));
    }
    // Declare the WM protocols supported by
    {
        Atom protocols[] =
        {
            WM_DELETE_WINDOW,
            NET_WM_PING
        };

        XSetWMProtocols(ext->display(), ext->handle(), protocols, sizeof(protocols) / sizeof(Atom));
    }
    // Declare our PID
    {
        const pid_t pid = ::getpid();

        XChangeProperty(ext->display(), ext->handle(), NET_WM_PID, XA_CARDINAL, 32,
                        PropModeReplace, (uchar*) &pid, 1);
    }
    if (NET_WM_WINDOW_TYPE && NET_WM_WINDOW_TYPE_NORMAL)
    {
        Atom type = NET_WM_WINDOW_TYPE_NORMAL;
        XChangeProperty(ext->display(), ext->handle(), NET_WM_WINDOW_TYPE, XA_ATOM, 32,
                        PropModeReplace, (uchar*) &type, 1);
    }
    // Set ICCCM WM_HINTS property
    {
        XWMHints* hints = XAllocWMHints();
        if (!hints)
        {
            printf("X11: Failed to allocate WM hints\n");
            return ;
        }

        hints->flags = StateHint;
        hints->initial_state = NormalState;

        XSetWMHints(ext->display(), ext->handle(), hints);
        XFree(hints);
    }
}

void x11_window_manager::toggle_fullscreen(int index )
{
    if (index >= 0)
    {
        if (ext->inerama.available && NET_WM_FULLSCREEN_MONITORS)
            sendEventToWM(NET_WM_FULLSCREEN_MONITORS, index, index, index, index, 0);

        if (NET_WM_STATE && NET_WM_STATE_FULLSCREEN)
            sendEventToWM(NET_WM_STATE, _NET_WM_STATE_ADD, NET_WM_STATE_FULLSCREEN, 0, 1, 0);
        else
        {
            XSetWindowAttributes attributes;
            attributes.override_redirect = True;
            XChangeWindowAttributes(ext->display(), ext->handle(), CWOverrideRedirect, &attributes);

            //window->x11.overrideRedirect = GLFW_TRUE;
        }

        // Enable compositor bypass
        {
            const ulong value = 1;

            XChangeProperty(ext->display(),  ext->handle(), NET_WM_BYPASS_COMPOSITOR,
                            XA_CARDINAL, 32, PropModeReplace, (uchar*) &value, 1);
        }
    }
    else
    {
        if (ext->inerama.available && NET_WM_FULLSCREEN_MONITORS)
            XDeleteProperty(ext->display(),  ext->handle(), NET_WM_FULLSCREEN_MONITORS);

        if (NET_WM_STATE && NET_WM_STATE_FULLSCREEN)
            sendEventToWM(NET_WM_STATE, _NET_WM_STATE_REMOVE, NET_WM_STATE_FULLSCREEN, 0, 1, 0);
        else
        {
            XSetWindowAttributes attributes;
            attributes.override_redirect = False;
            XChangeWindowAttributes(ext->display(),  ext->handle(), CWOverrideRedirect, &attributes);

            //window->x11.overrideRedirect = GLFW_FALSE;
        }

        // Disable compositor bypass
        {
            XDeleteProperty(ext->display(),  ext->handle(), NET_WM_BYPASS_COMPOSITOR);
        }
    }
}
void x11_window_manager::normal_window(const ivec2 &size)
{
    XSizeHints* hints = XAllocSizeHints();

    if (ext->wctx->format.style & Window_Resizable)
    {
        if (ext->wctx->format.minisize != ivec2(-1, -1))
        {
            hints->flags |= PMinSize;
            hints->min_width = ext->wctx->format.minisize.width;
            hints->min_height = ext->wctx->format.minisize.height;
        }

        if (ext->wctx->format.maxisize != ivec2(-1, -1))
        {
            hints->flags |= PMaxSize;
            hints->max_width = ext->wctx->format.maxisize.width;
            hints->max_height = ext->wctx->format.maxisize.height;
        }

        if (ext->wctx->format.minaspect != vec2(-1, -1))
        {
            hints->flags |= PAspect;
            hints->min_aspect.x = ext->wctx->format.minaspect.x;
            hints->min_aspect.y = ext->wctx->format.minaspect.y;

        }
        if (ext->wctx->format.maxaspect != vec2(-1, -1))
        {
            hints->max_aspect.x = ext->wctx->format.maxaspect.x;
            hints->max_aspect.y = ext->wctx->format.maxaspect.y;
        }
    }
    else
    {
        hints->flags |= (PMinSize | PMaxSize);
        hints->min_width  = hints->max_width  = size.width;
        hints->min_height = hints->max_height = size.height;
    }

    XSetWMNormalHints(ext->display(), ext->handle(), hints);
    XFree(hints);
}
