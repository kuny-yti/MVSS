#include "x11_extension.h"
#include <dlfcn.h>

x11_extension::x11_extension(iwcontext *wc):
    wctx(wc)
{
#ifdef hasX11ExtXf86vmode
    vidmode.available = XF86VidModeQueryExtension(display(), &vidmode.eventBase,
                                                  &vidmode.errorBase);
#endif
#ifdef hasX11ExtXRandR
    if (XRRQueryExtension(display(), &randr.eventBase, &randr.errorBase))
    {
        if (XRRQueryVersion(display(), &randr.major, &randr.minor))
        {
            if (randr.major > 1 || randr.minor >= 3)
                randr.available = 1;
        }
        else
        {
            printf("X11: Failed to query RandR version");
        }
    }
    if (randr.available)
    {
        XRRScreenResources* sr = XRRGetScreenResources(display(), root());

        if (!sr->ncrtc || !XRRGetCrtcGammaSize(display(), sr->crtcs[0]))
        {
            printf("X11: RandR gamma ramp support seems broken");
            randr.gammaBroken = true;
        }

        XRRFreeScreenResources(sr);

        XRRSelectInput(display(), root(), RROutputChangeNotifyMask);
    }
#endif
#ifdef hasX11ExtXinerama
    if (XineramaQueryExtension(display(), &inerama.major, &inerama.minor))
    {
        if (XineramaIsActive(display()))
            inerama.available = 1;
    }
#endif

#ifdef hasX11ExtXKb
    xkb.major = 1;
    xkb.minor = 0;
    xkb.available = XkbQueryExtension(display(), &xkb.majorOpcode, &xkb.eventBase,
                                      &xkb.errorBase, &xkb.major, &xkb.minor);

    if (xkb.available)
    {
        Bool supported;

        if (XkbSetDetectableAutoRepeat(display(), True, &supported))
        {
            if (supported)
                xkb.detectable = 1;
        }
    }
#endif
#ifdef hasX11ExtXcb
    xcb.handle = dlopen("libX11-xcb.so", RTLD_LAZY | RTLD_GLOBAL);
    if (xcb.handle)
    {
        xcb.XGetXCBConnection = (XGETXCBCONNECTION_T) dlsym(xcb.handle, "XGetXCBConnection");
    }
#endif
}
x11_extension::~x11_extension()
{
#ifdef hasX11ExtXcb
    if (xcb.handle)
    {
        dlclose(xcb.handle);
        xcb.handle = NULL;
    }
#endif
}
Atom x11_extension::supported(Atom* supportedAtoms, ulong atomCount, const char* atomName)
{
    ulong i;
    const Atom atom = XInternAtom(display(), atomName, False);

    for (i = 0;  i < atomCount;  i++)
    {
        if (supportedAtoms[i] == atom)
            return atom;
    }

    return None;
}
ulong x11_extension::property(Window window, Atom property, Atom type, uchar** value)
{
    Atom actualType;
    int actualFormat;
    ulong itemCount, bytesAfter;

    XGetWindowProperty(display(), window, property, 0, LONG_MAX, False, type,
                       &actualType, &actualFormat, &itemCount, &bytesAfter, value);

    if (type != AnyPropertyType && actualType != type)
        return 0;

    return itemCount;
}
void x11_extension::select(struct timeval* timeout)
{
    fd_set fds;
    int result, count;
    const int fd = ConnectionNumber(display());

    count = fd + 1;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    /*
#if defined(__linux__)
    FD_SET(_glfw.linux_js.inotify, &fds);

    if (fd < _glfw.linux_js.inotify)
        count = _glfw.linux_js.inotify + 1;
#endif*/

    // NOTE: Only retry on EINTR if there is no timeout, as select is not
    //       required to update it for the time elapsed
    // TODO: Update timeout value manually
    do
    {
        result = ::select(count, &fds, NULL, NULL, timeout);
    }
    while (result == -1 && errno == EINTR && timeout == NULL);
}
