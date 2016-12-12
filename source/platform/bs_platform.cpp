#include "bs_platform.h"

#include "x11/x11_window.h"
#include "x11/x11_cursor.h"
#include "x11/x11_monitor.h"
#include "x11/ext/x11_extension.h"

namespace impl {
static x11_cursor *_cursor = NULL;
static x11_monitor *_monitor = NULL;
static piwindow  _window;
}

bs_platform::bs_platform (const piwindow &w)
{
    impl::_window = w;
}
bs_platform::~bs_platform()
{
    if (impl::_cursor)
        bsDelete(impl::_cursor);
    if (impl::_monitor)
        bsDelete(impl::_monitor);
    impl::_cursor = NULL;
    impl::_monitor = NULL;
}
void bs_platform::set(const piwindow &pf)
{
    impl::_window = pf;
}
piwindow bs_platform::get()
{
    return impl::_window ;
}

#if bsPlatform == bsPlatform_Linux
#include "x11/x11_window.h"
#include "x11/x11_monitor.h"
#include "x11/x11_cursor.h"
piwindow bs_platform::create()
{
    bs_ptr(x11_window) pw = bs_malloc(x11_window);
    x11_extension *ext = (x11_extension *)pw->ext ();

    if (impl::_cursor)
        bsDelete(impl::_cursor);
    if (impl::_monitor)
        bsDelete(impl::_monitor);
    impl::_cursor = bsNew (x11_cursor(ext));
    impl::_monitor = bsNew (x11_monitor(ext));
    return pw;
}
#endif
int bs_platform::dispatch()
{
    return impl::_window->dispatch ();
}
void bs_platform::post(const tWindowNexus &evt)
{
    impl::_window->post (evt);
}

bool bs_platform::monitor_change(const tMonitorInofs &mi, bool is_fullscreen)
{
    return false;
}
tMonitorInofs bs_platform::monitor(const int )
{
    return tMonitorInofs();
}
int bs_platform::monitor_count()const
{
    return 0;
}

// mouse
bool bs_platform::cursor_set(const bs_image &)
{
    return false;
}
bs_image bs_platform::cursor_get() const
{
    return bs_image();
}
void bs_platform::cursor_set(eCursorTypes ct)
{
    impl::_cursor->set (ct);
}
void bs_platform::cursor_show(bool state)
{
    impl::_cursor->show (state);
}
void bs_platform::cursor_position(const ivec2 &s)
{
    impl::_cursor->position (s);
}
ivec2 bs_platform::cursor_position() const
{
    return impl::_cursor->position();
}
void bs_platform::cursor_event(const bs_event &e)
{
    impl::_cursor->event (e);
}
