#include "x11_cursor.h"
#include <X11/cursorfont.h>

x11_cursor::x11_cursor(x11_extension *e)
{
    ext = e;
    //back = XCreateFontCursor(ext->display(), 68);
}
x11_cursor::~x11_cursor()
{

}
bool x11_cursor::set(const bs_image &)
{
    return false;
}
bs_image x11_cursor::get() const
{
    return bs_image();
}

static uint x_cursor (eCursorTypes c)
{
    switch ((int)c)
    {
    case Cursor_IBeam: return XC_xterm;
    case Cursor_Cross: return XC_cross;
    case Cursor_Hand: return XC_hand1;
    case Cursor_Wait: return XC_watch;
    case Cursor_DragMove: return 0;
    case Cursor_DragCopy: return 0;
    case Cursor_DragLink: return 0;
    case Cursor_WhatsThis: return 0;
    case Cursor_Forbidden: return 0;
    case Cursor_Arrow:
    default:
        break;
    }
    return XC_arrow;
}
void x11_cursor::set(eCursorTypes ct)
{
    back = XCreateFontCursor(ext->display(), x_cursor(ct));
    XDefineCursor(ext->display(), ext->handle(), back);
}
/*
static void update(Cursor hand)
{
    if (hand)
        XDefineCursor(ext->display(), ext->handle(), hand);
    else
        XUndefineCursor(ext->display(), hand);
}*/
void x11_cursor::show(bool state)
{
    static Cursor cursorNone = None;
    if (state)
    {
        if (back)
            XDefineCursor(ext->display(), ext->handle(), back);
        else
            XUndefineCursor(ext->display(), ext->handle());
    }
    else
    {
        if( cursorNone == None )
        {
            char cursorNoneBits[ 32 ];
            XColor dontCare;
            Pixmap cursorNonePixmap;
            memset( cursorNoneBits, 0, sizeof( cursorNoneBits ) );
            memset( &dontCare, 0, sizeof( dontCare ) );
            cursorNonePixmap = XCreateBitmapFromData ( ext->display (), ext->root (),
                                                       cursorNoneBits, 16, 16 );
            if( cursorNonePixmap != None )
            {
                cursorNone = XCreatePixmapCursor(ext->display (),
                                                  cursorNonePixmap, cursorNonePixmap,
                                                  &dontCare, &dontCare, 0, 0 );
                XFreePixmap(ext->display (), cursorNonePixmap );
            }
        }
        XDefineCursor(ext->display(), ext->handle(), cursorNone);
    }
}
void x11_cursor::position(const vec2 &pos)
{
    XWarpPointer(ext->display (), None, ext->handle (), 0, 0, 0, 0, pos.x, pos.y);
    /* Make the warp visible immediately. */
    XFlush(ext->display ());
}
vec2 x11_cursor::position() const
{
    Window formroot, tmpwin;
    int wposx, wposy, rposx, rposy;
    uint tmpnull;
    XQueryPointer(ext->display(), ext->root(), &formroot, &tmpwin,
                  &rposx, &rposy, &wposx, &wposy, &tmpnull);
    return vec2(wposx, wposy);
}
void x11_cursor::event(const bs_event &)
{

}
