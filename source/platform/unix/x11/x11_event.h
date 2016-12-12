#ifndef X11_EVENT_H
#define X11_EVENT_H

#include "x11_header.h"
#include "ext/x11_inputmethod.h"


class x11_event : public x11_inputmethod
{
    friend class mouse_tracking;
private:
    int inx;
    x11_extension *x11_ext;
public:
    x11_event(x11_extension *ext);
    ~x11_event();

    virtual void send_event(const bs_event &evt) = 0;
    virtual void send_event(const XEvent &xEvent) = 0;

public:
    bool route(const XEvent &xEvent);

private:
    bool windows(const XEvent &xEvent);
    bool mouse(const XEvent &xEvent);
    bool keybord(const XEvent &xEvent);

    int getmodifiers(const XEvent& event);
    uint getmounsewhlee(const XEvent& event);
    uint getmousestate(const XEvent& event );
    uint getmouseaction(const XEvent& event );

    uint getkey (const XEvent &event);
    int xkb_translatekc (Display *display, int scancode, int is_shift = 0);
};


#endif // X11EVENT_H
