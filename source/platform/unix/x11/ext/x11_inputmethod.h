#ifndef X11_INPUTMETHOD_H
#define X11_INPUTMETHOD_H

#include "x11_extension.h"
#include "bs_event.h"

class x11_inputmethod
{
    friend class mouse_tracking;
public:
    x11_inputmethod(x11_extension *ext);
    ~x11_inputmethod();

    virtual void send_event(const bs_event &evt) = 0;

    void setProtocol();
    void setIc();
    void unsetIc();
    bool event(const XEvent &evt);
private:
    x11_extension *ext;

    XIM im;
    XIC ic;
};

#endif // X11_INPUTMETHOD_H
