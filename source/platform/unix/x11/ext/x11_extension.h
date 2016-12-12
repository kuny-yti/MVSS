#ifndef X11_EXTENSION_H
#define X11_EXTENSION_H

#include "interface/iwcontext.h"
#include "../x11_header.h"



#ifdef hasX11ExtXKb
#include <X11/XKBlib.h>
#include <X11/Xmd.h>
#include <X11/Xatom.h>
#include <X11/Xresource.h>
#endif

#ifdef hasX11ExtXRandR
#include <X11/extensions/Xrandr.h>
#endif

#ifdef hasX11ExtXinerama
#include <X11/extensions/Xinerama.h>
#endif

#ifdef hasX11ExtXcb

typedef XID xcb_window_t;
typedef XID xcb_visualid_t;
typedef struct xcb_connection_t xcb_connection_t;
typedef xcb_connection_t* (* XGETXCBCONNECTION_T)(Display*);
#endif

#ifdef hasX11ExtXKbcommon
#include <xkbcommon/xkbcommon.h>
#endif

#ifdef hasX11ExtXf86vmode
#include <X11/extensions/xf86vmode.h>
#endif

// Action for EWMH client messages
#define _NET_WM_STATE_REMOVE        0
#define _NET_WM_STATE_ADD           1
#define _NET_WM_STATE_TOGGLE        2

class x11_extension
{
    friend class x11_monitor;
    friend class x11_clipboard;
    friend class x11_drag_drop;
    friend class x11_input;
    friend class x11_inputmethod;
    friend class x11_window_manager;
public:
    x11_extension(iwcontext *wc);
    ~x11_extension();
    void select(struct timeval* timeout);

public:
    Display *display(){return wctx->parent<Display*>();}
    Window root() {return DefaultRootWindow(display());}
    int screen() {return DefaultScreen(display());}
    int depth() {return DefaultDepth(display(), screen());}
    Window handle(){return wctx->handle<Window>();}

    Atom supported(Atom* atoms, ulong count, const char* name);
    ulong property(Window window, Atom property, Atom type, uchar** value);

private:
    iwcontext *wctx;
#ifdef hasX11ExtXKb
    typedef struct {
        int         available;
        int         detectable;
        int         majorOpcode;
        int         eventBase;
        int         errorBase;
        int         major;
        int         minor;
    } tX11Xkbs;
    tX11Xkbs xkb;
#endif
#ifdef hasX11ExtXRandR
    typedef struct {
        int         available;
        int         eventBase;
        int         errorBase;
        int         major;
        int         minor;
        int         gammaBroken;
        int         monitorBroken;
    } tX11XRandRs;
    tX11XRandRs randr;
#endif
#ifdef hasX11ExtXinerama
    typedef struct {
        int         available;
        int         major;
        int         minor;
    } tX11Xineramas;
    tX11Xineramas inerama;
#endif
#ifdef hasX11ExtXcb
    typedef struct {
        void*       handle;
        XGETXCBCONNECTION_T XGetXCBConnection;
    } tX11Xcbs;
    tX11Xcbs xcb;
#endif
#ifdef hasX11ExtXf86vmode
    typedef struct {
        int         available;
        int         eventBase;
        int         errorBase;
    } tX11VidModes;
    tX11VidModes vidmode;
#endif
    Atom        UTF8_STRING;
};

#endif // X11_EXTENSION_H
