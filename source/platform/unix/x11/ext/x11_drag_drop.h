#ifndef X11_DRAG_DROP_H
#define X11_DRAG_DROP_H

#include "x11_extension.h"
#include "tools/bs_string.h"

class x11_drag_drop
{
public:
    x11_drag_drop(x11_extension *ext);
    ~x11_drag_drop();

    void setProtocol();
    bool event(const XEvent &evt);
    bool drop(const XEvent &evt);

private:
    x11_extension *ext;
    bs_string text;
    Window source;

    Atom        UTF8_STRING;
    // Xdnd (drag and drop) atoms
    Atom        XdndAware;
    Atom        XdndEnter;
    Atom        XdndPosition;
    Atom        XdndStatus;
    Atom        XdndActionCopy;
    Atom        XdndDrop;
    Atom        XdndLeave;
    Atom        XdndFinished;
    Atom        XdndSelection;
};

#endif // X11_DRAG_DROP_H
