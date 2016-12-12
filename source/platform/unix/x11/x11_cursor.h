#ifndef X11_CURSOR_H
#define X11_CURSOR_H

#include "interface/iplatform.h"
#include "ext/x11_extension.h"

class x11_cursor
{
public:
    x11_cursor(x11_extension *e);
    ~x11_cursor();

    virtual bool set(const bs_image &);
    virtual bs_image get() const;
    virtual void set(eCursorTypes ct);
    virtual void show(bool state = true) ;
    virtual void position(const vec2 &s);
    virtual vec2 position() const ;
    virtual void event(const bs_event &e) ;

private:
    x11_extension *ext;
    Cursor back;
};

#endif // X11_CURSOR_H
