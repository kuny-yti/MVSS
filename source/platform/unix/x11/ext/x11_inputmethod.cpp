#include "x11_inputmethod.h"
#include "tools/bs_datetime.h"

static bool hasUsableIMStyle(XIM im)
{
    unsigned int i;
    bool found = false;
    XIMStyles* styles = NULL;

    if (XGetIMValues(im, XNQueryInputStyle, &styles, NULL) != NULL)
        return false;

    for (i = 0;  i < styles->count_styles;  i++)
    {
        if (styles->supported_styles[i] == (XIMPreeditNothing | XIMStatusNothing))
        {
            found = true;
            break;
        }
    }

    XFree(styles);
    return found;
}

x11_inputmethod::x11_inputmethod(x11_extension *ext):
    ext(ext),
    im(NULL),
    ic(NULL)
{
    if (XSupportsLocale())
    {
        XSetLocaleModifiers("");

        im = XOpenIM(ext->display(), 0, NULL, NULL);
        if (im)
        {
            if (!hasUsableIMStyle(im))
            {
                XCloseIM(im);
                im = NULL;
            }
        }
    }
}
x11_inputmethod::~x11_inputmethod()
{
    if (ic)
    {
        XDestroyIC(ic);
        ic = NULL;
    }
    if (im)
    {
        XCloseIM(im);
        im = NULL;
    }
}



void x11_inputmethod::setProtocol()
{
    if (im)
    {
        ic = XCreateIC(im, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, ext->handle(), NULL);

        if (ic)
            XSetICFocus(ic);
    }
}
void x11_inputmethod::setIc()
{
    if (ic)
        XSetICFocus(ic);
}
void x11_inputmethod::unsetIc()
{
    if (ic)
        XUnsetICFocus(ic);
}
// Decode a Unicode code point from a UTF-8 stream
// Based on cutef8 by Jeff Bezanson (Public Domain)
//
#if defined(X_HAVE_UTF8_STRING)
static uint decodeUTF8(const char** s)
{
    uint ch = 0, count = 0;
    static const uint offsets[] =
    {
        0x00000000u, 0x00003080u, 0x000e2080u,
        0x03c82080u, 0xfa082080u, 0x82082080u
    };

    do
    {
        ch = (ch << 6) + (uchar) **s;
        (*s)++;
        count++;
    } while ((**s & 0xc0) == 0x80);

    bsASSERT(count <= 6, "");
    return ch - offsets[count - 1];
}
#endif /*X_HAVE_UTF8_STRING*/
static int translateState(int state)
{
    int mods = 0;

    if (state & ( ShiftMask | LockMask ))
        mods |= Event_ModifierShift;
    if (state & ControlMask)
        mods |= Event_ModifierControl;
    if (state & Mod1Mask)
        mods |= Event_ModifierAlt;
    if (state & Mod4Mask)
        mods |= Event_ModifierSuper;

    return mods;
}
bool x11_inputmethod::event(const XEvent &evt)
{
    static uint imkey;
    int filtered;
    if (im)
        filtered = XFilterEvent((XEvent*)&evt, None);

    if (ic && !filtered)
    {
        if (evt.type == KeyPress)
        {
            int count;
            Status status;
#if defined(X_HAVE_UTF8_STRING)
            char buffer[100];
            char* chars = buffer;

            count = Xutf8LookupString(ic, (XKeyPressedEvent*)&evt.xkey, buffer, sizeof(buffer) - 1,
                                      NULL, &status);

            if (status == XBufferOverflow)
            {
                chars = (char*)calloc(count + 1, 1);
                count = Xutf8LookupString(ic, (XKeyPressedEvent*)&evt.xkey, chars, count, NULL, &status);
            }

            if (status == XLookupChars || status == XLookupBoth)
            {
                const char* c = chars;
                chars[count] = '\0';
                while (c - chars < count)
                {
                    imkey = decodeUTF8(&c);
                    send_event(bs_event(imkey | Event_StateImDown, (eEventModifiers)translateState(evt.xkey.state),
                                        bs_datetime::microsecond()));
                }
            }
#else /*X_HAVE_UTF8_STRING*/
            wchar_t buffer[16];
            wchar_t* chars = buffer;

            count = XwcLookupString(ic, &evt.xkey,
                                    buffer, sizeof(buffer) / sizeof(wchar_t),
                                    NULL, &status);

            if (status == XBufferOverflow)
            {
                chars = calloc(count, sizeof(wchar_t));
                count = XwcLookupString(ic, &evt.xkey, chars, count, NULL, &status);
            }

            if (status == XLookupChars || status == XLookupBoth)
            {
                int i;
                for (i = 0;  i < count;  i++)
                {
                    imkey = chars[i];
                    send_event(bs_event(imkey | Event_StateImDown, (eEventModifiers)translateState(evt.xkey.state),
                                        bs_datetime::microsecond()));
                }
            }
#endif /*X_HAVE_UTF8_STRING*/

            if (chars != buffer)
                free(chars);
            return true;
        }
        else if (evt.type == KeyRelease)
        {
            send_event(bs_event(imkey | Event_StateImUp, (eEventModifiers)translateState(evt.xkey.state),
                                bs_datetime::microsecond()));
            return true;
        }
    }
    return false;
}
